// fms_option_normal.h - Black model with normal distribution
#pragma once
#include <cmath>
#include <numbers>
#include "fms_math.h"
#include "fms_option.h"

namespace fms::option {

	template<class X = double, class S = double>
	struct normal : base<X, S> {
	private:
		// Standard normal cumulative distribution function
		static X cdf(X x)
		{
			//return 0.5 * (1 + math::erf_as(x / std::numbers::sqrt2));
			return 0.5 * (1 + std::erf(x / std::numbers::sqrt2));
		}
	public:
		// cumulative distribution function
		X _cdf(X x, S s) const override
		{
			return cdf(x - s);
		}
		// cumulant generating function
		S _cgf(S s) const override
		{
			return s * s /2;
		}
	};

	namespace black {
		// Vol s given put price p
		template<class F = double, class S = double, class K = double>
		inline F put_implied(F f, F p, K k, const base<F, S>& m)
		{
			auto g = [f, p, k](S s) { return put(f, s, k, normal<F>()) - p; };
			auto res = root1d::secant<>(.1, .11).solve(g);

			return get<0>(res);
		}
	}

} // namespace fms::option::black
