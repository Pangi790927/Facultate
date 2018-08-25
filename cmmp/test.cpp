#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <memory>
#include "solver.h"
#include "solver_data.h"
#include "solver_window.h"
#include "parse.h"
#include "MathLib.h"

int main(int argc, char const *argv[])
{
	nlohmann::json jdata;
	try {
		std::ifstream("data.json") >> jdata;
	}
	catch (std::exception& exc) {
		std::cout << "json load error: " << exc.what() << std::endl;
		throw;
	}

	std::map<std::string, std::shared_ptr<solver>> aprox;
	try {
		auto& japrox = jdata["aprox"];
		for (auto it = japrox.begin(); it != japrox.end(); it++) {
			aprox[it.key()] = solver::from_json(it.value());
		}
	}
	catch (std::exception& exc) {
		std::cout << "solvers load error: " << exc.what() << std::endl;
		throw;
	}

	std::map<std::string, std::shared_ptr<solver_data>> data_sets;
	try {
		auto& jdata_sets = jdata["data_sets"];
		for (auto it = jdata_sets.begin(); it != jdata_sets.end(); it++) {
			data_sets[it.key()] = solver_data::from_file(it.value());
		}
	}
	catch (std::exception& exc) {
		std::cout << "data sets load error: " << exc.what() << std::endl;
		throw;
	}

	std::vector<std::shared_ptr<solver_window>> windows;
	try {
		for (auto&& jwindow : jdata["windows"]) {
			windows.push_back(std::shared_ptr<solver_window>
					(new solver_window(jwindow)));
			windows.back()->dataset = data_sets[jwindow["dataset"]];
			for (auto&& solv : jwindow["aprox"])
				windows.back()->aprox.push_back(aprox[solv]);
			windows.back()->solve();

			std::string outname;
			static int number = 0;
			if (jwindow.find("outfile") == jwindow.end()) {
				if (jwindow.find("name") == jwindow.end())
					outname = "res" + std::to_string(number++) + ".res";
				else
					outname = jwindow["outfile"];
			}
			else
				outname = jwindow["outfile"];
			std::ofstream out(outname.c_str());
			
			if (jwindow["stdout"]) {
				std::cout << "---------------------------------------------"
						"-----------------------------------" << std::endl;
				std::cout << "### " << outname << ": ###" << std::endl;
				std::cout << "---------------------------------------------"
						"-----------------------------------" << std::endl;
			}
			for (auto&& solv : windows.back()->aprox) {
				out << solv->str_result() << std::endl;
				if (jwindow["stdout"]) {
					std::cout << solv->str_result() << std::endl;
				}
			}
		}
	}
	catch (std::exception& exc) {
		std::cout << "solver solve: " << exc.what() << std::endl;
		throw;
	}

	auto alive = [&]() {
		for (auto&& swin : windows) {
			if (swin)
				if (swin->window.active)
					return true;
		}
		return false;
	};

	while (alive()) {
		for (auto&& swin : windows) {
			if (swin->window.handleInput())
				if (swin->window.keyboard.getKeyState(
						swin->window.keyboard.ESC))
					swin->window.requestClose();
			swin->draw();
		}
	}

	return 0;
}