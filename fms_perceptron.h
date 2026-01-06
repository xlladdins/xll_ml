// fms_perceptron.h
// A perceptron is a hyperplane separating two sets of points in R^n.
// Given sets S_0 and S_1, find a vector w and a scalar b such that
// w.x + b < 0 for x in S_0 and w.x + b > 0 for x in S_1.
#pragma once

#include <experimental/mdspan>
#include <experimental/linalg>

namespace fms::perceptron {

	// Update vector w and scalar b given point x and label y in {-1, 1}
	void update(std::mdspan<double> w, double& b, 
		const std::mdspan<const double> x, int y, double alpha = 1.0)
	{
		using namespace std::experimental::linalg;

		double wx_b = linalg::dot(w, x, b);
		// w.x + b does not fit y
		if (wx_b * y < 0) {

		}
	}

} // namespace fms
