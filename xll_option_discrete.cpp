// xll_option_discrete.cpp - Discrete distribution option pricing add-in
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

AddIn xai_option_discrete(
	Function(XLL_HANDLEX, L"xll_option_discrete", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"is the array of values."),
		Arg(XLL_FP, L"p", L"is the array of probabilities."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model.")
);
HANDLEX WINAPI xll_option_discrete(_FP12* x, _FP12* p)
{
#pragma XLLEXPORT
	HANDLEX result = INVALID_HANDLEX;

	try {
		auto nx = xll::size(*x);
		auto np = xll::size(*p);
		ensure(nx == np);
		ensure(nx > 0);

		double psum = 0;
		const auto* pa = xll::array(*p);
		for (int i = 0; i < np; ++i) {
			psum += pa[i];
		}
		ensure(psum > 0);

		auto n = static_cast<std::size_t>(nx);
		handle<base<>> m_(new discrete::model<>(n, xll::array(*x), pa));
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
		Arg(XLL_HANDLEX, L"m", L"is the handle to a discrete model."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return normalized xi values from a discrete option pricing model.")
);
_FP12* WINAPI xll_option_discrete_xi(HANDLEX m)
{
#pragma XLLEXPORT
	static FPX result;

	try {
		handle<base<>> m_(m);
		ensure(m_);
		auto* dm = dynamic_cast<discrete::model<>*>(m_.ptr());
		ensure(dm);

		const auto& xi = dm->get_xi();
		auto n = static_cast<int>(xi.size());
		result.resize(n, 1);
		for (int i = 0; i < n; ++i) {
			result[i] = xi[static_cast<std::size_t>(i)];
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
