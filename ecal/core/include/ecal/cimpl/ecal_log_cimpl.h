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
 * @file   ecal_log_cimpl.h
 * @brief  eCAL logging c interface
**/

#ifndef ecal_log_cimpl_h_included
#define ecal_log_cimpl_h_included

#include <ecal/ecal_os.h>

#include <ecal/ecal_log_level.h>

#ifdef __cplusplus
extern "C"
{
#endif /*__cplusplus*/
  /**
   * @brief Log a message (with current log level). 
   *
   * @param msg_  The log message string.
  **/
  ECALC_API void eCAL_Logging_Log(enum eCAL_Logging_eLogLevel level_, const char* const msg_);

  /**
   * @brief Get logging string.
   *
   * @param [out] buf_      Pointer to store the logging information.
   * @param       buf_len_  Length of allocated buffer or ECAL_ALLOCATE_4ME if
   *                        eCAL should allocate the buffer for you (see eCAL_FreeMem).
   *
   * @return  Logging buffer length or zero if failed.
  **/
  ECALC_API int eCAL_Logging_GetLogging(void* buf_, int buf_len_);
#ifdef __cplusplus
}
#endif /*__cplusplus*/

#endif /*ecal_log_cimpl_h_included*/
