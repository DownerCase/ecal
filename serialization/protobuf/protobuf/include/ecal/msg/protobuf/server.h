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
 * @file   server.h
 * @brief  eCAL Server interface based on protobuf service description
**/

#pragma once

#include <ecal/v5/ecal_server.h>
#include <ecal/msg/protobuf/ecal_proto_dyn.h>
#include <functional>

// protobuf includes
#ifdef _MSC_VER
#pragma warning(push, 0) // disable proto warnings
#endif
#include <google/protobuf/descriptor.pb.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

// stl includes
#include <iostream>
#include <map>
#include <memory>
#include <string>

namespace eCAL
{
  namespace protobuf
  {
    /**
     * @brief Google Protobuf Server wrapper class.
    **/
    template <typename T>
    class CServiceServer : public eCAL::v5::CServiceServer
    {
    public:
      /**
       * @brief Constructor.
      **/
      CServiceServer() : m_service(nullptr)
      {
      }

      /**
       * @brief Constructor.
       *
       * @param service_  Google protobuf service instance.
      **/
      CServiceServer(std::shared_ptr<T> service_) : m_service(nullptr)
      {
        Create(service_);
      }

      /**
       * @brief Constructor.
       *
       * @param service_       Google protobuf service instance.
       * @param service_name_  Unique service name.
      **/
      CServiceServer(std::shared_ptr<T> service_, const std::string& service_name_) : m_service(nullptr)
      {
        Create(service_, service_name_);
      }

      /**
       * @brief Destructor.
      **/
      ~CServiceServer() override
      {
        Destroy();
      }

      /**
       * @brief CServiceServers are non-copyable
      **/
      CServiceServer(const CServiceServer&) = delete;

      /**
       * @brief CServiceServers are non-copyable
      **/
      CServiceServer& operator=(const CServiceServer&) = delete;

      /**
       * @brief Create service.
       *
       * @param service_ Google protobuf service instance.
       *
       * @return  True if successful.
      **/
      bool Create(std::shared_ptr<T> service_)
      {
        return Create(service_, "");
      }
        
      /**
       * @brief Create service.
       *
       * @param service_       Google protobuf service instance.
       * @param service_name_  Unique service name.
       *
       * @return  True if successful.
      **/
      bool Create(std::shared_ptr<T> service_, const std::string& service_name_)
      {
        if (service_ == nullptr) return false;
        m_service = service_;

        const google::protobuf::ServiceDescriptor* service_descriptor = service_->GetDescriptor();
        if (service_name_.empty())
        {
          Create(service_descriptor->full_name());
        }
        else
        {
          Create(service_name_);
        }

        std::string error_s;
        CProtoDynDecoder dyn_decoder;

        for (int i = 0; i < service_descriptor->method_count(); ++i)
        {
          // get method name and descriptor
          const google::protobuf::MethodDescriptor* method_descriptor = service_descriptor->method(i);
          std::string method_name = method_descriptor->name();
          m_methods[method_name] = method_descriptor;

          // get message type names
          std::string input_type_name  = method_descriptor->input_type()->name();
          std::string output_type_name = method_descriptor->output_type()->name();

          // get message type descriptors
          std::string input_type_desc;
          std::string output_type_desc;
          dyn_decoder.GetServiceMessageDescFromType(service_descriptor, input_type_name, input_type_desc, error_s);
          dyn_decoder.GetServiceMessageDescFromType(service_descriptor, output_type_name, output_type_desc, error_s);

          // store descriptions
          AddDescription(method_name, input_type_name, input_type_desc, output_type_name, output_type_desc);

          // add callback
          AddMethodCallback(method_name,
            input_type_name,
            output_type_name,
            std::bind(&CServiceServer::RequestCallback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4, std::placeholders::_5)
          );
        }
        return true;
      }

      /**
       * @brief Destroy service.
       *
       * @return  True if successful.
      **/
      bool Destroy()
      {
        m_service = nullptr;
        return true;
      }

      using eCAL::v5::CServiceServer::Create;

    protected:
      int RequestCallback(const std::string& method_, const std::string& /*req_type_*/, const std::string& /*resp_type_*/, const std::string& request_, std::string& response_)
      {
        if (m_service == nullptr) return -1;

        auto iter = m_methods.find(method_);
        if (iter == m_methods.end()) return -1;

        // method descriptor
        const google::protobuf::MethodDescriptor* method_descriptor = iter->second;

        // create request
        google::protobuf::Message* request(m_service->GetRequestPrototype(method_descriptor).New());
        if (!request->ParseFromString(request_))
        {
          std::cerr << "Protobuf Service " << GetServiceName() << " failed to parse request message!" << std::endl;
          delete request;
          return -1;
        }

        // create response
        google::protobuf::Message* response(m_service->GetResponsePrototype(method_descriptor).New());
        // call method
        m_service->CallMethod(method_descriptor, nullptr, request, response, nullptr);
        response_ = response->SerializeAsString();

        delete request;
        delete response;

        return 0;
      };

      std::shared_ptr<T>                                                m_service;
      std::map<std::string, const google::protobuf::MethodDescriptor*>  m_methods;

      private:
        using eCAL::v5::CServiceServer::AddMethodCallback;
        using eCAL::v5::CServiceServer::RemMethodCallback;
        using eCAL::v5::CServiceServer::GetServiceName;
    };
  }
}
