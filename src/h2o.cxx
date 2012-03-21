/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <stdexcept>

#include "h2o"

namespace h2o
{
	namespace internals
	{
#		include <h2o/region1.h>
#		include <h2o/region2.h>
#		include <h2o/region5.h>
	};
};

using namespace h2o;
using namespace h2o::internals;

typedef double (*twoarg_func_t)(double, double);

static double not_supported(double, double)
{
	throw std::runtime_error("Requested function not implemented.");
};

H2O::H2O(double p, double T)
	: _p(p), _T(T), _region(H2O_REGION_OUT_OF_RANGE)
{
}

void H2O::_determine_region()
{
	_region = h2o_region_pT(_p, _T);

	if (_region == H2O_REGION_OUT_OF_RANGE)
		throw std::range_error("(p,T) parameters out of range");
}

double H2O::p()
{
	return _p;
}

double H2O::T()
{
	return _T;
}

static twoarg_func_t funcs_v_pT[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_v_pT,
	h2o_region2_v_pT,
	not_supported,
	not_supported,
	h2o_region5_v_pT
};

double H2O::v()
{
	if (!_region)
		_determine_region();

	return funcs_v_pT[_region](_p, _T);
}

static twoarg_func_t funcs_u_pT[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_u_pT,
	h2o_region2_u_pT,
	not_supported,
	not_supported,
	h2o_region5_u_pT
};

double H2O::u()
{
	if (!_region)
		_determine_region();

	return funcs_u_pT[_region](_p, _T);
}

static twoarg_func_t funcs_h_pT[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_h_pT,
	h2o_region2_h_pT,
	not_supported,
	not_supported,
	h2o_region5_h_pT
};

double H2O::h()
{
	if (!_region)
		_determine_region();

	return funcs_h_pT[_region](_p, _T);
}

static twoarg_func_t funcs_s_pT[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_s_pT,
	h2o_region2_s_pT,
	not_supported,
	not_supported,
	h2o_region5_s_pT
};

double H2O::s()
{
	if (!_region)
		_determine_region();

	return funcs_s_pT[_region](_p, _T);
}
