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
 * @file   ecal_service_info.h
 * @brief  eCAL service info
**/

#pragma once

#include <ecal/ecal_types.h>

#include <functional>
#include <string>
#include <vector>
#include <map>

namespace eCAL
{
  /**
   * @brief  Service call state.
  **/
  enum class eCallState
  {
    none = 0,    //!< undefined
    executed,    //!< executed (successfully)
    timeouted,   //!< timeout
    failed       //!< failed
  };

  /**
   * @brief Service method information struct containing the request and response type information.
  **/
  struct SMethodInfo
  {
    std::string              method_name; //!< The name of the method.
    SDataTypeInformation     req_type;    //!< The type of the method request.
    SDataTypeInformation     resp_type;   //!< The type of the method response.
  };

  /**
   * @brief Service response struct containing the (responding) server informations and the response itself. (deprecated)
  **/
  struct SServiceResponse
  {
    SServiceResponse()
    {
      ret_state  = 0;
      call_state = eCallState::none;
    };
    std::string  host_name;      //!< service host name
    std::string  service_name;   //!< name of the service
    std::string  service_id;     //!< id of the service
    std::string  method_name;    //!< name of the service method
    std::string  error_msg;      //!< human readable error message
    int          ret_state;      //!< return state of the called service method
    eCallState   call_state;     //!< call state (see eCallState)
    std::string  response;       //!< service response
  };
  using ServiceResponseVecT = std::vector<SServiceResponse>; //!< vector of multiple service responses (deprecated)

  /**
   * @brief Service response struct containing the (responding) server informations and the response itself.
  **/
  struct SServiceIDResponse
  {
    Registration::SServiceMethodId service_method_id;            //!< service method information (service id (entity id, process id, host name), service name, method name)
    std::string                    error_msg;                    //!< human readable error message
    int                            ret_state  = 0;               //!< return state of the called service method
    eCallState                     call_state = eCallState::none; //!< call state (see eCallState)
    std::string                    response;                     //!< service response
  };
  using ServiceIDResponseVecT = std::vector<SServiceIDResponse>; //!< vector of multiple service responses

  /**
   * @brief Service method callback function type (low level server interface). (deprecated)
   *
   * @param method_     The method name.
   * @param req_type_   The type of the method request.
   * @param resp_type_  The type of the method response.
   * @param request_    The request.
   * @param response_   The response returned from the method call.
  **/
  using MethodCallbackT = std::function<int(const std::string& method_, const std::string& req_type_, const std::string& resp_type_, const std::string& request_, std::string& response_)>;

  /**
   * @brief Service method callback function type (low level server interface).
   *
   * @param method_info The method information struct containing the request and response type information.
   * @param request_    The request.
   * @param response_   The response returned from the method call.
  **/
  using MethodInfoCallbackT = std::function<int(const SMethodInfo& method_info_, const std::string& request_, std::string& response_)>;

  /**
   * @brief Service response callback function type (low level client interface). (deprecated)
   *
   * @param service_response_  Service response struct containing the (responding) server informations and the response itself.
  **/
  using ResponseCallbackT = std::function<void(const struct SServiceResponse& service_response_)>;

  /**
   * @brief Service response callback function type (low level client interface).
   * 
   * @param entity_id_         Unique service id (entity id, process id, host name, service name, method name)
   * @param service_response_  Service response struct containing the (responding) server informations and the response itself.
  **/
  using ResponseIDCallbackT = std::function<void (const Registration::SEntityId& entity_id_, const struct SServiceIDResponse& service_response_)>;

  /**
   * @brief Map of <method name, method information (like request type, reponse type)>.
  **/
  using ServiceMethodInformationMapT = std::map<std::string, SServiceMethodInformation>;
}
