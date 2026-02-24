// xll_bootstrap.cpp - bootstrap functions
#include <cmath>
#include <stdexcept>
#include <string>
#include "xll_fi.h"
#include "fms_bootstrap.h"

using namespace xll;
using namespace fms;

static AddIn xai_curve_pwflat_bootstrap_(
	Function(XLL_HANDLEX, L"xll_curve_pwflat_bootstrap_", L"\\" CATEGORY L".CURVE.PWFLAT.BOOTSTRAP")
	.Arguments({
		Arg(XLL_FP, L"instruments", L"is a vector of handles to instruments."),
		Arg(XLL_FP, L"prices", L"is a vector of prices corresponding to instruments."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a bootstrapped piecewise-flat forward curve repricing instruments.")
);
HANDLEX WINAPI xll_curve_pwflat_bootstrap_(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		const int n = size(*pi);
		ensure(n == size(*pp) || !"instruments and prices size mismatch");

		curve::pwflat<> f;
		double t = 0;
		double fr = math::NaN<double>;

		for (int k = 0; k < n; ++k) {
			try {
				const double price = pp->array[k];
				if (std::isnan(price)) {
					throw std::runtime_error("price is NaN");
				}

				handle<instrument::base<>> ik(pi->array[k]);
				if (!ik) {
					throw std::runtime_error("invalid instrument handle (check the instruments range contains handles)");
				}

				std::tie(t, fr) = curve::bootstrap0<double, double, double, double>(*ik, f, t, fr, price);
				if (std::isnan(t) || std::isnan(fr)) {
					throw std::runtime_error("bootstrap returned NaN (check instrument maturities are strictly increasing)");
				}

				f.push_back(t, fr);
			}
			catch (const std::exception& ex) {
				throw std::runtime_error(std::string("FI.CURVE.PWFLAT.BOOTSTRAP: instrument #")
					+ std::to_string(k + 1) + ": " + ex.what());
			}
		}

		handle<curve::base<>> h_(new curve::pwflat<>(std::move(f)));
		ensure(h_);
		h = h_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return h;
}
