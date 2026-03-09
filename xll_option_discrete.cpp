// xll_option_discrete.cpp - Discrete distribution option pricing model add-in
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

// \OPTION.DISCRETE - construct a discrete model from arrays of x and p values
AddIn xai_option_discrete(
	Function(XLL_HANDLEX, L"xll_option_discrete", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"is the array of atom values x_i."),
		Arg(XLL_FP, L"p", L"is the array of probabilities p_i."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a discrete option pricing model with P(X = x_i) = p_i.")
);
HANDLEX WINAPI xll_option_discrete(_FP12* px, _FP12* pp)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		auto xs = span(*px);
		auto ps = span(*pp);
		ensure(xs.size() == ps.size());
		handle<base<>> m_(new discrete::model<>(xs.size(), xs.data(), ps.data()));
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

// OPTION.DISCRETE.XI - return normalized xi values from a discrete model handle
AddIn xai_option_discrete_xi(
	Function(XLL_FP, L"xll_option_discrete_xi", CATEGORY L".DISCRETE.XI")
	.Arguments({
		Arg(XLL_HANDLEX, L"m", L"is a handle to a discrete model."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the normalized atom values x_i of a discrete model.")
);
_FP12* WINAPI xll_option_discrete_xi(HANDLEX m)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		result.resize(0, 0);
		handle<base<>> m_(m);
		ensure(m_);
		auto* dm = m_.as<discrete::model<>>();
		ensure(dm);
		int n = (int)dm->size();
		result.resize(1, n);
		const auto& xi = dm->get_xi();
		for (int i = 0; i < n; ++i) {
			result[i] = xi[i];
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return result.get();
}
