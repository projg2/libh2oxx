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
#		include <h2o/region3.h>
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

static double out_of_range(double, double)
{
	throw std::range_error("Requested parameters out-of-range.");
};

H2O::H2O()
{
}

H2O::H2O(double p, double T)
	: _arg1(p), _arg2(T)
{
	_region = h2o_region_pT(p, T);

	if (_region == H2O_REGION_OUT_OF_RANGE)
		out_of_range(p, T);
}

H2O H2O::Tx(double T, double x)
{
	H2O ret;

	ret._arg1 = T;
	ret._arg2 = x;
	ret._region = H2O_REGION4;

	return ret;
}

H2O H2O::ph(double p, double h)
{
	enum h2o_region region = h2o_region_ph(p, h);

	twoarg_func_t T_getter;

	if (region == H2O_REGION4)
	{
		double T = h2o_saturation_T_p(p);
		return H2O::Tx(T, h2o_region4_x_Th(T, h));
	}

	switch (region)
	{
		case H2O_REGION1:
			T_getter = &h2o_region1_T_ph;
			break;
		case H2O_REGION2:
			T_getter = &h2o_region2_T_ph;
			break;
		case H2O_REGION_OUT_OF_RANGE:
			T_getter = &out_of_range;
			break;
		default:
			T_getter = &not_supported;
	}

	double T = T_getter(p, h);
	H2O ret;

	ret._arg1 = p;
	ret._arg2 = T;
	ret._region = region;

	return ret;
}

H2O H2O::ps(double p, double s)
{
	enum h2o_region region = h2o_region_ps(p, s);

	twoarg_func_t T_getter;

	if (region == H2O_REGION4)
	{
		double T = h2o_saturation_T_p(p);
		return H2O::Tx(T, h2o_region4_x_Ts(T, s));
	}

	switch (region)
	{
		case H2O_REGION1:
			T_getter = &h2o_region1_T_ps;
			break;
		case H2O_REGION2:
			T_getter = &h2o_region2_T_ps;
			break;
		case H2O_REGION_OUT_OF_RANGE:
			T_getter = &out_of_range;
			break;
		default:
			T_getter = &not_supported;
	}

	double T = T_getter(p, s);
	H2O ret;

	ret._arg1 = p;
	ret._arg2 = T;
	ret._region = region;

	return ret;
}

H2O H2O::rhoT(double rho, double T)
{
	enum h2o_region region = h2o_region_rhoT(rho, T);

	if (h2o_region_rhoT(rho, T) == H2O_REGION3)
	{
		H2O ret;

		ret._arg1 = rho;
		ret._arg2 = T;
		ret._region = H2O_REGION3;

		return ret;
	} else
		not_supported(rho, T);
}

Region H2O::region() const
{
	return _region;
}

double H2O::p() const
{
	switch (_region)
	{
		case H2O_REGION3:
			return h2o_region3_p_rhoT(_arg1, _arg2);
		case H2O_REGION4:
			return h2o_saturation_p_T(_arg1);
		default:
			return _arg1;
	}
}

double H2O::T() const
{
	if (_region == H2O_REGION4)
		return _arg1;
	return _arg2;
}

double H2O::x() const
{
	switch (_region)
	{
		case H2O_REGION1:
			return 0;
		case H2O_REGION2:
			return 1;
		case H2O_REGION4:
			return _arg2;
		default:
			not_supported(0, 0);
	}
}

double H2O::rho() const
{
	if (_region == H2O_REGION3)
		return _arg1;
	return 1/v();
}

static double region3_v_rhoT(double rho, double T)
{
	return 1/rho;
}

double H2O::v() const
{
	twoarg_func_t func;

	switch (_region)
	{
		case H2O_REGION1:
			func = &h2o_region1_v_pT;
			break;
		case H2O_REGION2:
			func = &h2o_region2_v_pT;
			break;
		case H2O_REGION3:
			func = &region3_v_rhoT;
			break;
		case H2O_REGION4:
			func = &h2o_region4_v_Tx;
			break;
		case H2O_REGION5:
			func = &h2o_region5_v_pT;
			break;
		default:
			func = &not_supported;
	}

	return func(_arg1, _arg2);
}

double H2O::u() const
{
	twoarg_func_t func;

	switch (_region)
	{
		case H2O_REGION1:
			func = &h2o_region1_u_pT;
			break;
		case H2O_REGION2:
			func = &h2o_region2_u_pT;
			break;
		case H2O_REGION3:
			func = &h2o_region3_u_rhoT;
			break;
		case H2O_REGION4:
			func = &h2o_region4_u_Tx;
			break;
		case H2O_REGION5:
			func = &h2o_region5_u_pT;
			break;
		default:
			func = &not_supported;
	}

	return func(_arg1, _arg2);
}

double H2O::h() const
{
	twoarg_func_t func;

	switch (_region)
	{
		case H2O_REGION1:
			func = &h2o_region1_h_pT;
			break;
		case H2O_REGION2:
			func = &h2o_region2_h_pT;
			break;
		case H2O_REGION3:
			func = &h2o_region3_h_rhoT;
			break;
		case H2O_REGION4:
			func = &h2o_region4_h_Tx;
			break;
		case H2O_REGION5:
			func = &h2o_region5_h_pT;
			break;
		default:
			func = &not_supported;
	}

	return func(_arg1, _arg2);
}

double H2O::s() const
{
	twoarg_func_t func;

	switch (_region)
	{
		case H2O_REGION1:
			func = &h2o_region1_s_pT;
			break;
		case H2O_REGION2:
			func = &h2o_region2_s_pT;
			break;
		case H2O_REGION3:
			func = &h2o_region3_s_rhoT;
			break;
		case H2O_REGION4:
			func = &h2o_region4_s_Tx;
			break;
		case H2O_REGION5:
			func = &h2o_region5_s_pT;
			break;
		default:
			func = &not_supported;
	}

	return func(_arg1, _arg2);
}
