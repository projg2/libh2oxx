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
#		include <h2o/region4.h>
#		include <h2o/region5.h>
#		include <h2o/saturation.h>
	};
};

using namespace h2o;
using namespace h2o::internals;

typedef double (*twoarg_func_t)(double, double);

static double not_supported(double, double)
{
	throw std::runtime_error("Requested function not implemented.");
};

H2O::H2O()
{
}

H2O::H2O(double p, double T)
	: _arg1(p), _arg2(T)
{
	_region = h2o_region_pT(p, T);

	if (_region == H2O_REGION_OUT_OF_RANGE)
		throw std::range_error("(p,T) parameters out of range");
}

H2O H2O::Tx(double T, double x)
{
	H2O ret;

	ret._arg1 = T;
	ret._arg2 = x;
	ret._region = H2O_REGION4;

	return ret;
}

double H2O::p()
{
	if (_region == H2O_REGION4)
		return h2o_saturation_p_T(_arg1);
	return _arg1;
}

double H2O::T()
{
	if (_region == H2O_REGION4)
		return _arg1;
	return _arg2;
}

static twoarg_func_t funcs_v_base[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_v_pT,
	h2o_region2_v_pT,
	not_supported,
	h2o_region4_v_Tx,
	h2o_region5_v_pT
};

double H2O::v()
{
	return funcs_v_base[_region](_arg1, _arg2);
}

static twoarg_func_t funcs_u_base[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_u_pT,
	h2o_region2_u_pT,
	not_supported,
	h2o_region4_u_Tx,
	h2o_region5_u_pT
};

double H2O::u()
{
	return funcs_u_base[_region](_arg1, _arg2);
}

static twoarg_func_t funcs_h_base[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_h_pT,
	h2o_region2_h_pT,
	not_supported,
	h2o_region4_h_Tx,
	h2o_region5_h_pT
};

double H2O::h()
{
	return funcs_h_base[_region](_arg1, _arg2);
}

static twoarg_func_t funcs_s_base[H2O_REGION_MAX] = {
	not_supported,
	h2o_region1_s_pT,
	h2o_region2_s_pT,
	not_supported,
	h2o_region4_s_Tx,
	h2o_region5_s_pT
};

double H2O::s()
{
	return funcs_s_base[_region](_arg1, _arg2);
}
