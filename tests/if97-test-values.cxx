/* libh2o -- water & steam properties
 * (c) 2012 Michał Górny
 * Released under the terms of the 2-clause BSD license
 */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif

#include "h2o"

#include <cmath>
#include <cstdio>
#include <stdexcept>

int exit_status;

void check(double result, double expected, double precision, const char* call,
		const char* arg1, double arg1_val,
		const char* arg2, double arg2_val)
{
	double difference = std::fabs(expected - result);

	if (difference >= precision)
	{
		std::fprintf(stderr, "[FAIL] %s(%s=%.3e, %s=%.3e) = %.9e, while %.9e expected.\n",
				call, arg1, arg1_val, arg2, arg2_val,
				result, expected);
		exit_status++;
	}
	else
		std::fprintf(stderr, "[ OK ] %s(%s=%.3e, %s=%.3e) = %.9e.\n",
				call, arg1, arg1_val, arg2, arg2_val,
				result);
}

typedef double (h2o::H2O::*property_getter)() const;

const char* name_by_prop(property_getter prop)
{
	if (prop == &h2o::H2O::p)
		return "p";
	else if (prop == &h2o::H2O::T)
		return "T";
	else if (prop == &h2o::H2O::x)
		return "x";
	else if (prop == &h2o::H2O::u)
		return "u";
	else if (prop == &h2o::H2O::h)
		return "h";
	else if (prop == &h2o::H2O::s)
		return "s";
	else if (prop == &h2o::H2O::v)
		return "v";

	throw std::runtime_error("Unknown property to name_by_prop()");
}

void check_any(const h2o::H2O& obj, property_getter prop,
		double expected, double precision,
		property_getter arg1_prop, property_getter arg2_prop)
{
	check((obj.*prop)(), expected, precision, name_by_prop(prop),
			name_by_prop(arg1_prop), (obj.*arg1_prop)(),
			name_by_prop(arg2_prop), (obj.*arg2_prop)());
}

void check_vuhs(const h2o::H2O& obj,
		double v_expected, double v_precision,
		double u_expected, double u_precision,
		double h_expected, double h_precision,
		double s_expected, double s_precision,
		property_getter arg1_prop, property_getter arg2_prop)
{
	check_any(obj, &h2o::H2O::v, v_expected, v_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::u, u_expected, u_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::h, h_expected, h_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::s, s_expected, s_precision,
			arg1_prop, arg2_prop);
}

int main(void)
{
	check_vuhs(h2o::H2O(3., 300),
			0.100215168E-2, 1E-11,
			0.112324818E+3, 1E-6,
			0.115331273E+3, 1E-6,
			0.392294792E+0, 1E-9,
			&h2o::H2O::p, &h2o::H2O::T);
	check_vuhs(h2o::H2O(80, 300),
			0.971180894E-3, 1E-12,
			0.106448356E+3, 1E-6,
			0.184142828E+3, 1E-6,
			0.368563852E+0, 1E-9,
			&h2o::H2O::p, &h2o::H2O::T);
	check_vuhs(h2o::H2O(3., 500),
			0.120241800E-2, 1E-11,
			0.971934985E+3, 1E-6,
			0.975542239E+3, 1E-6,
			0.258041912E+1, 1E-8,
			&h2o::H2O::p, &h2o::H2O::T);

	check_vuhs(h2o::H2O(35E-4, 300),
			0.394913866E+2, 1E-7,
			0.241169160E+4, 1E-5,
			0.254991145E+4, 1E-5,
			0.852238967E+1, 1E-8,
			&h2o::H2O::p, &h2o::H2O::T);
	check_vuhs(h2o::H2O(35E-4, 700),
			0.923015898E+2, 1E-7,
			0.301262819E+4, 1E-5,
			0.333568375E+4, 1E-5,
			0.101749996E+2, 1E-7,
			&h2o::H2O::p, &h2o::H2O::T);
	check_vuhs(h2o::H2O(30E+0, 700),
			0.542946619E-2, 1E-11,
			0.246861076E+4, 1E-5,
			0.263149474E+4, 1E-5,
			0.517540298E+1, 1E-8,
			&h2o::H2O::p, &h2o::H2O::T);

	check_vuhs(h2o::H2O(.5, 1500),
			0.138455090E+1, 1E-8,
			0.452749310E+4, 1E-5,
			0.521976855E+4, 1E-5,
			0.965408875E+1, 1E-8,
			&h2o::H2O::p, &h2o::H2O::T);
	check_vuhs(h2o::H2O(30, 1500),
			0.230761299E-1, 1E-10,
			0.447495124E+4, 1E-5,
			0.516723514E+4, 1E-5,
			0.772970133E+1, 1E-8,
			&h2o::H2O::p, &h2o::H2O::T);
	check_vuhs(h2o::H2O(30, 2000),
			0.311385219E-1, 1E-10,
			0.563707038E+4, 1E-5,
			0.657122604E+4, 1E-5,
			0.853640523E+1, 1E-8,
			&h2o::H2O::p, &h2o::H2O::T);

	check_any(h2o::H2O::Tx(300, 1), &h2o::H2O::p, 0.353658941E-2, 1E-11,
			&h2o::H2O::T, &h2o::H2O::x);
	check_any(h2o::H2O::Tx(500, 1), &h2o::H2O::p, 0.263889776E+1, 1E-8,
			&h2o::H2O::T, &h2o::H2O::x);
	check_any(h2o::H2O::Tx(600, 1), &h2o::H2O::p, 0.123443146E+2, 1E-7,
			&h2o::H2O::T, &h2o::H2O::x);

	return exit_status;
}
