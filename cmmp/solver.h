#ifndef CMMP_H
#define CMMP_H

#include <sstream>
#include "interpolation.h"

// http://www.alglib.net/translator/man/manual.cpp.html#example_lsfit_d_nlf
// /cpp/manual.cpp.html

struct solver {
	std::string expr;
	std::vector<std::string> in;
	std::vector<std::string> params;
	std::vector<std::vector<double>> data_x;
	std::vector<double> data_y;
	std::string out;

	alglib::real_1d_array param_val;
	alglib::lsfitstate state;
	alglib::lsfitreport rep;
	alglib::ae_int_t info;
	alglib::ae_int_t maxits = 100;

	std::vector<double> last_sol;
	bool hadError = false;

	solver (const std::string& expr,
			const std::vector<std::string>& in,
			const std::vector<std::string>& params,
			const std::vector<std::vector<double>>& data_x,
			const std::vector<double>& data_y,
			const std::string& out);
	static void stat_callback(const alglib::real_1d_array &c,
			const alglib::real_1d_array& x, double &res, void *ptr);
	void callback(const alglib::real_1d_array &c,
			const alglib::real_1d_array& x, double &res);
	std::vector<double> solve();
	std::string str_result();
	static std::vector<solver> from_json (const std::string& jname);
};


#endif