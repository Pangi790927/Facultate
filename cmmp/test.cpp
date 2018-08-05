#include <iostream>
#include "stdafx.h"
#include <fstream>
#include "cmmp.h"
#include "test.h"
#include "json.h"
#include "parse.h"
#define AE_NO_EXCEPTIONS
#define AE_THREADING AE_SERIAL_UNSAFE
#include "interpolation.h"

// http://www.alglib.net/translator/man/manual.cpp.html#example_lsfit_d_nlf
// file:///home/pangi/Facultate/cmmp/cpp/manual.cpp.html#sub_lsfitcreatef

int main()
{

	std::vector<double> va {1., 2., 3., 4., 5., 6., 7., 8., 9., 10.};
	alglib::real_1d_array x;
	x.setcontent(10, &va[0]);

	// lsfitcreatef()

	nlohmann::json data;
	std::ifstream("data.json") >> data;

	std::cout << "data:" << std::endl;
	for (auto&& elem : data["data"])
		std::cout << elem << std::endl;

	std::vector<std::pair<std::string, double>> v(
		{
			std::make_pair(std::string("a"), 20.),
			std::make_pair(std::string("b"), 15.)
		}
	);

	std::cout << util::eval_expr<double>("a + b", v) << std::endl;
	return 0;
}