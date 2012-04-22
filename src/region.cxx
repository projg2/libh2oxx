/* libh2o -- steam & water properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include <cassert>

#include "h2o"

using namespace h2o;
using namespace h2o::internals;

static const bool not_reached = false; // for assert()

Region::Region(enum_type val)
	: _val(val)
{
	assert(val >= OOR && val <= R5);
}

Region::Region(enum h2o_region val)
{
	Region ret;

	switch (val)
	{
		case H2O_REGION_OUT_OF_RANGE:
			ret = Region::OOR;
			break;
		case H2O_REGION1:
			ret = Region::R1;
			break;
		case H2O_REGION2:
			ret = Region::R2;
			break;
		case H2O_REGION3:
			ret = Region::R3;
			break;
		case H2O_REGION4:
			ret = Region::R4;
			break;
		case H2O_REGION5:
			ret = Region::R5;
			break;
		case H2O_REGION_MAX:
			assert(not_reached);
	}

	_val = ret;
}

Region::operator enum_type() const
{
	return _val;
}
