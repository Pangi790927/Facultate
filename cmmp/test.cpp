#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "solver.h"
#include "parse.h"

int main()
{
	auto solvers = solver::from_json("data.json");

	try {
		for (auto&& solv : solvers) {
			solv.solve();
			std::cout << solv.str_result() << std::endl;
		}
	}
	catch (alglib::ap_error& err) {
		std::cout << err.msg << std::endl;
	}
	return 0;
}
