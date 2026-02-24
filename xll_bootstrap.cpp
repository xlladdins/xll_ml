// xll_bootstrap.cpp - bootstrap functions
#include "xll_fi.h"
#include "fms_bootstrap.h"

using namespace xll;
using namespace fms;

static AddIn xai_curve_bootstrap(
	Function(XLL_HANDLEX, L"xll_curve_bootstrap", L"\\" CATEGORY L".CURVE.PWFLAT.BOOTSTRAP")
	.Arguments({
		Arg(XLL_FP, L"instruments", L"is a vector of handles to instruments."),
		Arg(XLL_FP, L"prices", L"is a vector of instrument prices."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a bootstrapped piecewise flat forward curve.")
);
HANDLEX WINAPI xll_curve_bootstrap(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		int n = size(*pi);
		ensure(n == size(*pp));

		curve::pwflat<> f;
		double _t = 0, _f = 0.03;

		for (int k = 0; k < n; ++k) {
			handle<instrument::base<>> ik(pi->array[k]);
			ensure(ik);
			std::tie(_t, _f) = curve::bootstrap0(*ik, f, _t, _f, pp->array[k]);
			f.push_back(_t, _f);
		}

		handle<curve::base<>> h_(new curve::pwflat<>(f));
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
