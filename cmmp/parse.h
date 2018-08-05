#ifndef PARSE_H
#define PARSE_H

#include "exprtk.h"

namespace util
{
	template <typename T>
	T eval_expr (const std::string& expr,
			const std::vector<std::pair<std::string, T>>& values)
	{
		typedef exprtk::symbol_table<double> symbol_table_t;
		typedef exprtk::expression<T> expression_t;
		typedef exprtk::parser<T> parser_t;

		symbol_table_t symbol_table;

		std::deque<T> vals;
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

	template <typename T>
	T eval_expr(const std::string& expr,
			std::initializer_list<std::pair<std::string, T>> values)
	{
		return eval_expr(expr, values);
	}
}

#endif