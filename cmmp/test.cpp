#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "cmmp.h"
#include "test.h"
#include "parse.h"

int main()
{
	auto solvers = solver::from_json("data.json");

	try {
		for (auto&& solv : solvers)
			std::cout << solv.solve_to_str(solv.solve()) << std::endl;
	}
	catch (alglib::ap_error& err) {
		std::cout << err.msg << std::endl;
	}
	return 0;
}