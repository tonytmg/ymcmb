/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (c) 2014  Regents of the University of California,
 *                     Arizona Board of Regents,
 *                     Colorado State University,
 *                     University Pierre & Marie Curie, Sorbonne University,
 *                     Washington University in St. Louis,
 *                     Beijing Institute of Technology
 *
 * This file is part of NFD (Named Data Networking Forwarding Daemon).
 * See AUTHORS.md for complete list of NFD authors and contributors.
 *
 * NFD is free software: you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * NFD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * NFD, e.g., in COPYING.md file.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "table/strategy-info-host.hpp"

#include "tests/test-common.hpp"

namespace nfd {
namespace tests {

static int g_DummyStrategyInfo_count = 0;

class DummyStrategyInfo : public fw::StrategyInfo
{
public:
  DummyStrategyInfo(int id)
    : m_id(id)
  {
    ++g_DummyStrategyInfo_count;
  }

  virtual ~DummyStrategyInfo()
  {
    --g_DummyStrategyInfo_count;
  }

  int m_id;
};

BOOST_FIXTURE_TEST_SUITE(TableStrategyInfoHost, BaseFixture)

BOOST_AUTO_TEST_CASE(SetGetClear)
{
  StrategyInfoHost host;

  BOOST_CHECK(!static_cast<bool>(host.getStrategyInfo<DummyStrategyInfo>()));

  g_DummyStrategyInfo_count = 0;

  shared_ptr<DummyStrategyInfo> info = make_shared<DummyStrategyInfo>(7591);
  host.setStrategyInfo(info);
  BOOST_REQUIRE(static_cast<bool>(host.getStrategyInfo<DummyStrategyInfo>()));
  BOOST_CHECK_EQUAL(host.getStrategyInfo<DummyStrategyInfo>()->m_id, 7591);

  info.reset(); // unlink local reference
  // host should still have a reference to info
  BOOST_REQUIRE(static_cast<bool>(host.getStrategyInfo<DummyStrategyInfo>()));
  BOOST_CHECK_EQUAL(host.getStrategyInfo<DummyStrategyInfo>()->m_id, 7591);

  host.clearStrategyInfo();
  BOOST_CHECK(!static_cast<bool>(host.getStrategyInfo<DummyStrategyInfo>()));
  BOOST_CHECK_EQUAL(g_DummyStrategyInfo_count, 0);
}

BOOST_AUTO_TEST_SUITE_END()

} // namespace tests
} // namespace nfd
