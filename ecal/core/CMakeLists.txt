# ========================= eCAL LICENSE =================================
#
# Copyright (C) 2016 - 2025 Continental Corporation
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

project(core VERSION ${eCAL_VERSION_STRING})

find_package(asio             REQUIRED)
find_package(Threads          REQUIRED)
find_package(ecaludp          REQUIRED)

if (ECAL_CORE_CONFIGURATION)
  find_package(yaml-cpp       REQUIRED)
  include(${ECAL_PROJECT_ROOT}/thirdparty/yaml-cpp/compatibility-yaml-cpp.cmake)
  yaml_cpp_create_compatibility_targets()
endif()

if (ECAL_CORE_COMMAND_LINE)
  find_package(tclap          REQUIRED)
endif()

if (ECAL_CORE_TRANSPORT_TCP)
  find_package(tcp_pubsub     REQUIRED)
endif()

# If we're currently doing a build within a git repository, we will configure the header files.
# Else, (e.g. for source packages such as debian source packages) we will use a preconfigured file.
# If there is really no information available, it will generate a dummy version file 0.0.0
if     (IS_GIT_TREE OR NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/ecal/ecal_defs.h")
  configure_file(src/ecal_defs.h.in "${CMAKE_CURRENT_BINARY_DIR}/include/ecal/ecal_defs.h" @ONLY)
endif  (IS_GIT_TREE OR NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/include/ecal/ecal_defs.h")

if(UNIX)
 include (CheckSymbolExists)
 set(CMAKE_REQUIRED_DEFINITIONS "-D__USE_GNU"
                                "-D_GNU_SOURCE")
  set(CMAKE_REQUIRED_LIBRARIES "pthread")
  check_symbol_exists(pthread_mutex_clocklock "pthread.h" ECAL_HAS_CLOCKLOCK_MUTEX)
  check_symbol_exists(pthread_mutexattr_setrobust "pthread.h" ECAL_HAS_ROBUST_MUTEX)
  unset(CMAKE_REQUIRED_DEFINITIONS)
  unset(CMAKE_REQUIRED_LIBRARIES)
  if(NOT ECAL_HAS_ROBUST_MUTEX)
    if(NOT ECAL_HAS_CLOCKLOCK_MUTEX)
      message(WARNING "This OS does not support robust mutexes with monotonic clock which can cause dead locks under certain circumstances. (e.g. memfile monitoring).")
    else()
      message(WARNING "This OS does not support robust mutexes which can cause dead locks under certain circumstances (e.g. memfile monitoring).")
    endif()
  endif()
endif()

######################################
# config
######################################
set(ecal_config_src
    src/config/default_configuration.cpp        
    src/config/ecal_config.cpp
    src/config/configuration.cpp
    src/config/ecal_path_processing.cpp
    src/types/ecal_custom_data_types.cpp
)
if (ECAL_CORE_CONFIGURATION)
  list(APPEND ecal_config_src
      src/config/configuration_to_yaml.cpp
      src/config/configuration_reader.cpp
      src/config/configuration_writer.cpp
  )
endif()
######################################
# io/mtx
######################################
if(ECAL_CORE_REGISTRATION_SHM OR ECAL_CORE_TRANSPORT_SHM)
  set(ecal_io_mtx_src
      src/io/mtx/ecal_named_mutex.cpp
      src/io/mtx/ecal_named_mutex.h
      src/io/mtx/ecal_named_mutex_base.h
  )

  # io/mtx/linux
  if(UNIX)
    set(ecal_io_mtx_linux_src
        src/io/mtx/linux/ecal_named_mutex_impl.cpp
        $<$<OR:$<BOOL:${ECAL_HAS_CLOCKLOCK_MUTEX}>,$<BOOL:${ECAL_HAS_ROBUST_MUTEX}>>:src/io/mtx/linux/ecal_named_mutex_robust_clocklock_impl.cpp>
        src/io/mtx/linux/ecal_named_mutex_impl.h
        $<$<OR:$<BOOL:${ECAL_HAS_CLOCKLOCK_MUTEX}>,$<BOOL:${ECAL_HAS_ROBUST_MUTEX}>>:src/io/mtx/linux/ecal_named_mutex_robust_clocklock_impl.h>
    )
  endif()

  # io/mtx/win32
  if(WIN32)
    set(ecal_io_mtx_win_src
        src/io/mtx/win32/ecal_named_mutex_impl.cpp
        src/io/mtx/win32/ecal_named_mutex_impl.h
    )
  endif()
endif()

######################################
# io/shm
######################################
if(ECAL_CORE_REGISTRATION_SHM OR ECAL_CORE_TRANSPORT_SHM)
  set(ecal_io_shm_src
      src/io/shm/ecal_memfile.cpp
      src/io/shm/ecal_memfile_db.cpp
      src/io/shm/ecal_memfile_naming.cpp      
      src/io/shm/ecal_memfile_pool.cpp
      src/io/shm/ecal_memfile_sync.cpp
      src/io/shm/ecal_memfile.h
      src/io/shm/ecal_memfile_db.h
      src/io/shm/ecal_memfile_header.h
      src/io/shm/ecal_memfile_info.h
      src/io/shm/ecal_memfile_naming.h
      src/io/shm/ecal_memfile_os.h
      src/io/shm/ecal_memfile_pool.h
      src/io/shm/ecal_memfile_sync.h
  )

  # io/shm/linux
  if(UNIX)
    set(ecal_io_shm_linux_src
        src/io/shm/linux/ecal_memfile_os.cpp
    )
  endif()

  # io/shm/win32
  if(WIN32)
    set(ecal_io_shm_win_src
       src/io/shm/win32/ecal_memfile_os.cpp
    )
  endif()
endif()

######################################
# io/udp
######################################
# io/udp (npcap)
if(ECAL_CORE_NPCAP_SUPPORT)
set(ecal_io_udp_src_npcap 
    src/io/udp/ecal_udp_sample_receiver_npcap.cpp
    src/io/udp/ecal_udp_sample_receiver_npcap.h
)
endif()

# io/udp
set(ecal_io_udp_src
    src/io/udp/ecal_udp_configurations.cpp
    src/io/udp/ecal_udp_configurations.h
    src/io/udp/ecal_udp_receiver_attr.h
    src/io/udp/ecal_udp_sample_receiver.cpp
    src/io/udp/ecal_udp_sample_receiver.h
    src/io/udp/ecal_udp_sample_receiver_asio.cpp
    src/io/udp/ecal_udp_sample_receiver_asio.h
    src/io/udp/ecal_udp_sample_receiver_base.h
    src/io/udp/ecal_udp_sample_sender.cpp
    src/io/udp/ecal_udp_sample_sender.h
    src/io/udp/ecal_udp_sender_attr.h
    src/io/udp/ecal_udp_topic2mcast.h
    ${ecal_io_udp_src_npcap}
)

# io/udp/sendreceive/linux
if(UNIX)
  set(ecal_io_udp_linux_src
      src/io/udp/linux/socket_os.h
)
endif()

######################################
# logging
######################################
set(ecal_logging_src
    src/logging/ecal_log.cpp
    src/logging/ecal_log_provider.cpp
    src/logging/ecal_log_receiver.cpp
)

######################################
# monitoring
######################################
if(ECAL_CORE_MONITORING)
set(ecal_monitoring_src
    src/monitoring/ecal_monitoring_def.cpp
    src/monitoring/ecal_monitoring_def.h
    src/monitoring/ecal_monitoring_impl.cpp
    src/monitoring/ecal_monitoring_impl.h
)
endif()

######################################
# pubsub
######################################
if(ECAL_CORE_PUBLISHER)
  set(ecal_pub_src
      src/pubsub/ecal_publisher.cpp
      src/pubsub/ecal_publisher_impl.cpp
      src/pubsub/ecal_publisher_impl.h
      src/pubsub/ecal_publisher_v5.cpp
      src/pubsub/ecal_pubgate.cpp
      src/pubsub/ecal_pubgate.h
  )
endif()

if(ECAL_CORE_SUBSCRIBER)
  set(ecal_sub_src
      src/pubsub/ecal_subscriber.cpp
      src/pubsub/ecal_subscriber_impl.cpp
      src/pubsub/ecal_subscriber_impl.h
      src/pubsub/ecal_subscriber_v5.cpp
      src/pubsub/ecal_subgate.cpp
      src/pubsub/ecal_subgate.h
  )
endif()

######################################
# readwrite
######################################
set(ecal_readwrite_src
    src/readwrite/ecal_transport_layer.h
)

if(ECAL_CORE_PUBLISHER)
  set(ecal_writer_src
      src/readwrite/ecal_writer_base.h
      src/readwrite/ecal_writer_buffer_payload.h
      src/readwrite/ecal_writer_data.h
      src/readwrite/ecal_writer_info.h
  )
  if(ECAL_CORE_TRANSPORT_UDP)
    list(APPEND ecal_writer_src
        src/readwrite/udp/ecal_writer_udp.cpp
        src/readwrite/udp/ecal_writer_udp.h
    )
  endif()
  if(ECAL_CORE_TRANSPORT_TCP)
    list(APPEND ecal_writer_src
        src/readwrite/tcp/ecal_tcp_pubsub_logger.h
        src/readwrite/tcp/ecal_writer_tcp.cpp
        src/readwrite/tcp/ecal_writer_tcp.h
    )
  endif()
  if(ECAL_CORE_TRANSPORT_SHM)
    list(APPEND ecal_writer_src
        src/readwrite/shm/ecal_writer_shm.cpp
        src/readwrite/shm/ecal_writer_shm.h
    )
  endif()
endif()

if(ECAL_CORE_SUBSCRIBER)
  set(ecal_reader_src
      src/readwrite/ecal_reader_layer.h
  )
  if(ECAL_CORE_TRANSPORT_UDP)
    list(APPEND ecal_reader_src
        src/readwrite/udp/ecal_reader_udp.cpp
        src/readwrite/udp/ecal_reader_udp.h
    )
  endif()
  if(ECAL_CORE_TRANSPORT_TCP)
    list(APPEND ecal_reader_src
        src/readwrite/tcp/ecal_reader_tcp.cpp
        src/readwrite/tcp/ecal_reader_tcp.h
    )
  endif()
  if(ECAL_CORE_TRANSPORT_SHM)
    list(APPEND ecal_reader_src
        src/readwrite/shm/ecal_reader_shm.cpp
        src/readwrite/shm/ecal_reader_shm.h
    )
  endif()
endif()

######################################
# registration
######################################
if (ECAL_CORE_REGISTRATION)
  set(ecal_registration_src
      src/registration/ecal_process_registration.cpp
      src/registration/ecal_process_registration.h
      src/registration/ecal_registration.cpp
      src/registration/ecal_registration_provider.cpp
      src/registration/ecal_registration_provider.h
      src/registration/ecal_registration_receiver.cpp
      src/registration/ecal_registration_receiver.h
      src/registration/ecal_registration_sample_applier.cpp
      src/registration/ecal_registration_sample_applier.h
      src/registration/ecal_registration_sample_applier_gates.cpp
      src/registration/ecal_registration_sample_applier_gates.h
      src/registration/ecal_registration_timeout_provider.cpp
      src/registration/ecal_registration_timeout_provider.h
      src/registration/ecal_registration_sender.h
      src/registration/udp/ecal_registration_receiver_udp.cpp
      src/registration/udp/ecal_registration_receiver_udp.h
      src/registration/udp/ecal_registration_sender_udp.cpp
      src/registration/udp/ecal_registration_sender_udp.h
  )
  if(ECAL_CORE_REGISTRATION_SHM)
    list(APPEND ecal_registration_src
        src/registration/shm/ecal_registration_receiver_shm.cpp
        src/registration/shm/ecal_registration_receiver_shm.h
        src/registration/shm/ecal_registration_sender_shm.cpp
        src/registration/shm/ecal_registration_sender_shm.h
        src/registration/shm/ecal_memfile_broadcast.cpp
        src/registration/shm/ecal_memfile_broadcast.h
        src/registration/shm/ecal_memfile_broadcast_reader.cpp
        src/registration/shm/ecal_memfile_broadcast_reader.h
        src/registration/shm/ecal_memfile_broadcast_writer.cpp
        src/registration/shm/ecal_memfile_broadcast_writer.h
        src/registration/shm/relocatable_circular_queue.h
    )
  endif()
endif()

######################################
# serialization
######################################
set(ecal_serialization_src
    src/serialization/nanopb/pb.h
    src/serialization/nanopb/pb_common.c
    src/serialization/nanopb/pb_common.h
    src/serialization/nanopb/pb_decode.c
    src/serialization/nanopb/pb_decode.h
    src/serialization/nanopb/pb_encode.c
    src/serialization/nanopb/pb_encode.h
    src/serialization/nanopb/ecal/core/pb/datatype.npb.c
    src/serialization/nanopb/ecal/core/pb/datatype.npb.h
    src/serialization/nanopb/ecal/core/pb/ecal.npb.c
    src/serialization/nanopb/ecal/core/pb/ecal.npb.h
    src/serialization/nanopb/ecal/core/pb/host.npb.c
    src/serialization/nanopb/ecal/core/pb/host.npb.h
    src/serialization/nanopb/ecal/core/pb/layer.npb.c
    src/serialization/nanopb/ecal/core/pb/layer.npb.h
    src/serialization/nanopb/ecal/core/pb/logging.npb.c
    src/serialization/nanopb/ecal/core/pb/logging.npb.h
    src/serialization/nanopb/ecal/core/pb/monitoring.npb.c
    src/serialization/nanopb/ecal/core/pb/monitoring.npb.h
    src/serialization/nanopb/ecal/core/pb/process.npb.c
    src/serialization/nanopb/ecal/core/pb/process.npb.h
    src/serialization/nanopb/ecal/core/pb/service.npb.c
    src/serialization/nanopb/ecal/core/pb/service.npb.h
    src/serialization/nanopb/ecal/core/pb/topic.npb.c
    src/serialization/nanopb/ecal/core/pb/topic.npb.h
    src/serialization/ecal_serialize_common.cpp
    src/serialization/ecal_serialize_common.h
    src/serialization/ecal_serialize_logging.cpp
    src/serialization/ecal_serialize_logging.h
    src/serialization/ecal_serialize_monitoring.cpp
    src/serialization/ecal_serialize_monitoring.h
    src/serialization/ecal_serialize_sample_payload.cpp
    src/serialization/ecal_serialize_sample_payload.h
    src/serialization/ecal_serialize_sample_registration.cpp
    src/serialization/ecal_serialize_sample_registration.h
    src/serialization/ecal_serialize_service.cpp
    src/serialization/ecal_serialize_service.h
    src/serialization/ecal_struct_sample_common.h
    src/serialization/ecal_struct_sample_payload.h
    src/serialization/ecal_struct_sample_registration.h
    src/serialization/ecal_struct_service.h
)

######################################
# service
######################################
if(ECAL_CORE_SERVICE)
  set(ecal_service_src
      src/service/ecal_clientgate.cpp
      src/service/ecal_clientgate.h
      src/service/ecal_servicegate.cpp
      src/service/ecal_servicegate.h
      src/service/ecal_service_client.cpp
      src/service/ecal_service_client_impl.cpp
      src/service/ecal_service_client_impl.h
      src/service/ecal_service_client_instance.cpp
      src/service/ecal_service_client_v5.cpp
      src/service/ecal_service_client_v5_impl.cpp
      src/service/ecal_service_client_v5_impl.h
      src/service/ecal_service_server.cpp
      src/service/ecal_service_server_impl.cpp
      src/service/ecal_service_server_impl.h
      src/service/ecal_service_server_v5.cpp
      src/service/ecal_service_server_v5_impl.cpp
      src/service/ecal_service_server_v5_impl.h
      src/service/ecal_service_singleton_manager.cpp
      src/service/ecal_service_singleton_manager.h
  )
endif()

######################################
# time
######################################
set(ecal_time_src
    src/time/ecal_time.cpp
    src/time/ecal_timer.cpp
)
if(ECAL_CORE_TIMEPLUGIN)
  list(APPEND ecal_time_src
      src/time/ecal_timegate.cpp
      src/time/ecal_timegate.h
  )
endif()

######################################
# util
######################################
set(ecal_util_src
    src/util/ecal_expmap.h
    src/util/ecal_thread.h
    src/util/expanding_vector.h
    src/util/frequency_calculator.h
    src/util/getenvvar.h
)
if (ECAL_CORE_COMMAND_LINE)
  list(APPEND ecal_util_src
      src/util/advanced_tclap_output.cpp
      src/util/advanced_tclap_output.h
  )
endif()

######################################
# common
######################################
set(ecal_cmn_src
    src/ecal.cpp
    src/ecal_def.h
    src/ecal_descgate.cpp
    src/ecal_descgate.h
    src/ecal_event.cpp 
    src/ecal_event.h
    src/ecal_eventhandle.h
    src/ecal_global_accessors.cpp
    src/ecal_global_accessors.h
    src/ecal_globals.cpp
    src/ecal_globals.h
    src/ecal_process.cpp
    src/ecal_util.cpp
    src/ecal_win_main.h
)
if (WIN32)
  list(APPEND ecal_cmn_src
      src/ecal_win_main.h
  )
endif()

######################################
# builder
######################################
set (ecal_builder_src
    src/config/builder/logging_attribute_builder.cpp
    src/config/builder/registration_attribute_builder.cpp
    src/logging/config/builder/udp_attribute_builder.cpp
    src/pubsub/config/builder/reader_attribute_builder.cpp
    src/pubsub/config/builder/writer_attribute_builder.cpp
    src/readwrite/config/builder/shm_attribute_builder.cpp
    src/readwrite/config/builder/tcp_attribute_builder.cpp
    src/readwrite/config/builder/udp_attribute_builder.cpp
    src/readwrite/tcp/config/builder/data_reader_tcp_attribute_builder.cpp
    src/readwrite/udp/config/builder/udp_attribute_builder.cpp
    src/registration/config/builder/udp_shm_attribute_builder.cpp
    src/registration/config/builder/sample_applier_attribute_builder.cpp
    src/registration/udp/config/builder/udp_attribute_builder.cpp
)

######################################
# public header
######################################
set(ecal_header_cmn
    include/ecal/config/application.h
    include/ecal/config/configuration.h
    include/ecal/config/logging.h
    include/ecal/config/publisher.h
    include/ecal/config/registration.h
    include/ecal/config/subscriber.h
    include/ecal/config/time.h
    include/ecal/config/transport_layer.h
    include/ecal/types/logging.h
    include/ecal/types/monitoring.h
    include/ecal/ecal.h
    include/ecal/ecal_callback.h
    include/ecal/ecal_callback_v5.h
    include/ecal/ecal_client.h
    include/ecal/ecal_client_instance.h
    include/ecal/ecal_client_v5.h
    include/ecal/ecal_config.h
    include/ecal/ecal_core.h
    include/ecal/ecal_deprecate.h
    include/ecal/ecal_init.h
    include/ecal/ecal_log.h
    include/ecal/ecal_log_level.h
    include/ecal/ecal_os.h
    include/ecal/ecal_payload_writer.h
    include/ecal/ecal_monitoring.h
    include/ecal/ecal_process.h
    include/ecal/ecal_process_severity.h
    include/ecal/ecal_registration.h
    include/ecal/ecal_publisher.h
    include/ecal/ecal_publisher_v5.h
    include/ecal/ecal_server.h
    include/ecal/ecal_server_v5.h
    include/ecal/ecal_service_info.h
    include/ecal/ecal_subscriber.h
    include/ecal/ecal_subscriber_v5.h
    include/ecal/ecal_time.h
    include/ecal/ecal_timer.h
    include/ecal/ecal_tlayer.h
    include/ecal/ecal_types.h
    include/ecal/ecal_util.h
)

set(ecal_header_msg
    include/ecal/msg/dynamic.h
    include/ecal/msg/publisher.h
    include/ecal/msg/subscriber.h
)

set(ecal_sources
    ${ecal_config_src}
    ${ecal_io_mtx_src}
    ${ecal_io_mtx_linux_src}
    ${ecal_io_mtx_win_src}
    ${ecal_io_shm_src}
    ${ecal_io_shm_linux_src}
    ${ecal_io_shm_win_src}
    ${ecal_io_udp_src}
    ${ecal_io_udp_linux_src}
    ${ecal_logging_src}
    ${ecal_monitoring_src}
    ${ecal_pub_src}
    ${ecal_sub_src}
    ${ecal_readwrite_src}
    ${ecal_writer_src}
    ${ecal_reader_src}
    ${ecal_registration_src}
    ${ecal_serialization_src}
    ${ecal_service_src}
    ${ecal_time_src}
    ${ecal_util_src}
    ${ecal_cmn_src}
    ${ecal_builder_src}
    ${ecal_header_cmn}
    ${ecal_header_msg}
)

ecal_add_ecal_shared_library(${PROJECT_NAME}
    ${ecal_sources}
    ${CMAKE_CURRENT_BINARY_DIR}/include/ecal/ecal_defs.h
)

if (ECAL_CORE_CONFIGURATION)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ECAL_CORE_CONFIGURATION)
endif()

if(UNIX)
  set_source_files_properties(src/util/convert_utf.cpp PROPERTIES COMPILE_FLAGS -Wno-implicit-fallthrough)
endif()

add_library(eCAL::${PROJECT_NAME}   ALIAS ${PROJECT_NAME})

target_compile_definitions(${PROJECT_NAME}
  PUBLIC
    ASIO_STANDALONE
    ASIO_DISABLE_VISIBILITY
  PRIVATE
    ECAL_CORE_EXPORTS
    $<$<BOOL:${ECAL_HAS_CLOCKLOCK_MUTEX}>:ECAL_HAS_CLOCKLOCK_MUTEX>
    $<$<BOOL:${ECAL_HAS_ROBUST_MUTEX}>:ECAL_HAS_ROBUST_MUTEX>
    $<$<BOOL:${ECAL_USE_CLOCKLOCK_MUTEX}>:ECAL_USE_CLOCKLOCK_MUTEX>
    ECAL_NO_DEPRECATION_WARNINGS
  INTERFACE
    ECAL_CORE_IMPORTS
)

set_target_properties(${PROJECT_NAME}
  PROPERTIES 
    CXX_VISIBILITY_PRESET hidden
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

set(ECAL_CORE_FEATURES
  ECAL_CORE_CONFIGURATION
  ECAL_CORE_COMMAND_LINE
  ECAL_CORE_REGISTRATION
  ECAL_CORE_MONITORING
  ECAL_CORE_PUBLISHER
  ECAL_CORE_SUBSCRIBER
  ECAL_CORE_SERVICE
  ECAL_CORE_TIMEPLUGIN
  ECAL_CORE_REGISTRATION_SHM
  ECAL_CORE_TRANSPORT_UDP
  ECAL_CORE_TRANSPORT_TCP
  ECAL_CORE_TRANSPORT_SHM
  ECAL_CORE_NPCAP_SUPPORT
)

foreach(CORE_FEATURE ${ECAL_CORE_FEATURES})
  if(${CORE_FEATURE})
    target_compile_definitions(${PROJECT_NAME}
      PUBLIC ${CORE_FEATURE})
  endif()
endforeach()

if(ECAL_CORE_CONFIGURATION)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      yaml-cpp::yaml-cpp
  )
endif()

if(ECAL_CORE_COMMAND_LINE)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      tclap::tclap
  )
endif()

if(ECAL_CORE_SERVICE)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      ecal_service
  )
endif()

if(ECAL_CORE_TRANSPORT_TCP)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
      tcp_pubsub::tcp_pubsub
  )
endif()

target_link_libraries(${PROJECT_NAME}
  PRIVATE
    ecaludp::ecaludp
)

target_include_directories(${PROJECT_NAME}
  PRIVATE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/serialization/nanopb>
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include/>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${QNXNTO}>>>:dl>
    $<$<AND:$<BOOL:${UNIX}>,$<NOT:$<BOOL:${APPLE}>>,$<NOT:$<BOOL:${QNXNTO}>>>:rt>
    $<$<STREQUAL:${CMAKE_SYSTEM_NAME},FreeBSD>:util>
    $<$<BOOL:${WIN32}>:iphlpapi>
    $<$<BOOL:${WIN32}>:psapi> 
    $<$<BOOL:${WIN32}>:shlwapi.lib> 
    $<$<BOOL:${WIN32}>:winmm> 
    $<$<BOOL:${WIN32}>:ws2_32> 
    $<$<BOOL:${WIN32}>:wsock32>
    $<$<BOOL:${QNXNTO}>:socket>
    asio::asio
    Threads::Threads
    eCAL::ecal-utils
)

set_property(TARGET ${PROJECT_NAME}   PROPERTY FOLDER core)

ecal_install_ecal_shared_library(${PROJECT_NAME})

install(DIRECTORY
   "include/" DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT sdk
    FILES_MATCHING PATTERN "*.h")

# install generated ecal_defs.h file    
install(DIRECTORY
   "${CMAKE_CURRENT_BINARY_DIR}/include/" DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT sdk
    FILES_MATCHING PATTERN "*.h")

if(NOT ${CMAKE_VERSION} VERSION_LESS "3.8.0") 
  source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES 
    ${ecal_sources}
  )
endif()

# eCAL process stub
if(UNIX)
    set(PROJECT_NAME_PROCESS_STUB process_stub)

    ecal_add_app_console(${PROJECT_NAME_PROCESS_STUB}
        src/ecal_process_stub.cpp
        src/ecal_process_stub.h
    )
    
    target_compile_features(${PROJECT_NAME_PROCESS_STUB} PUBLIC cxx_std_14)
    
    ecal_install_app(${PROJECT_NAME_PROCESS_STUB})

    set_property(TARGET ${PROJECT_NAME_PROCESS_STUB} PROPERTY FOLDER core)
endif(UNIX)