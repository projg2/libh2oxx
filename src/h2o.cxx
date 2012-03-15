/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <stdexcept>

#include "h2o"

using namespace h2o;

H2O::H2O(double p, double T)
	: _p(p), _T(T), _region(internals::H2O_REGION_OUT_OF_RANGE)
{
}

void H2O::_determine_region()
{
	_region = internals::h2o_region_pT(_p, _T);

	if (_region == internals::H2O_REGION_OUT_OF_RANGE)
		throw std::range_error("(p,T) parameters out of range");
}

double H2O::p() const
{
	return _p;
}

double H2O::T() const
{
	return _T;
}
