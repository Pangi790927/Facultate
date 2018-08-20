#include "parse.h"
#include <iostream>

namespace util
{
	void evaluator_t::init (const std::string& expr,
			const std::vector<std::pair<std::string, double>>& values)
	{
		this->expr = expr;

		for (int i = 0; i < values.size(); i++)
			vals.push_back(values[i].second);

		for (int i = 0; i < values.size(); i++) {
			symbol_table.add_variable(values[i].first, vals[i]);
		}

		symbol_table.add_constants();
		expression.register_symbol_table(symbol_table);
		parser.compile(expr, expression);
	}

	double evaluator_t::eval(
			const std::vector<std::pair<std::string, double>>& values)
	{
		for (int i = 0; i < values.size(); i++)
			vals[i] = values[i].second;

		return expression.value();
	}

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