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
 * @brief  eCAL service client implementation (deprecated eCAL5 version)
**/

#include "ecal_service_client_v5_impl.h"
#include <ecal/ecal_log.h>

namespace eCAL
{
  namespace v5
  {
    CServiceClientImpl::CServiceClientImpl()
      : m_service_client_impl(nullptr)
    {
      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Initializing default service client implementation.");
    }

    CServiceClientImpl::CServiceClientImpl(const std::string& service_name_)
      : m_service_client_impl(nullptr)
    {
      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Initializing service client with name: " + service_name_);
      Create(service_name_);
    }

    CServiceClientImpl::CServiceClientImpl(const std::string& service_name_, const ServiceMethodInformationMapT& method_information_map_)
      : m_service_client_impl(nullptr)
    {
      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Initializing service client with name: " + service_name_);
      Create(service_name_, method_information_map_);
    }

    CServiceClientImpl::~CServiceClientImpl()
    {
      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Destroying service client implementation.");
      Destroy();
    }

    bool CServiceClientImpl::Create(const std::string& service_name_)
    {
      return Create(service_name_, ServiceMethodInformationMapT());
    }

    bool CServiceClientImpl::Create(const std::string& service_name_, const ServiceMethodInformationMapT& method_information_map_)
    {
      if (m_service_client_impl != nullptr)
      {
        Logging::Log(log_level_warning, "v5::CServiceClientImpl: Service client already created: " + service_name_);
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Creating service client with name: " + service_name_);

      // Define the event callback to pass to CServiceClient
      ClientEventIDCallbackT event_callback = [this](const Registration::SServiceMethodId& service_id_, const struct SClientEventCallbackData& data_)
        {
          Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Event callback triggered for event type: " + std::to_string(data_.type));

          // Lock the mutex to safely access m_event_callbacks
          std::lock_guard<std::mutex> lock(m_event_callback_map_mutex);

          // Check if there's a callback registered for the event type
          const auto& callback = m_event_callback_map.find(data_.type);
          if (callback != m_event_callback_map.end())
          {
            Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Executing event callback for event type: " + std::to_string(data_.type));
            // Call the user's callback
            callback->second(service_id_.service_name.c_str(), &data_);
          }
        };

      // Create the new service client implementation with the event callback
      m_service_client_impl = std::make_shared<eCAL::CServiceClient>(
        service_name_,
        method_information_map_,
        event_callback
      );

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Service client created successfully with name: " + service_name_);
      return true;
    }

    bool CServiceClientImpl::Destroy()
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_warning, "v5::CServiceClientImpl: Service client not initialized, cannot destroy.");
        return false;
      }

      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Destroying service client implementation.");

      // Reset the service client implementation
      m_service_client_impl.reset();

      // Clear stored callbacks
      m_response_callback = nullptr;
      m_event_callback_map.clear();
      m_host_name.clear();

      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Service client destroyed successfully.");
      return true;
    }

    bool CServiceClientImpl::SetHostName(const std::string& host_name_)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot set host name.");
        return false;
      }

      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Setting host name to: " + host_name_);

      // Store the host name filter
      m_host_name = host_name_;

      return true;
    }

    bool CServiceClientImpl::Call(const std::string& method_name_, const std::string& request_, int timeout_)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot make a call.");
        return false;
      }
      if (!m_response_callback)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Response callback not set, cannot make a call.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Making a synchronous call to method: " + method_name_);

      // Wrap the response callback to filter by host name if necessary
      ResponseIDCallbackT wrapped_callback = [this](const Registration::SEntityId& /*entity_id_*/, const struct SServiceResponse& service_response_)
        {
          if (m_host_name.empty() || service_response_.host_name == m_host_name)
          {
            Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Response received for method call.");
            // Call the stored response callback
            m_response_callback(service_response_);
          }
        };

      // Call the method using the new API
      return m_service_client_impl->CallWithCallback(method_name_, request_, timeout_, wrapped_callback);
    }

    bool CServiceClientImpl::Call(const std::string& method_name_, const std::string& request_, int timeout_, ServiceResponseVecT* service_response_vec_)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot make a call.");
        return false;
      }
      if (!service_response_vec_)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Response vector is null, cannot make a call.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Making a synchronous call with response collection to method: " + method_name_);

      // Call the method using the new API
      ServiceResponseVecT all_responses;
      bool success = m_service_client_impl->CallWithResponse(method_name_, request_, timeout_, all_responses);

      // Filter responses based on host name if necessary
      if (!m_host_name.empty())
      {
        Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Filtering responses based on host name: " + m_host_name);
        for (const auto& response : all_responses)
        {
          if (response.host_name == m_host_name)
          {
            service_response_vec_->push_back(response);
          }
        }
      }
      else
      {
        *service_response_vec_ = std::move(all_responses);
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Call completed with success: " + std::to_string(success));
      return success;
    }

    bool CServiceClientImpl::CallAsync(const std::string& method_name_, const std::string& request_, int /*timeout_*/)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot make an async call.");
        return false;
      }
      if (!m_response_callback)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Response callback not set, cannot make an async call.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Making an asynchronous call to method: " + method_name_);

      // Wrap the response callback to filter by host name if necessary
      ResponseIDCallbackT wrapped_callback = [this](const Registration::SEntityId& /*entity_id_*/, const struct SServiceResponse& service_response_)
        {
          if (m_host_name.empty() || service_response_.host_name == m_host_name)
          {
            Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Response received for async method call.");
            // Call the stored response callback
            m_response_callback(service_response_);
          }
        };

      // Call the method asynchronously using the new API
      bool success = m_service_client_impl->CallWithCallbackAsync(method_name_, request_, wrapped_callback);
      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Async call to method: " + method_name_ + " completed with success: " + std::to_string(success));
      return success;
    }

    bool CServiceClientImpl::AddResponseCallback(const ResponseCallbackT& callback_)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot add response callback.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Adding response callback.");

      {
        const std::lock_guard<std::mutex> lock(m_response_callback_mutex);
        m_response_callback = callback_;
      }

      return true;
    }

    bool CServiceClientImpl::RemResponseCallback()
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot remove response callback.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Removing response callback.");

      {
        const std::lock_guard<std::mutex> lock(m_response_callback_mutex);
        m_response_callback = nullptr;
      }

      return true;
    }

    bool CServiceClientImpl::AddEventCallback(eCAL_Client_Event type_, ClientEventCallbackT callback_)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot add event callback.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Adding event callback for event type: " + std::to_string(type_));

      {
        const std::lock_guard<std::mutex> lock(m_event_callback_map_mutex);
        m_event_callback_map[type_] = callback_;
      }

      return true;
    }

    bool CServiceClientImpl::RemEventCallback(eCAL_Client_Event type_)
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot remove event callback.");
        return false;
      }

      Logging::Log(log_level_debug1, "v5::CServiceClientImpl: Removing event callback for event type: " + std::to_string(type_));

      {
        const std::lock_guard<std::mutex> lock(m_event_callback_map_mutex);
        m_event_callback_map.erase(type_);
      }

      return true;
    }

    std::string CServiceClientImpl::GetServiceName()
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot get service name.");
        return "";
      }

      std::string service_name = m_service_client_impl->GetServiceName();
      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Retrieved service name: " + service_name);
      return service_name;
    }

    bool CServiceClientImpl::IsConnected()
    {
      if (m_service_client_impl == nullptr)
      {
        Logging::Log(log_level_error, "v5::CServiceClientImpl: Service client not initialized, cannot check connection status.");
        return false;
      }

      bool connected = m_service_client_impl->IsConnected();
      Logging::Log(log_level_debug2, "v5::CServiceClientImpl: Connection status: " + std::string(connected ? "connected" : "disconnected"));
      return connected;
    }
  }
}
