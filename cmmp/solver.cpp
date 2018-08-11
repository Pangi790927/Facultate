#include "solver.h"
#include "json.h"
#include "parse.h"

// strange bug detected, sometimes the value is not detected correctly

solver::solver (const std::string& expr,
		const std::vector<std::string>& in,
		const std::vector<std::string>& params,
		const std::vector<std::vector<double>>& data_x,
		const std::vector<double>& data_y,
		const std::string& out = "f(x)")
: expr(expr), in(in), out(out), params(params),
		data_x(data_x), data_y(data_y)
{
	if (data_x.empty())
		throw std::runtime_error("solver: empty data_x array");
	if (data_x[0].empty())
		throw std::runtime_error("solver: no components");
	if (data_y.empty())
		throw std::runtime_error("solver: empty data_y array");
	if (expr.empty())
		throw std::runtime_error("solver: empty expr");
	if (in.empty())
		throw std::runtime_error("solver: empty in names");
	if (params.empty())
		throw std::runtime_error("solver: empty params names");
}

void solver::stat_callback(const alglib::real_1d_array &c,
		const alglib::real_1d_array& x,
		double &res, void *ptr)
{
	static_cast<solver *>(ptr)->callback(c, x, res);
}

void solver::callback(const alglib::real_1d_array &c,
		const alglib::real_1d_array& x,
		double &res)
{
	std::vector<std::pair<std::string, double>> par;
	for (int i = 0; i < params.size(); i++)
		par.push_back(std::make_pair(params[i], c[i]));
	for (int i = 0; i < in.size(); i++)
		par.push_back(std::make_pair(in[i], x[i]));

	res = util::eval_expr(expr, par);
}

std::vector<double> solver::solve() {
	using namespace alglib;

	real_2d_array x;
	x.setlength(data_x.size(), data_x[0].size());
	
	for (int i = 0; i < data_x.size(); i++)
		for (int j = 0; j < data_y.size(); j++)
				x[i][j] = data_x[i][j];

	real_1d_array y;
	y.setcontent(data_y.size(), &data_y[0]);

	param_val.setlength(params.size());

	double diff_step = 0.0001;
	lsfitcreatef(x, y, param_val, diff_step, state);

	double epsx = 0.000001;
	lsfitsetcond(state, epsx, maxits);
	lsfitfit(state, stat_callback, NULL, (void *)this);
	lsfitresults(state, info, param_val, rep);

	hadError = false;
	last_sol = std::vector<double>();
	for (int i = 0; i < params.size(); i++) {
		if (int(info) == 2)
			last_sol.push_back(param_val[i]);
		else {
			last_sol.push_back(std::nan(""));
			hadError = true;
		}
	}
	return last_sol;
}

std::string solver::str_result() {
	std::stringstream ss;

	ss << out << " = " << expr << std::endl;
	if (hadError)
		ss << "\t" << "Results are invalid: " << int(info) << std::endl;
	for (int i = 0; i < last_sol.size(); i++)
		ss << "\t" << params[i] << ": " << last_sol[i] << std::endl;
	return ss.str();
}

std::vector<solver> solver::from_json (const std::string& jname) {
	nlohmann::json jdata;
	std::ifstream(jname.c_str()) >> jdata;

	std::vector<solver> solvers;

	std::vector<std::vector<double>> data_x;
	std::vector<double> data_y;

	for (auto&& pair : jdata["data"]) {
		data_x.push_back(std::vector<double>());
		for (auto&& x : pair["in"])
			data_x.back().push_back(x.get<double>());

		data_y.push_back(pair["out"].get<double>());
	}

	for (auto&& solv : jdata["aprox"]) {
		std::string expr = solv["expr"].get<std::string>();
		std::string out = solv["out"].get<std::string>();
		std::vector<std::string> in;
		std::vector<std::string> params;

		for (auto&& elem : solv["in"])
			in.push_back(elem.get<std::string>());

		for (auto&& elem : solv["params"])
			params.push_back(elem.get<std::string>());

		solvers.push_back(solver(expr, in, params, data_x, data_y, out));
	}
	return solvers;
}