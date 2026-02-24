// fms_curve_bootstrap.h - Bootstrap a piecewise-flat curve forward curve.
#pragma once
#ifdef _DEBUG
#include <cassert>
#include <cmath>
#endif
#include <utility>
#include <type_traits>
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
	inline std::pair<T, F> bootstrap0(const instrument::base<U, C>& i, const curve::base<T, F>& f,
		T _t, F _f = math::NaN<F>, C p = 0)
	{
		const auto [u_last, c_last] = i.last(); // last instrument cash flow
		if (u_last <= _t) {
			return { math::NaN<T>, math::NaN<F> };
		}

		// fix up initial guess
		if (std::isnan(_f)) {
			_f = f(_t); // last forward rate
		}
		if (std::isnan(_f)) {
			_f = 0.01;
		}

		const auto vp = [&i, &f, _t, p](F f_) { return value::present(i, extrapolate(f, _t, f_)) - p; };

		auto [f_, tol, n] = root1d::secant(_f, _f + 0.01).solve(vp);
		_f = f_;

		return { u_last, _f };
	}

	// Bootstrap a piecewise flat curve from instruments and prices.
	template<class I, class U = double, class C = double, class P = double>
	constexpr curve::pwflat<U, P> bootstrap(std::span<const I> is, std::span<const P> ps,
		U _t = 0, P _f = 0.03)
		//requires std::convertible_to<I,const instrument::base<U,C>&>
	{
		ensure(is.size() == ps.size());

		curve::pwflat<U, P> f;

		for (size_t k = 0; k < is.size(); ++k) {
			if constexpr (std::is_pointer_v<I>) {
				ensure(is[k] != nullptr);
				std::tie(_t, _f) = bootstrap0<U, C, U, P>(*is[k], f, _t, _f, ps[k]);
			}
			else {
				std::tie(_t, _f) = bootstrap0<U, C, U, P>(is[k], f, _t, _f, ps[k]);
			}

			f.push_back(_t, _f);
		}

		return f;
	}
#ifdef _DEBUG
	inline int bootstrap_test()
	{
		{
			curve::constant<> f;
			double r = 0.1;
			const double tol = math::sqrt_epsilon<double>;
			auto zcb = instrument::zero_coupon_bond(1, std::exp(r));
			auto D = f.discount(1, 0., r);
			assert(D < 1);
			auto p = value::present(zcb, extrapolate(f, 0., r));
			assert(std::fabs(p - 1) <= tol);
			auto d = value::duration(zcb, extrapolate(f, 0., r));
			assert(std::fabs(d + 1) <= tol);
			auto [_t, _f] = curve::bootstrap0(zcb, f, 0., 0.2, 1.);
			assert(_t == 1);
			assert(std::fabs(_f - r) <= tol);
		}

		return 0;
	}
#endif // _DEBUG

}