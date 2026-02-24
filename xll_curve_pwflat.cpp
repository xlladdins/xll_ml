// xll_curve.cpp - curve functions
#include <limits>
#include "xll_fi.h"
#include "fms_curve_pwflat.h"

using namespace xll;
using namespace fms;

static AddIn xai_curve_pwflat_(
	Function(XLL_HANDLEX, L"xll_curve_pwflat_", L"\\" CATEGORY L".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_FP, L"t", L"is the vector of forward rate times."),
		Arg(XLL_FP, L"f", L"is the vector of forward rates."),
		})
	.Uncalced()
	.Category(CATEGORY)
	.FunctionHelp(L"Return a handle to a curve with forward rates f at times t.")
);
HANDLEX WINAPI xll_curve_pwflat_(_FP12* pt, _FP12* pf)
{
#pragma XLLEXPORT
	HANDLEX h = INVALID_HANDLEX;

	try {
		handle<curve::base<>> h_(new curve::pwflat(span(*pt), span(*pf)));
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

static AddIn xai_curve_pwflat(
	Function(XLL_FP, L"xll_curve_pwflat", CATEGORY L".CURVE.PWFLAT")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a pwflat curve."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return a two row array of times and rates.")
);
_FP12* WINAPI xll_curve_pwflat(HANDLEX h)
{
#pragma XLLEXPORT
	static FPX tf;

	try {
		tf.resize(0, 0);
		handle<curve::base<>> h_(h);
		if (!h_) {
			XLL_ERROR("FI.CURVE.PWFLAT: invalid handle (ensure the curve is built in another cell first)");
			tf.resize(2, 1);
			tf[0] = tf[1] = std::numeric_limits<double>::quiet_NaN();
			return tf.get();
		}
		curve::pwflat<>* ptf = h_.as<curve::pwflat<>>();
		if (!ptf) {
			XLL_ERROR("FI.CURVE.PWFLAT: handle is not a piecewise-flat curve");
			tf.resize(2, 1);
			tf[0] = tf[1] = std::numeric_limits<double>::quiet_NaN();
			return tf.get();
		}
		int n = (int)ptf->size();
		tf.resize(2, n);
		std::copy_n(ptf->time(), n, tf.array());
		std::copy_n(ptf->rate(), n, tf.array() + n);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
		tf.resize(2, 1);
		tf[0] = tf[1] = std::numeric_limits<double>::quiet_NaN();
	}
	catch (...) {
		XLL_ERROR(__FUNCTION__ ": unknown exception");
		tf.resize(2, 1);
		tf[0] = tf[1] = std::numeric_limits<double>::quiet_NaN();
	}

	return tf.get();
}

static AddIn xai_curve_forward(
	Function(XLL_FP, L"xll_curve_forward", CATEGORY L".CURVE.FORWARD")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
		Arg(XLL_FP, L"u", L"is a time or array of times."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the continuously compounded forward rate f(u).")
);
_FP12* WINAPI xll_curve_forward(HANDLEX h, _FP12* pu)
{
#pragma XLLEXPORT
	static FPX fu;

	try {
		handle<curve::base<>> h_(h);
		ensure(h_);

		const int n = size(*pu);
		fu.resize(pu->rows, pu->columns);
		for (int i = 0; i < n; ++i) {
			fu[i] = h_->forward(pu->array[i]);
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

	return fu.get();
}

static AddIn xai_curve_discount(
	Function(XLL_FP, L"xll_curve_discount", CATEGORY L".CURVE.DISCOUNT")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
		Arg(XLL_FP, L"u", L"is a time or array of times."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the discount factor D(u).")
);
_FP12* WINAPI xll_curve_discount(HANDLEX h, _FP12* pu)
{
#pragma XLLEXPORT
	static FPX Du;

	try {
		handle<curve::base<>> h_(h);
		ensure(h_);

		const int n = size(*pu);
		Du.resize(pu->rows, pu->columns);
		for (int i = 0; i < n; ++i) {
			Du[i] = h_->discount(pu->array[i]);
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

	return Du.get();
}

static AddIn xai_curve_spot(
	Function(XLL_FP, L"xll_curve_spot", CATEGORY L".CURVE.SPOT")
	.Arguments({
		Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
		Arg(XLL_FP, L"u", L"is a time or array of times."),
		})
	.Category(CATEGORY)
	.FunctionHelp(L"Return the continuously compounded spot rate r(u).")
);
_FP12* WINAPI xll_curve_spot(HANDLEX h, _FP12* pu)
{
#pragma XLLEXPORT
	static FPX ru;

	try {
		handle<curve::base<>> h_(h);
		ensure(h_);

		const int n = size(*pu);
		ru.resize(pu->rows, pu->columns);
		for (int i = 0; i < n; ++i) {
			ru[i] = h_->spot(pu->array[i]);
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

	return ru.get();
}