#include "parse.h"
#include <iostream>

namespace util
{
	double eval_expr (const std::string& expr,
			const std::vector<std::pair<std::string, double>>& values)
	{
		typedef exprtk::symbol_table<double> symbol_table_t;
		typedef exprtk::expression<double> expression_t;
		typedef exprtk::parser<double> parser_t;

		symbol_table_t symbol_table;

		std::deque<double> vals;
		for (auto&& val : values) {
			vals.push_back(val.second);
			symbol_table.add_variable(val.first, vals.back());
		}
		symbol_table.add_constants();

		expression_t expression;
		expression.register_symbol_table(symbol_table);

		parser_t parser;
		parser.compile(expr, expression);

		return expression.value();
	}

	double eval_expr(const std::string& expr,
			std::initializer_list<std::pair<std::string, double>> values)
	{
		return eval_expr(expr, values);
	}
}