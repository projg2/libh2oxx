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
	}
}

using namespace h2o;
using namespace h2o::internals;

typedef double (*twoarg_func_t)(double, double);

static inline void uninitialized()
{
	throw std::runtime_error("Requesting parameters from uninitialized H2O.");
}

static inline void out_of_range()
{
	throw std::range_error("Requested parameters out-of-range.");
}

H2O::H2O(internals::h2o_t data)
	: _data(data)
{
	if (_data.region == internals::H2O_REGION_OUT_OF_RANGE)
		out_of_range();
}

H2O::H2O()
{
	_data.region = internals::H2O_REGION_OUT_OF_RANGE;
}

H2O::H2O(double p, double T)
	: _data(internals::h2o_new_pT(p, T))
{
	if (_data.region == internals::H2O_REGION_OUT_OF_RANGE)
		out_of_range();
}

H2O H2O::pT(double p, double T)
{
	return H2O(p, T);
}

H2O H2O::Tx(double T, double x)
{
	return H2O(internals::h2o_new_Tx(T, x));
}

H2O H2O::px(double p, double x)
{
	return H2O(internals::h2o_new_px(p, x));
}

H2O H2O::ph(double p, double h)
{
	return H2O(internals::h2o_new_ph(p, h));
}

H2O H2O::ps(double p, double s)
{
	return H2O(internals::h2o_new_ps(p, s));
}

H2O H2O::hs(double h, double s)
{
	return H2O(internals::h2o_new_hs(h, s));
}

H2O H2O::rhoT(double rho, double T)
{
	return H2O(internals::h2o_new_rhoT(rho, T));
}

bool H2O::initialized() const
{
	return _data.region != H2O_REGION_OUT_OF_RANGE;
}

Region H2O::region() const
{
	return _data.region;
}

double H2O::p() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_p(_data);
}

double H2O::T() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_T(_data);
}

double H2O::x() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();
	if (_data.region == H2O_REGION3)
		throw std::runtime_error("x undefined in Region 3.");

	return h2o_get_x(_data);
}

double H2O::rho() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_rho(_data);
}

double H2O::v() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_v(_data);
}

double H2O::u() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_u(_data);
}

double H2O::h() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_h(_data);
}

double H2O::s() const
{
	if (_data.region == H2O_REGION_OUT_OF_RANGE)
		uninitialized();

	return h2o_get_s(_data);
}

H2O H2O::expand(double pout) const
{
	if (_data.region == H2O_REGION5)
		throw std::range_error("Expansion not supported in region 5");

	return H2O::ps(pout, s());
}

H2O H2O::expand(double pout, double eta) const
{
	H2O ideal = expand(pout);

	double hin = h();
	double hout = ideal.h();
	double houtr = hin - (hin - hout) * eta;

	return H2O::ph(pout, houtr);
}
