#include "solver_window.h"

static Math::Point2f fitIn (Math::Point2f point, Math::Point2f bl,
		Math::Point2f tr)
{
	float width = std::fabs(tr.x - bl.x);
	float height = std::fabs(tr.y - bl.y);

	return Math::Point2f((point.x - bl.x) / width * 2 - 1,
			(point.y - bl.y) / height * 2 - 1);
}

template <typename Func>
static void plot (Func&& func, Math::Point2f bl, Math::Point2f tr, int complex = 100) {
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

static void draw_circle (Math::Point2f c, float rad, int n = 30) {
	const float pi = 3.141592653589;
	const float step = 2 * pi / n;

	glBegin(GL_LINES);
		for (float a = 0; a <= 2 * pi; a += step) {
			glVertex2f(sin(a) * rad + c.x, cos(a) * rad + c.y);
			glVertex2f(sin(a + step) * rad + c.x, cos(a + step) * rad + c.y);
		}
	glEnd();
}

solver_window::solver_window (nlohmann::json& jdata)
: window(jdata["width"], jdata["height"], jdata["name"])
{}

void solver_window::solve() {
	for (auto&& solv : aprox) {
		if (!solv)
			throw std::runtime_error("solver was null");
		solv->solve(*dataset);
	}
}

void solver_window::draw() {
	if (dataset->x.cols() != 1)
		return ;

	window.focus();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	for (auto&& solv : aprox) {
		glColor3f(solv->r, solv->g, solv->b);
		if (!solv->hadError)
			plot(
				[&] (double x) -> double {
					return solv->eval_at(std::vector<double>{x}, 1);
				},
				dataset->botLeft,
				dataset->topRight,
				1000
			);
		glBegin (GL_POINTS);
			glColor3f(1, 1, 1);
			for (int i = 0; i < dataset->x.rows(); i++) {
				double x = dataset->x[i][0];
				double y = dataset->y[i];
				draw_circle(fitIn(Math::Point2f(x, y),
						dataset->botLeft,
						dataset->topRight), 0.01);
			}
		glEnd();
	}
	window.swapBuffers();
}