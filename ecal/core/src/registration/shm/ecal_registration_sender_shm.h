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
 * @brief  eCAL registration provider
 *
 * All process internal publisher/subscriber, server/clients register here with all their attributes.
 *
 * These information will be send cyclic (registration refresh) via UDP to external eCAL processes.
 *
**/

#pragma once

#include "registration/ecal_registration_sender.h"

#include "registration/shm/ecal_memfile_broadcast.h"
#include "registration/shm/ecal_memfile_broadcast_writer.h"

#include "config/attributes/registration_shm_attributes.h"

#include <vector>

namespace eCAL
{
  class CRegistrationSenderSHM : public CRegistrationSender
  {
  public:
    CRegistrationSenderSHM(const Registration::SHM::SAttributes& attr_);
    ~CRegistrationSenderSHM() override;

    // Special member functionss
    CRegistrationSenderSHM(const CRegistrationSenderSHM& other) = delete;
    CRegistrationSenderSHM& operator=(const CRegistrationSenderSHM& other) = delete;
    CRegistrationSenderSHM(CRegistrationSenderSHM&& other) noexcept = delete;
    CRegistrationSenderSHM& operator=(CRegistrationSenderSHM&& other) noexcept = delete;

    //bool SendSample(const Registration::Sample& sample_) override;
    bool SendSampleList(const Registration::SampleList& sample_list) override;

  private:
    CMemoryFileBroadcast                m_memfile_broadcast;
    CMemoryFileBroadcastWriter          m_memfile_broadcast_writer;
    std::vector<char>                   m_sample_list_buffer;
  };
}