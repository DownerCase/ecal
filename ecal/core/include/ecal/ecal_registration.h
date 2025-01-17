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
 * @file   ecal_registration.h
 * @brief  eCAL registration interface
**/

#pragma once

#include <ecal/ecal_os.h>
#include <ecal/ecal_types.h>

#include <cstdint>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace eCAL
{
  namespace Registration
  {
    struct SServiceMethod
    {
      std::string service_name;
      std::string method_name;

      bool operator<(const SServiceMethod& other) const
      {
        return std::tie(service_name, method_name) < std::tie(other.service_name, other.method_name);
      }
    };

    using CallbackToken = std::size_t;

    enum class RegistrationEventType
    {
      new_entity,     //!< Represents a new entity registration
      deleted_entity  //!< Represents a deletion of an entity
    };

    using TopicIDCallbackT = std::function<void(const STopicId&, RegistrationEventType)>;

    /**
     * @brief Get complete snapshot of all known publisher.
     *
     * @return Set of topic id's.
    **/
    ECAL_API std::set<STopicId> GetPublisherIDs();

    /**
     * @brief Get data type information with quality for specific publisher.
     *
     * @return True if information could be queried.
    **/
    ECAL_API bool GetPublisherInfo(const STopicId& id_, SDataTypeInformation& topic_info_);

    /**
     * @brief Register a callback function to be notified when a new publisher becomes available.
     *
     * @param callback_       The callback function to be called with the STopicId of the new publisher.
     *                        The callback function must not be blocked for a longer period of time, 
     *                        otherwise timeout mechanisms of the eCAL registration would be triggered.
     *
     * @return CallbackToken  Token that can be used to unregister the callback.
     */
    ECAL_API CallbackToken AddPublisherEventCallback(const TopicIDCallbackT& callback_);

    /**
     * @brief Unregister the publisher callback using the provided token.
     *
     * @param token  The token returned by AddPublisherCallback.
    */
    ECAL_API void RemPublisherEventCallback(CallbackToken token_);

    /**
     * @brief Get complete snapshot of all known subscriber.
     *
     * @return Set of topic id's.
    **/
    ECAL_API std::set<STopicId> GetSubscriberIDs();

    /**
     * @brief Get data type information with quality for specific subscriber.
     *
     * @return True if information could be queried.
    **/
    ECAL_API bool GetSubscriberInfo(const STopicId& id_, SDataTypeInformation& topic_info_);

    /**
     * @brief Register a callback function to be notified when a new subscriber becomes available.
     *
     * @param callback_       The callback function to be called with the STopicId of the new subscriber.
     *                        The callback function must not be blocked for a longer period of time, 
     *                        otherwise timeout mechanisms of the eCAL registration would be triggered.
     *
     * @return CallbackToken  Token that can be used to unregister the callback.
     */
    ECAL_API CallbackToken AddSubscriberEventCallback(const TopicIDCallbackT& callback_);

    /**
     * @brief Unregister the subscriber callback using the provided token.
     *
     * @param token  The token returned by AddSubscriberCallback.
    */
    ECAL_API void RemSubscriberEventCallback(CallbackToken token_);

    /**
     * @brief Get complete snapshot of all known services.
     *
     * @return Set of service id's.
    **/
    ECAL_API std::set<SServiceMethodId> GetServiceIDs();

    /**
     * @brief Get service method information with quality for specific service.
     *
     * @return True if information could be queried.
    **/
    ECAL_API bool GetServiceInfo(const SServiceMethodId& id_, SServiceMethodInformation& service_method_info_);

    /**
     * @brief Get complete snapshot of all known clients.
     *
     * @return Set of service id's.
    **/
    ECAL_API std::set<SServiceMethodId> GetClientIDs();

    /**
     * @brief Get service method information with quality for specific client.
     *
     * @return True if information could be queried.
    **/
    ECAL_API bool GetClientInfo(const SServiceMethodId& id_, SServiceMethodInformation& service_method_info_);

    /**
     * @brief Get all topic names.
     *
     * @param topic_names_ Set to store the topic names.
    **/
    ECAL_API void GetTopicNames(std::set<std::string>& topic_names_);

    /**
     * @brief Get all service/method names.
     *
     * @param service_method_names_ Set to store the service/method names (Set { (ServiceName, MethodName) }).
    **/
    ECAL_API void GetServiceMethodNames(std::set<SServiceMethod>& service_method_names_);

    /**
     * @brief Get all client/method names.
     *
     * @param client_method_names_ Set to store the client/method names (Set { (ClientName, MethodName) }).
    **/
    ECAL_API void GetClientMethodNames(std::set<SServiceMethod>& client_method_names_);
  }
}
