/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2025 Continental Corporation
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
 * @brief Registration public API.
 *
**/

#include <ecal/registration.h>

#include "ecal_def.h"
#include "ecal_globals.h"
#include "ecal_event.h"
#include "registration/ecal_registration_receiver.h"
#include "pubsub/ecal_pubgate.h"

namespace eCAL
{
  namespace Registration
  {
    std::set<STopicId> GetPublisherIDs()
    {
      if (g_descgate() == nullptr) return std::set<STopicId>();
      return g_descgate()->GetPublisherIDs();
    }

    bool GetPublisherInfo(const STopicId& id_, SDataTypeInformation& topic_info_)
    {
      if (g_descgate() == nullptr) return false;
      return g_descgate()->GetPublisherInfo(id_, topic_info_);
    }

    CallbackToken AddPublisherEventCallback(const TopicEventCallbackT& callback_)
    {
      if (g_descgate() == nullptr) return CallbackToken();
      return g_descgate()->AddPublisherEventCallback(callback_);
    }

    void RemPublisherEventCallback(CallbackToken token_)
    {
      if (g_descgate() == nullptr) return;
      return g_descgate()->RemPublisherEventCallback(token_);
    }

    std::set<STopicId> GetSubscriberIDs()
    {
      if (g_descgate() == nullptr) return std::set<STopicId>();
      return g_descgate()->GetSubscriberIDs();
    }

    bool GetSubscriberInfo(const STopicId& id_, SDataTypeInformation& topic_info_)
    {
      if (g_descgate() == nullptr) return false;
      return g_descgate()->GetSubscriberInfo(id_, topic_info_);
    }

    ECAL_API CallbackToken AddSubscriberEventCallback(const TopicEventCallbackT& callback_)
    {
      if (g_descgate() == nullptr) return CallbackToken();
      return g_descgate()->AddSubscriberEventCallback(callback_);
    }

    void RemSubscriberEventCallback(CallbackToken token_)
    {
      if (g_descgate() == nullptr) return;
      return g_descgate()->RemSubscriberEventCallback(token_);
    }

    std::set<SServiceId> GetServerIDs()
    {
      if (g_descgate() == nullptr) return std::set<SServiceId>();
      return g_descgate()->GetServerIDs();
    }

    bool GetServerInfo(const SServiceId& id_, ServiceMethodInfoSetT& service_info_)
    {
      if (g_descgate() == nullptr) return false;
      return g_descgate()->GetServerInfo(id_, service_info_);
    }

    std::set<SServiceId> GetClientIDs()
    {
      if (g_descgate() == nullptr) return std::set<SServiceId>();
      return g_descgate()->GetClientIDs();
    }

    bool GetClientInfo(const SServiceId& id_, ServiceMethodInfoSetT& service_info_)
    {
      if (g_descgate() == nullptr) return false;
      return g_descgate()->GetClientInfo(id_, service_info_);
    }

    void GetTopicNames(std::set<std::string>& topic_names_)
    {
      topic_names_.clear();

      // get publisher & subscriber id sets and insert names into the topic_names set
      const std::set<STopicId> pub_id_set = GetPublisherIDs();
      for (const auto& pub_id : pub_id_set)
      {
        topic_names_.insert(pub_id.topic_name);
      }
      const std::set<STopicId> sub_id_set = GetSubscriberIDs();
      for (const auto& sub_id : sub_id_set)
      {
        topic_names_.insert(sub_id.topic_name);
      }
    }
    
    void GetServerMethodNames(std::set<SServiceMethod>& server_method_names_)
    {
      server_method_names_.clear();

      // get servers id set and insert names into the server_method_names_ set
      const std::set<SServiceId> server_id_set = GetServerIDs();
      for (const auto& server_id : server_id_set)
      {
        eCAL::ServiceMethodInfoSetT methods;
        (void)GetServerInfo(server_id, methods);
        for (const auto& method : methods)
        {
          server_method_names_.insert({ server_id.service_name, method.method_name });
        }
      }
    }

    void GetClientMethodNames(std::set<SServiceMethod>& client_method_names_)
    {
      client_method_names_.clear();

      // get clients id set and insert names into the client_method_names set
      const std::set<SServiceId> client_id_set = GetClientIDs();
      for (const auto& client_id : client_id_set)
      {
        eCAL::ServiceMethodInfoSetT methods;
        (void)GetClientInfo(client_id, methods);
        for (const auto& method : methods)
        {
          client_method_names_.insert({ client_id.service_name, method.method_name });
        }
      }
    }
    
  }
}
