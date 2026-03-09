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
		Arg(XLL_FP, L"x", L"is an array of discrete values."),
		Arg(XLL_FP, L"p", L"is an array of probabilities."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model.")
);
HANDLEX WINAPI xll_option_discrete_(_FP12* px, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		ensure(size(*px) == size(*pp) || !"x and p arrays must be the same size");
		handle<base<>> m_(new discrete::model<>(size(*px), px->array, pp->array));
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
		Arg(XLL_HANDLEX, L"h", L"is a handle returned by \\OPTION.DISCRETE."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return normalized xi values from discrete model.")
);
_FP12* WINAPI xll_option_discrete(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX xi;

	try {
		handle<base<>> h_(h);
		ensure(h_);

		auto* m = h_.as<discrete::model<>>();
		ensure(m || !"handle is not a discrete model");

		FPX xi_((int)m->size(), 1, m->data());
		xi.swap(xi_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
		return 0;
	}

	return xi.get();
}
