/* -*-  Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2012 University of California, Los Angeles
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

// ndn-simple-with-custom-app.cc

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/point-to-point-module.h"

using namespace ns3;
using ns3::ndn::StackHelper;
using ns3::ndn::AppHelper;

/**
 * This scenario simulates a one-node two-custom-app scenario:
 *
 *   +------+ <-----> (CustomApp1)
 *   | Node |
 *   +------+ <-----> (CustomApp2)
 *
 *     NS_LOG=CustomApp ./waf --run=ndn-simple-with-custom-app
 */

int
main (int argc, char *argv[])
{
  // Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
  CommandLine cmd;
  cmd.Parse (argc, argv);

  // Creating nodes
  Ptr<Node> node = CreateObject<Node> ();

  // Install CCNx stack on all nodes
  StackHelper ccnxHelper;
  ccnxHelper.InstallAll ();

  // Installing applications

  // Consumer
  AppHelper consumerHelper ("CustomApp");
  //consumerHelper.Install (nodes.Get (0));
  ApplicationContainer app1 = consumerHelper.Install (node);
  ApplicationContainer app2 = consumerHelper.Install (node);

  app1.Start (Seconds (1.0)); // will send out Interest, which nobody will receive (Interests generated by an app will not got back to the app)
  app2.Start (Seconds (2.0)); // will send out an Interests, which will be received and satisfied by app1

  Simulator::Stop (Seconds (3.0));

  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
