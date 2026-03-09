// xll_option_discrete.cpp
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

// Returns array where result[0] = handle, result[1..n] = normalized xi values.
// First cell can be passed to OPTION.CDF/CGF/PUT, rest can be tallied in Excel.
AddIn xai_option_discrete(
	Function(XLL_FP, L"xll_option_discrete", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"is an array of atom values."),
		Arg(XLL_FP, L"p", L"is an array of probabilities."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle and normalized xi values of discrete option pricing model.")
);
_FP12* WINAPI xll_option_discrete(_FP12* x, _FP12* p)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		int n = size(*x);
		auto* dm = new discrete::model<>(n, x->array, p->array);
		handle<base<>> m_(dm);
		ensure(m_);
		const auto& xi = dm->atoms();
		result.resize(n + 1, 1);
		result[0] = m_.get(); // handle in first cell
		for (int i = 0; i < n; ++i)
			result[i + 1] = xi[i];
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return 0;
	}
	catch (...) {
		XLL_ERROR(__FUNCDNAME__ ": unknown exception");
		return 0;
	}

	return result.get();
}
