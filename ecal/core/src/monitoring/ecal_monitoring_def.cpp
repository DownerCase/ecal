/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2024 Continental Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * ========================= eCAL LICENSE =================================
*/

/**
 * @brief  Global monitoring class
**/

#include <ecal/ecal.h>

#include "ecal_monitoring_def.h"
#include "ecal_monitoring_impl.h"
#include "ecal_global_accessors.h"

namespace eCAL
{
  CMonitoring::CMonitoring(const Monitoring::SAttributes& attr_)
  {
    m_monitoring_impl = std::make_unique<CMonitoringImpl>(attr_);
  }

  CMonitoring::~CMonitoring()
  {
    m_monitoring_impl.reset();
  }

  void CMonitoring::Start()
  {
    m_monitoring_impl->Create();
  }

  void CMonitoring::Stop()
  {
    m_monitoring_impl->Destroy();
  }

  void CMonitoring::SetExclFilter(const std::string& filter_)
  {
    m_monitoring_impl->SetExclFilter(filter_);
  }

  void CMonitoring::SetInclFilter(const std::string& filter_)
  {
    m_monitoring_impl->SetInclFilter(filter_);
  }

  void CMonitoring::SetFilterState(bool state_)
  {
    m_monitoring_impl->SetFilterState(state_);
  }

  void CMonitoring::GetMonitoring(std::string& monitoring_, unsigned int entities_)
  {
    m_monitoring_impl->GetMonitoring(monitoring_, entities_);
  }

  void CMonitoring::GetMonitoring(eCAL::Monitoring::SMonitoring& monitoring_, unsigned int entities_)
  {
    m_monitoring_impl->GetMonitoring(monitoring_, entities_);
  }

  namespace Monitoring
  {
    ////////////////////////////////////////////////////////
    // static library interface
    ////////////////////////////////////////////////////////
    bool SetExclFilter(const std::string& filter_)
    {
      if (g_monitoring() != nullptr)
      {
        g_monitoring()->SetExclFilter(filter_);
        return true;
      }
      return false;
    }

    bool SetInclFilter(const std::string& filter_)
    {
      if (g_monitoring() != nullptr)
      {
        g_monitoring()->SetInclFilter(filter_);
        return true;
      }
      return false;
    }

    bool SetFilterState(const bool state_)
    {
      if (g_monitoring() != nullptr)
      {
        g_monitoring()->SetFilterState(state_);
        return true;
      }
      return false;
    }

    bool GetMonitoring(std::string& mon_, unsigned int entities_)
    {
      if (g_monitoring() != nullptr)
      {
        mon_.clear();
        g_monitoring()->GetMonitoring(mon_, entities_);
        return true;
      }
      return false;
    }

    bool GetMonitoring(SMonitoring& mon_, unsigned int entities_)
    {
      if (g_monitoring() != nullptr)
      {
        g_monitoring()->GetMonitoring(mon_, entities_);
        return true;
      }
      return false;
    }
  }
}
