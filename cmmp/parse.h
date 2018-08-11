#ifndef PARSE_H
#define PARSE_H

#include "exprtk.h"

namespace util
{
	double eval_expr (const std::string& expr,
			const std::vector<std::pair<std::string, double>>& values);

	double eval_expr(const std::string& expr,
			std::initializer_list<std::pair<std::string, double>> values);
}

#endif