/* vim: set sw=4 sts=4 et foldmethod=syntax : */

/*
 * Copyright (c) 2014 Danny van Dyk
 *
 * This file is part of the EOS project. EOS is free software;
 * you can redistribute it and/or modify it under the terms of the GNU General
 * Public License version 2, as published by the Free Software Foundation.
 *
 * EOS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <test/test.hh>
#include <eos/form-factors/analytic-b-to-pi.hh>
#include <eos/form-factors/mesonic.hh>

#include <cmath>
#include <limits>
#include <vector>

#include <iostream>
#include <array>

using namespace test;
using namespace eos;

class AnalyticFormFactorBToPiDKMMO2008Test :
    public TestCase
{
    public:
        AnalyticFormFactorBToPiDKMMO2008Test() :
            TestCase("analytic_form_factor_b_to_pi_DKMMO2008_test")
        {
        }

        virtual void run() const
        {
            static const double eps = 1e-5;

            /* Factory */
            {
                Parameters p = Parameters::Defaults();
                std::shared_ptr<FormFactors<PToP>> ff = FormFactorFactory<PToP>::create("B->pi::DKMMO2008", p, Options{ });

                TEST_CHECK(0 != ff.get());
            }

            /* Decay Constant */
            {
                Parameters p = Parameters::Defaults();
                AnalyticFormFactorBToPiDKMMO2008 ff(p, Options{ });
                p["mass::B_d"] = 5.2795;
                p["mass::b(MSbar)"] = 4.2;
                p["B->pi::mu@DKMMO2008"] = 4.2;
                p["B->pi::Mp^2@DKMMO2008"] = 5.0;       // decay constant
                p["B->pi::sp_0^B@DKMMO2008"] = 35.75;   // decay constant
                p["B->pi::s_0^+(0)@DKMMO2008"] = 37.5;  // f_+
                p["B->pi::s_0^+'(0)@DKMMO2008"] = 0.0;  // f_+
                p["B->pi::s_0^0'(0)@DKMMO2008"] = 0.0;  // f_0
                p["B->pi::s_0^T(0)@DKMMO2008"] = 37.5;  // f_T
                p["B->pi::s_0^T'(0)@DKMMO2008"] = 0.0;  // f_T
                p["QCD::m_0^2"] = 0.8;
                p["QCD::cond_GG"] = 0.012;
                p["QCD::r_vac"] = 1.0;

                Diagnostics diagnostics = ff.diagnostics();
                static const std::vector<std::pair<double, double>> reference
                {
                    std::make_pair(-5.05150,  eps), // rho_1(s = 19.60, m_b = 4.16, mu = 4.16)
                    std::make_pair(-4.62757,  eps), // rho_1(s = 22.05, m_b = 4.16, mu = 4.16)
                    std::make_pair(+0.67764,  eps), // rho_1(s = 25.20, m_b = 4.16, mu = 4.16)
                    std::make_pair( 0.22315, 1e-3), // f_B
                    std::make_pair( 1.00000,  eps), // rescale factor for f_+ at s =  0.0 GeV^2
                    std::make_pair( 1.09609,  eps), // rescale factor for f_+ at s = 10.0 GeV^2
                    std::make_pair( 1.00000,  eps), // rescale factor for f_0 at s =  0.0 GeV^2
                    std::make_pair( 1.14470,  eps), // rescale factor for f_0 at s = 10.0 GeV^2
                    std::make_pair( 1.00000,  eps), // rescale factor for f_T at s =  0.0 GeV^2
                    std::make_pair( 1.07532,  eps), // rescale factor for f_T at s = 10.0 GeV^2
                    std::make_pair( 5.07482,  eps), // M_B for f_+ at s =  0.0 GeV^2
                    std::make_pair( 4.91462,  eps), // M_B for f_+ at s = 10.0 GeV^2
                    std::make_pair( 5.07482,  eps), // M_B for f_0 at s =  0.0 GeV^2
                    std::make_pair( 5.25937,  eps), // M_B for f_0 at s = 10.0 GeV^2
                    std::make_pair( 5.07051,  eps), // M_B for f_T at s =  0.0 GeV^2
                    std::make_pair( 4.91132,  eps), // M_B for f_T at s = 10.0 GeV^2
                };

                TEST_CHECK_DIAGNOSTICS(diagnostics, reference);
            }

            /*
             * B -> pi f_+ Form Factor at test scale mu = 3.0 GeV.
             * These test values are in reasonably agreement with values
             * derived from the Mathematica notebook graciously
             * provided by I. Sentitemsu Imsong.
             */
            {
                static const double eps = 1e-4;

                Parameters p = Parameters::Defaults();
                p["mass::pi^+"] = 0.13957;
                p["mass::b(MSbar)"] = 4.18;
                p["mass::d(2GeV)"] = 0.0048;
                p["mass::u(2GeV)"] = 0.0032;
                p["pi::a2@1GeV"] = 0.17;
                p["pi::a4@1GeV"] = 0.06;
                p["pi::f3@1GeV"] = 0.0045;
                p["pi::omega3@1GeV"] = -1.5;
                p["pi::omega4@1GeV"] = 0.2;
                p["pi::delta^2@1GeV"] = 0.18;
                p["B->pi::M^2@DKMMO2008"] = 12.0;
                p["B->pi::Mp^2@DKMMO2008"] = 4.5;
                p["B->pi::mu@DKMMO2008"] = 3.0;
                p["B->pi::s_0^+(0)@DKMMO2008"] = 37.5;
                p["B->pi::s_0^+'(0)@DKMMO2008"] = 0.0;
                p["B->pi::s_0^0'(0)@DKMMO2008"] = 0.0;
                p["B->pi::s_0^T(0)@DKMMO2008"] = 37.5;
                p["B->pi::s_0^T'(0)@DKMMO2008"] = 0.0;
                p["B->pi::sp_0^B@DKMMO2008"] = 36.5;
                p["QCD::m_0^2"] = 0.8;
                p["QCD::cond_GG"] = 0.012;
                p["QCD::r_vac"] = 1.0;
                p["QCD::alpha_s(MZ)"] = 0.1184;

                AnalyticFormFactorBToPiDKMMO2008 ff(p, Options{ });

                // LO, tw2
                TEST_CHECK_NEARLY_EQUAL( 0.1167, ff.F_lo_tw2(-5.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1484, ff.F_lo_tw2(-1.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1584, ff.F_lo_tw2( 0.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1696, ff.F_lo_tw2( 1.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2290, ff.F_lo_tw2( 5.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.3604, ff.F_lo_tw2(10.0),  eps);

                // LO, tw3
                TEST_CHECK_NEARLY_EQUAL( 0.1261, ff.F_lo_tw3(-5.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1628, ff.F_lo_tw3(-1.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1746, ff.F_lo_tw3( 0.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1876, ff.F_lo_tw3( 1.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2580, ff.F_lo_tw3( 5.0),  eps);
                TEST_CHECK_NEARLY_EQUAL( 0.4213, ff.F_lo_tw3(10.0),  eps);

                // LO, tw4
                TEST_CHECK_NEARLY_EQUAL(-0.0013, ff.F_lo_tw4( 0.0),  eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0016, ff.F_lo_tw4( 1.0),  eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0034, ff.F_lo_tw4( 5.0),  eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0087, ff.F_lo_tw4(10.0),  eps);

                // NLO, tw2
                const auto nlo_eps = 400 * eps;
                TEST_CHECK_NEARLY_EQUAL(+0.7706, ff.F_nlo_tw2(-5.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(+0.7706, ff.F_nlo_tw2(-1.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(+0.7706, ff.F_nlo_tw2( 0.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(+0.8190, ff.F_nlo_tw2( 1.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(+1.0609, ff.F_nlo_tw2( 5.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(+1.4741, ff.F_nlo_tw2(10.0), nlo_eps);

                // NLO, tw3
                TEST_CHECK_NEARLY_EQUAL(-0.9221, ff.F_nlo_tw3(-5.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(-0.9221, ff.F_nlo_tw3(-1.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(-0.9221, ff.F_nlo_tw3( 0.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(-0.9963, ff.F_nlo_tw3( 1.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(-1.4371, ff.F_nlo_tw3( 5.0), nlo_eps);
                TEST_CHECK_NEARLY_EQUAL(-2.7571, ff.F_nlo_tw3(10.0), nlo_eps);

                // fp form factor @ mu = 3.0
                TEST_CHECK_NEARLY_EQUAL( 0.2831, ff.f_p( 0.0), 10 * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2831, ff.f_p( 0.0), 10 * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2831, ff.f_p( 0.0), 10 * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2988, ff.f_p( 1.0), 10 * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.3777, ff.f_p( 5.0), 10 * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.5346, ff.f_p(10.0), 10 * eps);

                AnalyticFormFactorBToPiDKMMO2008 ff_no_rescale(p, Options{{"rescale-borel", "0"}});

                // Ftil LO, tw3
                TEST_CHECK_NEARLY_EQUAL( 0.0480, ff_no_rescale.Ftil_lo_tw3(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0480, ff_no_rescale.Ftil_lo_tw3(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0480, ff_no_rescale.Ftil_lo_tw3( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0512, ff_no_rescale.Ftil_lo_tw3( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0677, ff_no_rescale.Ftil_lo_tw3( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1058, ff_no_rescale.Ftil_lo_tw3(10.0), 1. * eps);

                // Ftil LO, tw4
                TEST_CHECK_NEARLY_EQUAL( 0.0012, ff_no_rescale.Ftil_lo_tw4(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0012, ff_no_rescale.Ftil_lo_tw4(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0012, ff_no_rescale.Ftil_lo_tw4( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0013, ff_no_rescale.Ftil_lo_tw4( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0012, ff_no_rescale.Ftil_lo_tw4( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0006, ff_no_rescale.Ftil_lo_tw4(10.0), 1. * eps);

                // Ftil NLO, tw2
                TEST_CHECK_NEARLY_EQUAL( 0.2454, ff_no_rescale.Ftil_nlo_tw2(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2454, ff_no_rescale.Ftil_nlo_tw2(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2454, ff_no_rescale.Ftil_nlo_tw2(1e-5), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2513, ff_no_rescale.Ftil_nlo_tw2( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2775, ff_no_rescale.Ftil_nlo_tw2( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.3147, ff_no_rescale.Ftil_nlo_tw2(10.0), 1. * eps);

                // Ftil NLO, tw3
                TEST_CHECK_NEARLY_EQUAL( 1.1373, ff_no_rescale.Ftil_nlo_tw3(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 1.1373, ff_no_rescale.Ftil_nlo_tw3(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 1.1373, ff_no_rescale.Ftil_nlo_tw3(1e-5), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 1.1747, ff_no_rescale.Ftil_nlo_tw3( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 1.3439, ff_no_rescale.Ftil_nlo_tw3( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 1.5684, ff_no_rescale.Ftil_nlo_tw3(10.0), 1. * eps);

                // f0 form factor @ mu = 3.0
                TEST_CHECK_NEARLY_EQUAL( 0.2834, ff_no_rescale.f_0(-5.0), 10. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2834, ff_no_rescale.f_0(-1.0), 10. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2834, ff_no_rescale.f_0( 0.0), 10. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2941, ff_no_rescale.f_0( 1.0), 10. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.3456, ff_no_rescale.f_0( 5.0), 10. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.4425, ff_no_rescale.f_0(10.0), 10. * eps);

                // FT LO, tw2
                TEST_CHECK_NEARLY_EQUAL( 0.0354, ff_no_rescale.FT_lo_tw2(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0354, ff_no_rescale.FT_lo_tw2(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0354, ff_no_rescale.FT_lo_tw2( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0373, ff_no_rescale.FT_lo_tw2( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0473, ff_no_rescale.FT_lo_tw2( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0680, ff_no_rescale.FT_lo_tw2(10.0), 1. * eps);

                // FT LO, tw3
                TEST_CHECK_NEARLY_EQUAL( 0.0233, ff_no_rescale.FT_lo_tw3(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0233, ff_no_rescale.FT_lo_tw3(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0233, ff_no_rescale.FT_lo_tw3( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0248, ff_no_rescale.FT_lo_tw3( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0330, ff_no_rescale.FT_lo_tw3( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.0520, ff_no_rescale.FT_lo_tw3(10.0), 1. * eps);

                // FT LO, tw4
                TEST_CHECK_NEARLY_EQUAL(-0.0016, ff_no_rescale.FT_lo_tw4(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0016, ff_no_rescale.FT_lo_tw4(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0016, ff_no_rescale.FT_lo_tw4( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0017, ff_no_rescale.FT_lo_tw4( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0025, ff_no_rescale.FT_lo_tw4( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.0044, ff_no_rescale.FT_lo_tw4(10.0), 1. * eps);

                // FT NLO, tw2
                TEST_CHECK_NEARLY_EQUAL( 0.1506, ff_no_rescale.FT_nlo_tw2(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1506, ff_no_rescale.FT_nlo_tw2(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1506, ff_no_rescale.FT_nlo_tw2( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1573, ff_no_rescale.FT_nlo_tw2( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.1870, ff_no_rescale.FT_nlo_tw2( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2211, ff_no_rescale.FT_nlo_tw2(10.0), 1. * eps);

                // FT NLO, tw3
                TEST_CHECK_NEARLY_EQUAL(-0.1717, ff_no_rescale.FT_nlo_tw3(-5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.1717, ff_no_rescale.FT_nlo_tw3(-1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.1717, ff_no_rescale.FT_nlo_tw3( 0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.1887, ff_no_rescale.FT_nlo_tw3( 1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.2928, ff_no_rescale.FT_nlo_tw3( 5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL(-0.6184, ff_no_rescale.FT_nlo_tw3(10.0), 1. * eps);

                // fT form factor @ mu = 3.0
                TEST_CHECK_NEARLY_EQUAL( 0.1685, ff_no_rescale.f_t(-10.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2080, ff_no_rescale.f_t( -5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2515, ff_no_rescale.f_t( -1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2647, ff_no_rescale.f_t(  0.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.2790, ff_no_rescale.f_t(  1.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.3510, ff_no_rescale.f_t(  5.0), 1. * eps);
                TEST_CHECK_NEARLY_EQUAL( 0.4904, ff_no_rescale.f_t( 10.0), 1. * eps);

            }
        }
} analytic_form_factor_b_to_pi_DKMMO2008_test;
