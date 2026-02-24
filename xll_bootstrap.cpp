// xll_bootstrap.cpp - bootstrap functions
// TODO: Implement \FI.CURVE.PWFLAT.BOOTSTRAP(instruments, prices) 
 
//using namespace fms;
//using namespace xll;

/* TODO: Hints
handle<curve::pwflat> WINAPI xll_fi_curve_bootstrap(_FP12* pi, _FP12* pp) { ...
	ensure(size(*pi) == size(*pp));
	<something> = curve::bootstrap(std::span<handle<instrument::base<>>(pi->array, n), span(*pp));
}
*/
// xll_bootstrap.cpp - bootstrap functions
#include "xll_fi.h"
#include "fms_bootstrap.h"
#include "fms_instrument.h"
#include "fms_curve_pwflat.h"
#include <vector>
#include <span>
#include <limits>

using namespace xll;
using namespace fms;


// \FI.CURVE.PWFLAT.BOOTSTRAP(instruments, prices)
//
// instruments: a range of instrument handles (HANDLEX stored in cells)
// prices:      a range of prices corresponding 1-1 with instruments
static AddIn xai_curve_pwflat_bootstrap_(
    Function(XLL_HANDLEX, L"xll_curve_pwflat_bootstrap_", L"\\" CATEGORY L".CURVE.PWFLAT.BOOTSTRAP")
    .Arguments({
        Arg(XLL_FP, L"instruments", L"is a vector/range of instrument handles."),
        Arg(XLL_FP, L"prices", L"is a vector/range of prices."),
        })
        .Uncalced()
    .Category(CATEGORY)
    .FunctionHelp(L"Return a handle to a piecewise-flat forward curve bootstrapped from instruments and prices.")
);
HANDLEX WINAPI xll_curve_pwflat_bootstrap_(_FP12* pi, _FP12* pp)
{
#pragma XLLEXPORT
    HANDLEX h = INVALID_HANDLEX;

    try {
        ensure(pi && pp);

        auto si = span(*pi);
        auto sp0 = span(*pp);

        ensure(si.size() == sp0.size());
        const int n = static_cast<int>(si.size());

        std::vector<const instrument::base<>*> pinstr;
        pinstr.reserve(n);

        for (int k = 0; k < n; ++k) {
            HANDLEX hi = static_cast<HANDLEX>(si.data()[k]);

            handle<instrument::base<>> hk(hi);
            ensure(hk);

            const instrument::base<>* p = hk.operator->();
            ensure(p);

            pinstr.push_back(p);
        }

        auto spi = std::span<const instrument::base<>*const>(pinstr.data(), pinstr.size());
        auto spp = std::span<const double>(sp0.data(), sp0.size());

        auto c = curve::bootstrap(
            spi,
            spp,
            std::numeric_limits<double>::infinity(),
            std::numeric_limits<double>::quiet_NaN()
        );

        handle<curve::base<>> hc(new curve::pwflat<>(c));
        ensure(hc);

        h = hc.get();
    }
    catch (const std::exception& ex) {
        XLL_ERROR(ex.what());
    }
    catch (...) {
        XLL_ERROR(__FUNCTION__ ": unknown exception");
    }

    return h;
}