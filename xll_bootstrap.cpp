// xll_bootstrap.cpp - bootstrap functions
// TODO: Implement \FI.CURVE.PWFLAT.BOOTSTRAP(instruments, prices) 
 
//using namespace fms;
//using namespace xll;

/* TODO: Hints
handle<curve::pwflat> WINAPI xll_fi_curve_bootstrap(_FP12* pi, _FP12* pp) { ...
	ensure(size(*pi) == size(*pp));
	<something> = curve::bootstrap(std::span<handle<instrument::base<>>(pi->array, n), span(*pp));
}
*/
#include "xll_fi.h"
#include "fms_bootstrap.h"

using namespace fms;
using namespace xll;

static AddIn xai_fi_curve_bootstrap(
	Function(XLL_HANDLEX, L"xll_fi_curve_bootstrap", L"\\" CATEGORY L".CURVE.PWFLAT.BOOTSTRAP")
	.Arguments({
		Arg(XLL_FP, L"instruments", L"is s vector of instrument handles."),
		Arg(XLL_FP, L"prices", L"is s vector of instrument prices."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to boostrap curves.")
);
HANDLEX WINAPI xll_fi_curve_bootstrap(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;
	try {
		ensure(size(*pi) == size(*pp));
		curve::pwflat<> f;

		double _t = 0;
		double _f = 0.03;

		for (int i = 0; i < size(*pi); ++i) {
			handle<instrument::base<>> is(pi->array[i]);
			ensure(is);
			std::tie(_t, _f) = curve::bootstrap0(*is, f, _t, _f, pp->array[i]);
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