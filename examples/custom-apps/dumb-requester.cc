/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011-2012 University of California, Los Angeles
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Alexander Afanasyev <alexander.afanasyev@ucla.edu>
 */

// dumb-requester.cc

#include "dumb-requester.h"
#include "ns3/ptr.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/random-variable.h"
#include "ns3/string.h"

#include "ns3/ndn-interest.h"
#include "ns3/ndn-data.h"
#include "ns3/ndn-app-face.h"

NS_LOG_COMPONENT_DEFINE ("DumbRequester");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (DumbRequester);

// register NS-3 type
TypeId
DumbRequester::GetTypeId ()
{
  static TypeId tid = TypeId ("DumbRequester")
    .SetParent<ndn::App> ()
    .AddConstructor<DumbRequester> ()

    .AddAttribute ("Prefix", "Requested name",
                   StringValue ("/dumb-interest"),
                   ndn::MakeNameAccessor (&DumbRequester::m_name),
                   ndn::MakeNameChecker ())
    ;
  return tid;
}

DumbRequester::DumbRequester ()
  : m_isRunning (false)
{
}

// Processing upon start of the application
void
DumbRequester::StartApplication ()
{
  // initialize ndn::App
  ndn::App::StartApplication ();

  m_isRunning = true;
  Simulator::ScheduleNow (&DumbRequester::SendInterest, this);
}

// Processing when application is stopped
void
DumbRequester::StopApplication ()
{
  m_isRunning = false;
  // cleanup ndn::App
  ndn::App::StopApplication ();
}

void
DumbRequester::SendInterest ()
{
  if (!m_isRunning) return;

  /////////////////////////////////////
  // Sending one Interest packet out //
  /////////////////////////////////////

  shared_ptr<Name> prefix = make_shared<Name> (m_name); // another way to create name

  // Create and configure ndn::Interest
  shared_ptr<ndn::Interest> interest = make_shared<ndn::Interest> ();
  UniformVariable rand (0,std::numeric_limits<uint32_t>::max ());
  interest->setNonce            (rand.GetValue ());
  interest->setName             (*prefix);
  Time lifetime = MilliSeconds (1000);
  interest->setInterestLifetime (::ndn::time::milliseconds (lifetime.GetMilliSeconds ()));

  NS_LOG_DEBUG ("Sending Interest packet for " << *prefix);

  ndn::FwHopCountTag hopCountTag;
  interest->getPacket ()->AddPacketTag (hopCountTag);

  // Call trace (for logging purposes)
  m_transmittedInterests ((dynamic_cast<::ndn::Interest&>(*interest).shared_from_this()), this, m_face);

  // Forward packet to lower (network) layer
  m_face->onReceiveInterest (dynamic_cast<::ndn::Interest&>(*interest));

  Simulator::Schedule (Seconds (1.0), &DumbRequester::SendInterest, this);
}

void
DumbRequester::OnData (shared_ptr<const ::ndn::Data> contentObject)
{
  NS_LOG_DEBUG ("Receiving Data packet for " << contentObject->getName ());
}


} // namespace ns3
