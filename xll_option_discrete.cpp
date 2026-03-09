// xll_option_discrete.cpp
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

AddIn xai_option_discrete_(
	Function(XLL_HANDLEX, L"xll_option_discrete_", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"x-values"),
		Arg(XLL_FP, L"p", L"probabilities"),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model.")
);
HANDLEX WINAPI xll_option_discrete_(const _FP12* px, const _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		int n = size(*px);
		ensure(n == size(*pp));
		handle<base<>> m_(new discrete::model<>(n, px->array, pp->array));
		ensure(m_);
		result = m_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return result;
}

AddIn xai_option_discrete(
	Function(XLL_FP, L"xll_option_discrete", CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_HANDLEX, L"m", L"is a handle to a discrete model."),
		})
		.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return normalized discrete option pricing model values.")
);
_FP12* WINAPI xll_option_discrete(HANDLEX m)
{
#pragma XLLEXPORT
	static FPX x;

	try {
		handle<base<>> m_(m);
		ensure(m_);
		const discrete::model<>* pm = m_.as<discrete::model<>>();
		ensure(pm);
		int n = static_cast<int>(pm->xi.size());
		x.resize(n, 1);
		std::copy(std::begin(pm->xi), std::end(pm->xi), begin(x));
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return x.get();
}