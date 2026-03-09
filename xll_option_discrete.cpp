// xll_option_discrete.cpp
#include "fms_option_discrete.h"
#include "xll_ml.h"

#undef CATEGORY
#define CATEGORY L"OPTION"

using namespace xll;
using namespace fms::option;

AddIn xai_option_discrete1(
	Function(XLL_HANDLEX, L"xll_option_discrete1", L"\\" CATEGORY L".DISCRETE")
	.Arguments({
		Arg(XLL_FP, L"x", L"is the vector of discrete numbers."),
		Arg(XLL_FP, L"p", L"is the vector of possibilities."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return handle to discrete option pricing model.")
);
HANDLEX WINAPI xll_option_discrete1(_FP12* x, _FP12* p)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		std::size_t n = size(*x);
		ensure(n > 0);
		ensure(size(*p) == n);
		handle<base<>> m_(new discrete<>{n, x->array, p->array});
		ensure(m_);
		h = m_.get();
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
	}

	return h;
}

AddIn xai_option_discrete2(
    Function(XLL_FP, L"xll_option_discrete2", CATEGORY L".DISCRETE2")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a discrete model."),
	})
	.Category(CATEGORY)
    .FunctionHelp(L"Return the normalized xi vector from a discrete option pricing model.")
);
_FP12* WINAPI xll_option_discrete2(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX uc;

	try {
		uc.resize(0, 0);
		handle<base<>> h_(h);
		ensure(h_);

		discrete<>* d = h_.as<discrete<>>();
		ensure(d);
		int n = static_cast<int>(d->size());
		uc.resize(1, n);
		const auto& normalized_x = d->x();
		for (int i = 0; i < n; ++i) {
		    uc[i] = normalized_x[i];
		}
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		return nullptr;
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
		return nullptr;
	}

	return uc.get();
}
