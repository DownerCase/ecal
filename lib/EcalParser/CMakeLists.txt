# ========================= eCAL LICENSE =================================
#
# Copyright (C) 2016 - 2019 Continental Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#      http://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# ========================= eCAL LICENSE =================================

project(EcalParser)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
find_package(Threads REQUIRED)

set (includes
    include/EcalParser/EcalParser.h
    include/EcalParser/Function.h
)

set(sources
    src/EcalParser.cpp

    src/functions/env.cpp
    src/functions/env.h
    src/functions/hostname.cpp
    src/functions/hostname.h
    src/functions/os.cpp
    src/functions/os.h
    src/functions/osselect.cpp
    src/functions/osselect.h
    src/functions/time.cpp
    src/functions/time.h
    src/functions/username.cpp
    src/functions/username.h
)

# QEcalParser is an internal implementation detail for applications so it can
# always be static and not installed/exported
ecal_add_static_library(${PROJECT_NAME}
    ${includes}
    ${sources}
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_include_directories(${PROJECT_NAME} PRIVATE src/)

target_link_libraries (${PROJECT_NAME}
    PRIVATE
    eCAL::ecal-utils
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE wsock32 ws2_32)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

# Create a source tree that mirrors the filesystem
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    FILES
        ${includes}
        ${sources}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER lib)
