// fms_perceptron.h
// A perceptron is a hyperplane separating two sets of points in R^n.
// Given sets S_0 and S_1, find a vector w and a scalar b such that
// w.x + b < 0 for x in S_0 and w.x + b > 0 for x in S_1.
#pragma once

#define MDSPAN_USE_PAREN_OPERATOR 1
#include <experimental/mdspan>
#include <experimental/linalg>

namespace fms::perceptron {

    // Update vector w and scalar b given point x and label y in {-1, 1}
    template<class T, class S, ::std::size_t E>
    void update(std::mdspan<T, std::extents<S,E>>& w, T& b,
        std::mdspan<const T, std::extents<S, E>> x, int y, double alpha = 1.0)
    {
        using std::experimental::linalg::add;
        using std::experimental::linalg::dot;
        using std::experimental::linalg::scaled;

        // Compute w·x + b
        double prediction = dot(w, x) + b;

        // Check if misclassified
        if (prediction * y < 0) {
            // Update: w = w + alpha y x    ,
            add(w, scaled(alpha * y, x), w);

            // Update bias
            b += alpha * y;
        }
    }

 
} // namespace fms::perceptron
