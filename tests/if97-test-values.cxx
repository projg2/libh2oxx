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
	else if (prop == &h2o::H2O::rho)
		return "rho";

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

void check_puhs(const h2o::H2O& obj,
		double p_expected, double p_precision,
		double u_expected, double u_precision,
		double h_expected, double h_precision,
		double s_expected, double s_precision,
		property_getter arg1_prop, property_getter arg2_prop)
{
	check_any(obj, &h2o::H2O::p, p_expected, p_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::u, u_expected, u_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::h, h_expected, h_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::s, s_expected, s_precision,
			arg1_prop, arg2_prop);
}

void check_Tv(const h2o::H2O& obj,
		double T_expected, double T_precision,
		double v_expected, double v_precision,
		property_getter arg1_prop, property_getter arg2_prop)
{
	check_any(obj, &h2o::H2O::T, T_expected, T_precision,
			arg1_prop, arg2_prop);
	check_any(obj, &h2o::H2O::v, v_expected, v_precision,
			arg1_prop, arg2_prop);
}

int main(void)
{
	// Region 1
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

	// Region 2
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

	// Region 3
	check_puhs(h2o::H2O::rhoT(500, 650),
			0.255837018E2, 1E-7,
			0.181226279E4, 1E-5,
			0.186343019E4, 1E-5,
			0.405427273E1, 1E-8,
			&h2o::H2O::rho, &h2o::H2O::T);
	check_puhs(h2o::H2O::rhoT(200, 650),
			0.222930643E2, 1E-7,
			0.226365868E4, 1E-5,
			0.237512401E4, 1E-5,
			0.485438792E1, 1E-8,
			&h2o::H2O::rho, &h2o::H2O::T);
	check_puhs(h2o::H2O::rhoT(500, 750),
			0.783095639E2, 1E-7,
			0.210206932E4, 1E-5,
			0.225868845E4, 1E-5,
			0.446971906E1, 1E-8,
			&h2o::H2O::rho, &h2o::H2O::T);

	// Region 5
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

	// saturation line
	check_any(h2o::H2O::Tx(300, 1), &h2o::H2O::p,
			0.353658941E-2, 1E-11,
			&h2o::H2O::T, &h2o::H2O::x);
	check_any(h2o::H2O::Tx(500, 1), &h2o::H2O::p,
			0.263889776E+1, 1E-8,
			&h2o::H2O::T, &h2o::H2O::x);
	check_any(h2o::H2O::Tx(600, 1), &h2o::H2O::p,
			0.123443146E+2, 1E-7,
			&h2o::H2O::T, &h2o::H2O::x);

	// Region 1, f(p, h)
	check_any(h2o::H2O::ph(3., 500.), &h2o::H2O::T,
			0.391798509E+3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(80, 500.), &h2o::H2O::T,
			0.378108626E+3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(80, 1500), &h2o::H2O::T,
			0.611041229E+3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);

	// Region 2, f(p, h)
	check_any(h2o::H2O::ph(1E-3, 3000), &h2o::H2O::T,
			0.534433241E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(3.00, 3000), &h2o::H2O::T,
			0.575373370E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(3.00, 4000), &h2o::H2O::T,
			0.101077577E4, 1E-5,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(5.00, 3500), &h2o::H2O::T,
			0.801299102E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(5.00, 4000), &h2o::H2O::T,
			0.101531583E4, 1E-5,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(25.0, 3500), &h2o::H2O::T,
			0.875279054E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(40.0, 2700), &h2o::H2O::T,
			0.743056411E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(60.0, 2700), &h2o::H2O::T,
			0.791137067E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ph(60.0, 3200), &h2o::H2O::T,
			0.882756860E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);

	// Region 1, f(p, s)
	check_any(h2o::H2O::ps(3., 0.5), &h2o::H2O::T,
			0.307842258E+3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(80, 0.5), &h2o::H2O::T,
			0.309979785E+3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(80, 3.0), &h2o::H2O::T,
			0.565899909E+3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);

	// Region 2, f(p, s)
	check_any(h2o::H2O::ps(0.1, 7.50), &h2o::H2O::T,
			0.399517097E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(0.1, 8.00), &h2o::H2O::T,
			0.514127081E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(2.5, 8.00), &h2o::H2O::T,
			0.103984917E4, 1E-5,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(8.0, 6.00), &h2o::H2O::T,
			0.600484040E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(8.0, 7.50), &h2o::H2O::T,
			0.106495556E4, 1E-5,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(90., 6.00), &h2o::H2O::T,
			0.103801126E4, 1E-5,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(20., 5.75), &h2o::H2O::T,
			0.697992849E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(80., 5.25), &h2o::H2O::T,
			0.854011484E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);
	check_any(h2o::H2O::ps(80., 5.75), &h2o::H2O::T,
			0.949017998E3, 1E-6,
			&h2o::H2O::p, &h2o::H2O::s);

	// Region 1, f(h, s)
	check_any(h2o::H2O::hs(1E-3, 0.0), &h2o::H2O::p,
			0.9800980612E-3, 1E-12,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(90E0, 0.0), &h2o::H2O::p,
			0.9192954727E+2, 1E-7,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(15E2, 3.4), &h2o::H2O::p,
			0.5868294423E+2, 1E-7,
			&h2o::H2O::h, &h2o::H2O::s);

	// Region 2, f(h, s)
	check_any(h2o::H2O::hs(2800, 6.5), &h2o::H2O::p,
			0.1371012767E+1, 1E-9,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2800, 9.5), &h2o::H2O::p,
			0.1879743844E-2, 1E-12,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(4100, 9.5), &h2o::H2O::p,
			0.1024788997E+0, 1E-10,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2800, 6.0), &h2o::H2O::p,
			0.4793911442E+1, 1E-9,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(3600, 6.0), &h2o::H2O::p,
			0.8395519209E+2, 1E-8,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(3600, 7.0), &h2o::H2O::p,
			0.7527161441E+1, 1E-9,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2800, 5.1), &h2o::H2O::p,
			0.9439202060E+2, 1E-8,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2800, 5.8), &h2o::H2O::p,
			0.8414574124E+1, 1E-9,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(3400, 5.8), &h2o::H2O::p,
			0.8376903879E+2, 1E-8,
			&h2o::H2O::h, &h2o::H2O::s);

	// Region 3, f(p, h)
	check_Tv(h2o::H2O::ph(20., 1700),
			0.6293083892E+3, 1E-7,
			0.1749903962E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::h);
	check_Tv(h2o::H2O::ph(50., 2000),
			0.6905718338E+3, 1E-7,
			0.1908139035E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::h);
	check_Tv(h2o::H2O::ph(100, 2100),
			0.7336163014E+3, 1E-7,
			0.1676229776E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::h);
	check_Tv(h2o::H2O::ph(20., 2500),
			0.6418418053E+3, 1E-7,
			0.6670547043E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::h);
	check_Tv(h2o::H2O::ph(50., 2400),
			0.7351848618E+3, 1E-7,
			0.2801244590E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::h);
	check_Tv(h2o::H2O::ph(100, 2700),
			0.8420460876E+3, 1E-7,
			0.2404234998E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::h);

	// Region 3, f(p, s)
	check_Tv(h2o::H2O::ps(20., 3.8),
			0.6282959869E+3, 1E-7,
			0.1733791463E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::s);
	check_Tv(h2o::H2O::ps(50., 3.6),
			0.6297158726E+3, 1E-7,
			0.1469680170E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::s);
	check_Tv(h2o::H2O::ps(100, 4.0),
			0.7056880237E+3, 1E-7,
			0.1555893131E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::s);
	check_Tv(h2o::H2O::ps(20., 5.0),
			0.6401176443E+3, 1E-7,
			0.6262101987E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::s);
	check_Tv(h2o::H2O::ps(50., 4.5),
			0.7163687517E+3, 1E-7,
			0.2332634294E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::s);
	check_Tv(h2o::H2O::ps(100, 5.0),
			0.8474332825E+3, 1E-7,
			0.2449610757E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::s);

	// Region 3, f(h, s)
	// (h,s)->(p,s)->(v,T)->p -- we've got to lose precision
	check_any(h2o::H2O::hs(1700, 3.8), &h2o::H2O::p,
			0.2555703246E+2, 1E-2,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2000, 4.2), &h2o::H2O::p,
			0.4540873468E+2, 5E-3,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2100, 4.3), &h2o::H2O::p,
			0.6078123340E+2, 5E-2,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2600, 5.1), &h2o::H2O::p,
			0.3434999263E+2, 1E-3,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2400, 4.7), &h2o::H2O::p,
			0.6363924887E+2, 5E-3,
			&h2o::H2O::h, &h2o::H2O::s);
	check_any(h2o::H2O::hs(2700, 5.0), &h2o::H2O::p,
			0.8839043281E+2, 1E-3,
			&h2o::H2O::h, &h2o::H2O::s);

	// Region 3, f(p, T)
	check_any(h2o::H2O(50.000, 630.00), &h2o::H2O::v,
			0.1470853100E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(80.000, 670.00), &h2o::H2O::v,
			0.1503831359E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(50.000, 710.00), &h2o::H2O::v,
			0.2204728587E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(80.000, 750.00), &h2o::H2O::v,
			0.1973692940E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(20.000, 630.00), &h2o::H2O::v,
			0.1761696406E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(30.000, 650.00), &h2o::H2O::v,
			0.1819560617E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(26.000, 656.00), &h2o::H2O::v,
			0.2245587720E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(30.000, 670.00), &h2o::H2O::v,
			0.2506897702E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(26.000, 661.00), &h2o::H2O::v,
			0.2970225962E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(30.000, 675.00), &h2o::H2O::v,
			0.3004627086E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(26.000, 671.00), &h2o::H2O::v,
			0.5019029401E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(30.000, 690.00), &h2o::H2O::v,
			0.4656470142E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(23.600, 649.00), &h2o::H2O::v,
			0.2163198378E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(24.000, 650.00), &h2o::H2O::v,
			0.2166044161E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(23.600, 652.00), &h2o::H2O::v,
			0.2651081407E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(24.000, 654.00), &h2o::H2O::v,
			0.2967802335E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(23.600, 653.00), &h2o::H2O::v,
			0.3273916816E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(24.000, 655.00), &h2o::H2O::v,
			0.3550329864E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(23.500, 655.00), &h2o::H2O::v,
			0.4545001142E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(24.000, 660.00), &h2o::H2O::v,
			0.5100267704E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(23.000, 660.00), &h2o::H2O::v,
			0.6109525997E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(24.000, 670.00), &h2o::H2O::v,
			0.6427325645E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.600, 646.00), &h2o::H2O::v,
			0.2117860851E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(23.000, 646.00), &h2o::H2O::v,
			0.2062374674E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.600, 648.60), &h2o::H2O::v,
			0.2533063780E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.800, 649.30), &h2o::H2O::v,
			0.2572971781E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.600, 649.00), &h2o::H2O::v,
			0.2923432711E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.800, 649.70), &h2o::H2O::v,
			0.2913311494E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.600, 649.10), &h2o::H2O::v,
			0.3131208996E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.800, 649.90), &h2o::H2O::v,
			0.3221160278E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.600, 649.40), &h2o::H2O::v,
			0.3715596186E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(22.800, 650.20), &h2o::H2O::v,
			0.3664754790E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(21.100, 640.00), &h2o::H2O::v,
			0.1970999272E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(21.800, 643.00), &h2o::H2O::v,
			0.2043919161E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(21.100, 644.00), &h2o::H2O::v,
			0.5251009921E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(21.800, 648.00), &h2o::H2O::v,
			0.5256844741E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(19.100, 635.00), &h2o::H2O::v,
			0.1932829079E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(20.000, 638.00), &h2o::H2O::v,
			0.1985387227E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(17.000, 626.00), &h2o::H2O::v,
			0.8483262001E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);
	check_any(h2o::H2O(20.000, 640.00), &h2o::H2O::v,
			0.6227528101E-2, 1E-12,
			&h2o::H2O::p, &h2o::H2O::T);

	return exit_status;
}
