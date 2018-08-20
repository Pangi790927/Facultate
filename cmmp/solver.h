#ifndef CMMP_H
#define CMMP_H

#include <memory>
#include <sstream>
#include "interpolation.h"
#include "parse.h"
#include "solver_data.h"
#include "json.h"

// http://www.alglib.net/translator/man/manual.cpp.html#example_lsfit_d_nlf
// /cpp/manual.cpp.html

struct solver {
	std::string expr;
	std::vector<std::string> in;
	std::vector<std::string> params;
	std::string out;

	alglib::real_1d_array param_val;
	alglib::lsfitstate state;
	alglib::lsfitreport rep;
	alglib::ae_int_t info;
	alglib::ae_int_t maxits = 10000;

	std::vector<double> last_sol;
	util::evaluator_t evaluator;
	bool hadError = false;
	float r;
	float g;
	float b;

	solver (const std::string& expr,
			const std::vector<std::string>& in,
			const std::vector<std::string>& params,
			const std::string& out);
	static void stat_callback(const alglib::real_1d_array &c,
			const alglib::real_1d_array& x, double &res, void *ptr);
	void callback(const alglib::real_1d_array &c,
			const alglib::real_1d_array& x, double &res);
	std::vector<double> solve (const solver_data& data);
	double _eval_at (const std::vector<double>& x);
	std::string str_result();
	static std::shared_ptr<solver> from_json (nlohmann::json& jdata);

	template <typename Type>
	double eval_at (Type&& arg, int count) {
		std::vector<double> value(count);

		for (int i = 0; i < count; i++)
			value[i] = arg[i];
		_eval_at(value);
	}
};


#endif