#ifndef PARSE_H
#define PARSE_H

#include "exprtk.h"

namespace util
{
	struct evaluator_t {
		exprtk::symbol_table<double> symbol_table;
		exprtk::expression<double> expression;
		exprtk::parser<double> parser;
		std::vector<double> vals;
		std::string expr;

		double eval(const std::vector<std::pair<std::string, double>>& values);
		void init (const std::string& expr,
				const std::vector<std::pair<std::string, double>>& values);
	};

	double eval_expr (const std::string& expr,
			const std::vector<std::pair<std::string, double>>& values);

	double eval_expr(const std::string& expr,
			std::initializer_list<std::pair<std::string, double>> values);
}

#endif