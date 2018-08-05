#include <iostream>
#include <fstream>
#include "cmmp.h"
#include "test.h"
#include "json.h"
#include "parse.h"

int main()
{
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