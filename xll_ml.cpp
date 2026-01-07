// xll_ml.cpp
#include "fms_perceptron.h"
#include "xll24/include/xll.h"

using namespace xll;

AddIn xai_perceptron_update(
	Function(XLL_FP, L"xll_perceptron_update", L"PERCEPTRON.UPDATE")
	.Arguments({
		Arg(XLL_FP, L"w", L"is an array of weights"),
		Arg(XLL_DOUBLE, L"b", L"is the bias term"),
		Arg(XLL_FP, L"x", L"is an array representing the input vector"),
		Arg(XLL_INT, L"y", L"is the label (-1 or 1)"),
		Arg(XLL_DOUBLE, L"alpha", L"is the learning rate (default=1.0)", 1.0)
		})
	.FunctionHelp(L"Update perceptron weights and bias given input vector and label.")
	.Category(L"ML")
);
_FP12* WINAPI xll_perceptron_update(_FP12* pw, double b, const _FP12* const px, int y, double alpha)
{
#pragma XLLEXPORT
	try {
		auto w = std::mdspan(pw->array, size(*pw));
		auto x = std::mdspan(px->array, size(*px));
		alpha = alpha ? alpha : 1;

		fms::perceptron::update(w, b, x, y, alpha);
	}
	catch (const std::exception& ex) {
		XLL_ERROR(ex.what());
	}
	catch (...) {
		XLL_ERROR("xll_perceptron_update: unknown exception");
	}
	return pw;
}