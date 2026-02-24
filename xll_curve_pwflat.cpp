// xll_curve.cpp - curve functions
#include "xll_fi.h"
#include "fms_curve_pwflat.h"

using namespace xll;
using namespace fms;

static AddIn xai_curve_pwflat_(
    Function(XLL_HANDLEX, L"xll_curve_pwflat_", CATEGORY L".CURVE.PWFLAT")
    .Arguments({
        Arg(XLL_FP, L"t", L"is the vector of forward rate times."),
        Arg(XLL_FP, L"f", L"is the vector of forward rates."),
        })
        .Uncalced()
    .Category(CATEGORY)
    .FunctionHelp(L"Return a handle to a curve with forward rates f at times t.")
);

extern "C" HANDLEX WINAPI xll_curve_pwflat_(_FP12* pt, _FP12* pf)
{
#pragma XLLEXPORT
    HANDLEX h = INVALID_HANDLEX;

    try {
        handle<curve::base<>> h_(new curve::pwflat<>(span(*pt), span(*pf)));
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

extern "C" _FP12* WINAPI xll_curve_pwflat(HANDLEX h)
{
#pragma XLLEXPORT
    static FPX tf;

    try {
        tf.resize(0, 0);
        handle<curve::base<>> h_(h);
        ensure(h_);
        curve::pwflat<>* ptf = h_.as<curve::pwflat<>>();
        int n = (int)ptf->size();
        tf.resize(2, n);
        std::copy_n(ptf->time(), n, tf.array());
        std::copy_n(ptf->rate(), n, tf.array() + n);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }
    catch (...) {
        XLL_ERROR(__FUNCTION__ ": unknown exception");
    }

    return tf.get();
}

// Forward rate at time u.
static AddIn xai_curve_forward(
    Function(XLL_DOUBLE, L"xll_curve_forward", CATEGORY L".CURVE.FORWARD")
    .Arguments({
        Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
        Arg(XLL_DOUBLE, L"u", L"is the time."),
        })
        .Category(CATEGORY)
    .FunctionHelp(L"Return the forward rate at time u.")
);

extern "C" double WINAPI xll_curve_forward(HANDLEX h, double u)
{
#pragma XLLEXPORT
    double f = std::numeric_limits<double>::quiet_NaN();

    try {
        handle<curve::base<>> h_(h);
        ensure(h_);
        f = h_->forward(u);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }
    catch (...) {
        XLL_ERROR(__FUNCTION__ ": unknown exception");
    }

    return f;
}

// Discount factor at time u.
static AddIn xai_curve_discount(
    Function(XLL_DOUBLE, L"xll_curve_discount", CATEGORY L".CURVE.DISCOUNT")
    .Arguments({
        Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
        Arg(XLL_DOUBLE, L"u", L"is the time."),
        })
        .Category(CATEGORY)
    .FunctionHelp(L"Return the discount factor at time u.")
);

extern "C" double WINAPI xll_curve_discount(HANDLEX h, double u)
{
#pragma XLLEXPORT
    double d = std::numeric_limits<double>::quiet_NaN();

    try {
        handle<curve::base<>> h_(h);
        ensure(h_);
        d = h_->discount(u);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }
    catch (...) {
        XLL_ERROR(__FUNCTION__ ": unknown exception");
    }

    return d;
}

// Spot/yield at time u.
static AddIn xai_curve_spot(
    Function(XLL_DOUBLE, L"xll_curve_spot", CATEGORY L".CURVE.SPOT")
    .Arguments({
        Arg(XLL_HANDLEX, L"h", L"is a handle to a curve."),
        Arg(XLL_DOUBLE, L"u", L"is the time."),
        })
        .Category(CATEGORY)
    .FunctionHelp(L"Return the spot/yield at time u.")
);

extern "C" double WINAPI xll_curve_spot(HANDLEX h, double u)
{
#pragma XLLEXPORT
    double s = std::numeric_limits<double>::quiet_NaN();

    try {
        handle<curve::base<>> h_(h);
        ensure(h_);
        s = h_->spot(u);
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }
    catch (...) {
        XLL_ERROR(__FUNCTION__ ": unknown exception");
    }

    return s;
}