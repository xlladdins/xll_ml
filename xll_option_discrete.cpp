// xll_option_discrete.cpp - Discrete distribution option pricing add-in
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

// \OPTION.DISCRETE(x, p) - Constructor: returns handle to discrete model
AddIn xai_option_discrete_(
	Function(XLL_HANDLEX, L"xll_option_discrete_", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"is the array of outcome values."),
		Arg(XLL_FP, L"p", L"is the array of probabilities."),
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

// OPTION.DISCRETE(m) - Accessor: returns normalized xi values from discrete model
AddIn xai_option_discrete(
	Function(XLL_FP, L"xll_option_discrete", CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_HANDLEX, L"m", L"is the handle returned by \\OPTION.DISCRETE."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return normalized xi values from discrete model.")
);
_FP12* WINAPI xll_option_discrete(HANDLEX m)
{
#pragma XLLEXPORT
	static FPX xi;

	try {
		handle<discrete::model<>> m_(m);
		ensure(m_);
		const auto& v = m_->get_xi();
		FPX xi_((int)v.size(), 1);
		for (int i = 0; i < (int)v.size(); ++i) {
			xi_.array()[i] = v[i];
		}
		xi.swap(xi_);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return nullptr;
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
		return nullptr;
	}

	return xi.get();
}
