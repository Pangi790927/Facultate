#ifndef SOLVER_DATA_H
#define SOLVER_DATA_H

#include "interpolation.h"
#include "MathLib.h"
#include <memory>

struct solver_data {
	alglib::real_2d_array x;
	alglib::real_1d_array y;
	Math::Point2f botLeft;
	Math::Point2f topRight;

	solver_data (const std::vector<std::vector<double>>& data_x,
			const std::vector<double>& data_y);
	static std::shared_ptr<solver_data> from_file(const std::string& filename);
};

#endif