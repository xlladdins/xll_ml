// fms_option_discrete.h - Discrete distribution for option pricing
#pragma once
#include <valarray>
#include "fms_option.h"

namespace fms::option::discrete {
	template<class F = double, class S = double>
	class model : public option::base<F, S> {
		std::valarray<F> xi, pi; // P(X = xi[i]) = pi[i]

		void normalize()
		{
			pi /= pi.sum(); // pi.sum() == 1
			F Ex = (xi * pi).sum(); // mean
			xi -= Ex; // mean 0
			xi /= std::sqrt((xi * xi * pi).sum()); // variance 1
		}
	public:
		model(std::size_t n, const F* x, const F* p)
			: xi(x, n), pi(p, n)
		{
			normalize();
		}

		std::size_t size() const { return xi.size(); }
		const std::valarray<F>& atoms() const { return xi; }

		// E[exp(s X - kappa(s)) 1(X <= x) ]
		//   = sum_{x_i <= x} exp(s x_i - kappa(s)) pi_i
		F _cdf(F x, S s) const override
		{
			F kappa = _cgf(s);
			F sum = 0;
			for (std::size_t i = 0; i < xi.size(); ++i) {
				if (xi[i] <= x)
					sum += pi[i] * std::exp(s * xi[i] - kappa);
			}
			return sum;
		}

		// kappa(s) = log E[exp(s X)] = log sum p_i exp(s x_i)
		S _cgf(S s) const override
		{
			S sum = 0;
			for (std::size_t i = 0; i < xi.size(); ++i)
				sum += pi[i] * std::exp(s * xi[i]);
			return std::log(sum);
		}
	};
} // namespace fms::option::discrete

// TODO: Create xll_option_discrete.cpp based on xll_option_normal.cpp
// TODO: Implement add-in for \OPTION.DISCRETE
// TODO: Implement add-in for OPTION.DISCRETE to return normalized xi values
// TODO: Load add-in and follow comments in final.xlsx. in xlsx folder
// TODO: Put link to your GitHub repository on Brightspace submission.
