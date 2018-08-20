#include "solver_data.h"
#include <fstream>
#include <sstream>

solver_data::solver_data (const std::vector<std::vector<double>>& data_x,
			const std::vector<double>& data_y)
{
	if (data_x.empty())
		throw std::runtime_error("solver: empty data_x array");
	if (data_x[0].empty())
		throw std::runtime_error("solver: no components");
	if (data_y.empty())
		throw std::runtime_error("solver: empty data_y array");

	x.setlength(data_x.size(), data_x[0].size());

	for (int i = 0; i < data_x.size(); i++)
		for (int j = 0; j < data_y.size(); j++)
				x[i][j] = data_x[i][j];

	y.setcontent(data_y.size(), &data_y[0]);

	botLeft = Math::Point2f(
		(*std::min_element(data_x.begin(), data_x.end()))[0],
		(*std::min_element(data_y.begin(), data_y.end()))
	);
	topRight = Math::Point2f(
		(*std::max_element(data_x.begin(), data_x.end()))[0],
		(*std::max_element(data_y.begin(), data_y.end()))
	);
}

std::shared_ptr<solver_data> solver_data::from_file(
		const std::string& filename)
{
	int n = 0;
	std::vector<std::vector<double>> data_x;
	std::vector<double> data_y;

	std::ifstream file(filename.c_str());

	std::string line;
	while (std::getline(file, line)) {
		if (line.size() != 0 && line[0] != '#') {
			std::stringstream ss(line);
			std::vector<double> values;

			double value;
			while (ss >> value)
				values.push_back(value);

			if (values.size() != 0) {
				if (n && n != values.size())
					throw std::runtime_error("value count of two"
							"lines differ!");

				if (!n)
					n = values.size();

				if (n < 2)
					throw std::runtime_error(
							"there must be at least an input and"
							"an output on each line");

				std::vector<double> in;
				for (int i = 0; i < n - 1; i++)
					in.push_back(values[i]);
				data_x.push_back(in);
				data_y.push_back(values[n - 1]);
			}
		}
	}
	return std::shared_ptr<solver_data>(new solver_data(data_x, data_y));
}
