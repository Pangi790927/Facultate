#ifndef SOLVER_WINDOW_H
#define SOLVER_WINDOW_H

#include "OpenglWindow.h"
#include "solver.h"
#include "solver_data.h"
#include <memory>
#include <vector>

struct solver_window {
	OpenglWindow window;
	std::shared_ptr<solver_data> dataset;
	std::vector<std::shared_ptr<solver>> aprox;

	solver_window (nlohmann::json& jdata);
	void solve();
	void draw();
};

#endif