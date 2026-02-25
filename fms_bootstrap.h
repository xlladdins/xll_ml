// fms_curve_bootstrap.h - Bootstrap a piecewise-flat curve forward curve.
#pragma once

#ifdef _DEBUG
#include <cassert>
#endif

#include <utility>
#include <span>
#include <cmath>

#include "fms_instrument.h"
#include "fms_curve_pwflat.h"
#include "fms_valuation.h"
#include "fms_math.h"

namespace fms::curve {

	// bootstrap1 - cash deposit
	// bootstrap2 - forward rate agreement

	// Bootstrap a single instrument given last time on curve and optional initial forward rate guess.
	// Return point on the curve repricing the instrument.
	template<class U, class C, class T = double, class F = double>
	inline std::pair<T, F> bootstrap0(instrument::base<U, C> i,
		const curve::base<T, F>& f,
		T _t,
		F _f = math::NaN<F>,
		F p = 0)
	{
		const auto uc = i.last(); // last instrument cash flow
		if (uc.u <= _t) {
			return { math::NaN<T>, math::NaN<F> };
		}

		// reasonable initial guess if none provided
		if (std::isnan(_f)) {
			_f = f(_t);
		}
		if (std::isnan(_f)) {
			_f = static_cast<F>(0.01);
		}

		// root find: PV(i) - price = 0, with curve extrapolated flat beyond _t at f_
		const auto pv_gap = [i, &f, _t, p](F f_) {
			return value::present(i, extrapolate(f, _t, f_)) - p;
		};

		auto [sol, tol, iters] = root1d::secant(_f, _f + static_cast<F>(0.01)).solve(pv_gap);
		_f = sol;

		return { uc.u, _f };
	}

	// Bootstrap a piecewise flat curve from instruments and prices.
	template<class I, class U = double, class C = double, class P = double>
	constexpr curve::pwflat<U, P> bootstrap(std::span<const I> is,
		std::span<const P> ps,
		double _t = 0,
		double _f = 0.03)
		//requires std::convertible_to<I,const instrument::base<U,C>&>
	{
		curve::pwflat<U, P> f;

#ifdef _DEBUG
		assert(is.size() == ps.size());
#endif
		const size_t n = (is.size() < ps.size()) ? is.size() : ps.size();

		for (size_t k = 0; k < n; ++k) {
			auto tf = bootstrap0(is[k], f, _t, _f, ps[k]);
			_t = tf.first;
			_f = tf.second;

			// only append valid points
			if (!std::isnan(_t) && !std::isnan(_f)) {
				f.push_back(_t, _f);
			}
		}

		return f;
	}

#ifdef _DEBUG
	inline int bootstrap_test()
	{
		{
			curve::constant<> f;
			const double r = 0.1;

			auto zcb = instrument::zero_coupon_bond(1, std::exp(r));

			// sanity: discount factor should be < 1 for positive rate
			const auto D = f.discount(1, 0., r);
			assert(D < 1);

			// PV at the "right" flat extrapolated rate should reprice to 1
			const auto pv = value::present(zcb, extrapolate(f, 0., r));
			assert(pv == 1);

			const auto dur = value::duration(zcb, extrapolate(f, 0., r));
			assert(dur == -1);

			// bootstrap should recover approximately r
			auto [t1, f1] = curve::bootstrap0(zcb, f, 0., 0.2, 1.);
			assert(t1 == 0);
			assert(std::fabs(f1 - r) <= math::sqrt_epsilon<double>);
		}

		return 0;
	}
#endif // _DEBUG

} // namespace fms::curve
