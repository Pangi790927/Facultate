#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>
#include "solver.h"
#include "parse.h"
#include "OpenglWindow.h"
#include "MathLib.h"

Math::Point2f fitIn (Math::Point2f point, Math::Point2f bl,
		Math::Point2f tr)
{
	float width = std::fabs(tr.x - bl.x);
	float height = std::fabs(tr.y - bl.y);

	return Math::Point2f((point.x - bl.x) / width * 2 - 1,
			(point.y - bl.y) / height * 2 - 1);
}

template <typename Func>
void plot (Func&& func, Math::Point2f bl, Math::Point2f tr, int complex = 100) {
	float width = std::fabs(tr.x - bl.x);
	float step = width / float(complex);

	for (float x = bl.x; x < tr.x; x += step) {
		float x1 = x;
		float x2 = x + step;
		float y1 = func(x);
		float y2 = func(x + step);

		glBegin(GL_LINES);
			glVertex2fv(fitIn(Math::Point2f(x1, y1), bl, tr).getPtr());
			glVertex2fv(fitIn(Math::Point2f(x2, y2), bl, tr).getPtr());
		glEnd();
	}
}

void draw_circle (Math::Point2f c, float rad, int n = 30) {
	const float pi = 3.141592653589;
	const float step = 2 * pi / n;

	glBegin(GL_LINES);
		for (float a = 0; a <= 2 * pi; a += step) {
			glVertex2f(sin(a) * rad + c.x, cos(a) * rad + c.y);
			glVertex2f(sin(a + step) * rad + c.x, cos(a + step) * rad + c.y);
		}
	glEnd();
}

struct solver_plot {
	Math::Point2f botLeft;
	Math::Point2f topRight;
	std::map<double, double> values;
	solver& solv;

	solver_plot (solver& solv) : solv(solv) {
		botLeft = Math::Point2f(
			(*std::min_element(solv.data_x.begin(),
					solv.data_x.end()))[0],
			(*std::min_element(solv.data_y.begin(),
					solv.data_y.end()))
		);
		topRight = Math::Point2f(
			(*std::max_element(solv.data_x.begin(),
					solv.data_x.end()))[0],
			(*std::max_element(solv.data_y.begin(),
					solv.data_y.end()))
		);
	}

	double operator () (double x) {
		if (values.find(x) == values.end())
			values[x] = solv.eval_at(std::vector<double>(1, x), 1);
		return values[x];
	}
};

int main(int argc, char const *argv[])
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

	std::vector<std::shared_ptr<solver_plot>> solver_plots;
	for (auto&& solv : solvers)
		if (solv.data_x[0].size() != 1)
			std::cout << "multidimensional expresion can't be ploted"
					<< "expr: " << solv.expr << std::endl;
		else
			solver_plots.push_back(std::shared_ptr<solver_plot>(
					new solver_plot(solv)));

	int width = 600;
	int height = 300;

	OpenglWindow window(width, height, "cmmp");
	
	while (window.active) {
		static float x = 0;
		static float y = 0;
		
		if (window.handleInput()) {
			if (window.keyboard.getKeyState(window.keyboard.ESC))
				window.requestClose();

			x = -(1.0f - window.mouse.x / (float)window.width * 2);
			y = 1.0f - window.mouse.y / (float)window.height * 2;
		}
		
		window.focus();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (auto&& solv_plot : solver_plots) {
			if (!solv_plot)
				continue;
			glColor3f(solv_plot->solv.r, solv_plot->solv.g, solv_plot->solv.b);
			plot(
				[&] (double x) {
					return (*solv_plot)(x);
				},
				solv_plot->botLeft,
				solv_plot->topRight,
				1000
			);

			glBegin (GL_POINTS);
				glColor3f(1, 1, 1);
				for (int i = 0; i < solv_plot->solv.data_x.size(); i++) {
					double x = solv_plot->solv.data_x[i][0];
					double y = solv_plot->solv.data_y[i];
					draw_circle(fitIn(Math::Point2f(x, y),
							solv_plot->botLeft,
							solv_plot->topRight), 0.01);
				}
			glEnd();
		}
		window.swapBuffers();
	}
	return 0;
}