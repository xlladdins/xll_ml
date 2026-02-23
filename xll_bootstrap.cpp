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

		std::vector<instrument::instrument<>> is;
		is.reserve(n);
		for (int i = 0; i < n; ++i) {
			handle<instrument::instrument<>> ik(pi->array[i]);
			ensure(ik);
			is.push_back(*ik);
		}

		std::span<const double> ps(pp->array, n);
		std::span<const instrument::instrument<>> is_span(is.data(), is.size());

		auto f = curve::bootstrap(is_span, ps);

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