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

project(ThreadingUtils)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Threads REQUIRED)

set (threading_utils_includes
  include/ThreadingUtils/DynamicSleeper.h
  include/ThreadingUtils/InterruptibleLoopThread.h
  include/ThreadingUtils/InterruptibleThread.h
  include/ThreadingUtils/ThreadSafeQueue.h
)

set(threading_utils_src
  src/dummy.cpp # Workaround to make this project generate an output. Header only librarys do not work well with Visual Studio
)

# WARNING: This is only used for the play and recorder applications (CLI and GUI)
# so it is never needed by the SDK portion of the codebase and thus can be:
# Internal implementation - not exported/installed
ecal_add_static_library(${PROJECT_NAME}
  ${threading_utils_includes}
  ${threading_utils_src}
)

target_include_directories(${PROJECT_NAME}
  PUBLIC include/
  PRIVATE src/)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)

# Create a source tree that mirrors the filesystem
source_group(TREE "${CMAKE_CURRENT_LIST_DIR}"
    FILES
        ${threading_utils_includes}
        ${threading_utils_src}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER lib)
