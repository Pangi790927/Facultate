#include "solver.h"
#include "parse.h"
// strange bug detected, sometimes the value is not detected correctly

solver::solver (const std::string& expr,
		const std::vector<std::string>& in,
		const std::vector<std::string>& params,
		const std::string& out = "f(x)")
: expr(expr), in(in), out(out), params(params)
{
	if (expr.empty())
		throw std::runtime_error("solver: empty expr");
	if (in.empty())
		throw std::runtime_error("solver: empty in names");
	if (params.empty())
		throw std::runtime_error("solver: empty params names");
	last_sol = std::vector<double>(params.size());

	std::vector<std::pair<std::string, double>> par;
	for (int i = 0; i < params.size(); i++)
		par.push_back(std::make_pair(params[i], 0));
	for (int i = 0; i < in.size(); i++)
		par.push_back(std::make_pair(in[i], 0));
	evaluator.init(expr, par);
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

	res = evaluator.eval(par);
}

std::vector<double> solver::solve (const solver_data& data) {
	using namespace alglib;

	param_val.setlength(params.size());
	real_1d_array lower_bound;
	real_1d_array upper_bound;

	lower_bound.setlength(params.size());
	upper_bound.setlength(params.size());

	for (int i = 0; i < params.size(); i++)
		lower_bound[i] = 0.0000000001;
	for (int i = 0; i < params.size(); i++)
		upper_bound[i] = fp_posinf;

	double diff_step = 0.0001;
	lsfitcreatef(data.x, data.y, param_val, diff_step, state);

	double epsx = 0.000001;
	lsfitsetcond(state, epsx, maxits);
	lsfitsetbc(state, lower_bound, upper_bound);
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

	mae = 0;
	for (int i = 0; i < data.x.rows(); i++)
		mae += std::fabs(data.y[i] - eval_at(data.x[i], data.x.cols()));
	mae /= (double)data.x.rows();

	rmse = 0;
	for (int i = 0; i < data.x.rows(); i++)
		rmse += std::pow(data.y[i] - eval_at(data.x[i], data.x.cols()), 2);
	rmse /= (double)data.x.rows();
	rmse = sqrt(rmse);

	return last_sol;
}

double solver::_eval_at (const std::vector<double>& x) {
	std::vector<std::pair<std::string, double>> par;
	if (x.size() != in.size())
		return std::nan("");

	for (int i = 0; i < params.size(); i++)
		par.push_back(std::make_pair(params[i], last_sol[i]));
	for (int i = 0; i < in.size(); i++)
		par.push_back(std::make_pair(in[i], x[i]));
	return evaluator.eval(par);
}

std::string solver::str_result() {
	std::stringstream ss;

	ss << out << " = " << expr << std::endl;
	if (hadError)
		ss << "\t" << "Results are invalid: " << int(info) << std::endl;
	for (int i = 0; i < last_sol.size(); i++)
		ss << "\t" << params[i] << ": " << last_sol[i] << std::endl;
	ss << "mae: " << mae << std::endl;
	ss << "rmse: " << rmse << std::endl; 
	return ss.str();
}

std::shared_ptr<solver> solver::from_json (nlohmann::json& jdata) {
	std::string expr = jdata["expr"].get<std::string>();
	std::string out = jdata["out"].get<std::string>();
	std::vector<std::string> in;
	std::vector<std::string> params;

	for (auto&& elem : jdata["in"])
		in.push_back(elem.get<std::string>());

	for (auto&& elem : jdata["params"])
		params.push_back(elem.get<std::string>());

	std::shared_ptr<solver> solv = std::shared_ptr<solver>(
		new solver(expr, in, params, out)
	);

	if (jdata.find("iter") == jdata.end())
		solv->maxits = 1000;
	else
		solv->maxits = jdata["iter"];	

	if (jdata.find("color") == jdata.end()) {
		solv->r = 1;
		solv->g = 1;
		solv->b = 1;
	}
	else {
		solv->r = jdata["color"]["r"].get<double>();
		solv->g = jdata["color"]["g"].get<double>();
		solv->b = jdata["color"]["b"].get<double>();
	}

	return solv;
}