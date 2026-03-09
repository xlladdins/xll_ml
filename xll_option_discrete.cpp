// xll_option_discrete.cpp - Discrete option pricing model
#include <algorithm>
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

AddIn xai_option_discrete(
	Function(XLL_HANDLEX, L"xll_option_discrete", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"xi", L"is the array of support points."),
		Arg(XLL_FP, L"pi", L"is the array of probabilities."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model with normalized support.")
);
HANDLEX WINAPI xll_option_discrete(_FP12* pxi, _FP12* ppi)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		int n = size(*pxi);
		ensure(n > 0 || !"xi array cannot be empty");
		ensure(size(*ppi) == static_cast<unsigned>(n) || !"xi and pi must have same size");

		handle<base<>> m_(new discrete::model<>(n, pxi->array, ppi->array));
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

AddIn xai_option_discrete_xi(
	Function(XLL_FP, L"xll_option_discrete_xi", CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle returned by \\" CATEGORY L".DISCRETE."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return normalized xi values from discrete option model.")
);
_FP12* WINAPI xll_option_discrete_xi(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX xi_out;

	try {
		xi_out.resize(0, 0);
		handle<base<>> h_(h);
		ensure(h_);

		auto* dm = h_.as<discrete::model<>>();
		ensure(dm || !"handle must be from \\OPTION.DISCRETE");

		const auto& xi = dm->xi_values();
		int n = static_cast<int>(xi.size());
		xi_out.resize(n, 1);
		std::copy_n(&xi[0], n, xi_out.array());
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return nullptr;
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
		return nullptr;
	}

	return xi_out.get();
}