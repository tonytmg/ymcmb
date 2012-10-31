/* -*- Mode: C++; c-file-style: "gnu"; indent-tabs-mode:nil -*- */
/*
 * Copyright (c) 2011 University of California, Los Angeles
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

#ifndef _NDN_LIMITS_H_
#define	_NDN_LIMITS_H_

#include "ns3/ptr.h"
#include "ns3/object.h"
#include "ns3/traced-value.h"

namespace ns3 {
namespace ndn {

/**
 * \ingroup ndn
 * \brief Abstract class to manage Interest limits 
 */
class Limits :
    public Object
{
public:
  static TypeId
  GetTypeId ();

  /**
   * @brief Default constructor
   */
  Limits ()
  : m_maxRate (-1)
  , m_maxDelay (1.0)
  { }

  /**
   * @brief Virtual destructor
   */
  virtual
  ~Limits () {}
  
  /**
   * @brief Set limit for the number of outstanding interests
   * @param rate   Maximum rate that needs to be enforced
   * @param delay  Maximum delay for BDP product for window-based limits
   */
  virtual void
  SetLimits (double rate, double delay)
  {
    m_maxRate = rate;
    m_maxDelay = delay;
  }    

  /**
   * @brief Get maximum rate that needs to be enforced
   */
  virtual double
  GetMaxRate () const
  {
    return m_maxRate;
  }

  /**
   * @brief Get maximum delay for BDP product for window-based limits
   */
  virtual double
  GetMaxDelay () const
  {
    return m_maxDelay;
  }

  /**
   * @brief Check whether limits are enabled or not
   */
  virtual inline bool
  IsEnabled () const
  {
    return m_maxRate > 0.0;
  }

  /**
   * @brief Update a current value of the limit
   * @param limit Value of current limit.
   *
   * Note that interpretation of this value may be different in different ndn::Limit realizations
   *
   * All realizations will try to guarantee that if limit is larger than previously set value of maximum limit,
   * then the current limit will be limited to that maximum value
   */
  virtual void
  UpdateCurrentLimit (double limit) = 0;

  /**
   * @brief Get value of the current limit
   *
   * Note that interpretation of this value may be different in different ndn::Limit realizations
   */
  virtual double
  GetCurrentLimit () const = 0;
  
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////

  /**
   * @brief Realization-specific method called to check availability of the limit
   */
  virtual bool
  IsBelowLimit () = 0;

  /**
   * @brief "Borrow" limit
   *
   * IsBelowLimit **must** be true, otherwise assert fail
   */
  virtual void
  BorrowLimit () = 0;

  /**
   * @brief "Return" limit
   */
  virtual void
  ReturnLimit () = 0;
  
private:
  double m_maxRate;
  double m_maxDelay;
};
  

} // namespace ndn
} // namespace ns3

#endif // _NDN_LIMITS_H_
