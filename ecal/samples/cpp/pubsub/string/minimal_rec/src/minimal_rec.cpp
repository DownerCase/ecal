/* ========================= eCAL LICENSE =================================
 *
 * Copyright (C) 2016 - 2019 Continental Corporation
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

#include <ecal/ecal.h>
#include <ecal/msg/string/subscriber.h>

#include <iostream>
#include <sstream>

int main()
{
  std::cout << "-------------------------------" << std::endl;
  std::cout << " HELLO WORLD RECEIVER"           << std::endl;
  std::cout << "-------------------------------" << std::endl;

  // initialize eCAL API
  eCAL::Initialize("minimal_rec");

  // subscriber for topic "Hello"
  eCAL::string::CSubscriber<std::string> sub("Hello");

  // receive updates
  std::string rcv_content;
  while (eCAL::Ok())
  {
    // receive content
    if (sub.Receive(rcv_content, nullptr, 100))
    {
      std::cout << "Received \"" << rcv_content << "\"" << std::endl;
    }
  }

  // finalize eCAL API
  eCAL::Finalize();

  return(0);
}
