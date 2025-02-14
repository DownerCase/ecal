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
 * @brief  Function definitions for setting default config values
**/

#include "ecal/config.h"
#include "ecal_config_internal.h"
#include "ecal/util.h"

#include "ecal_global_accessors.h"
#include "ecal_def.h"

#include "ecal/process.h"

#ifdef ECAL_CORE_CONFIGURATION
  #include "configuration_reader.h"
  #include "configuration_to_yaml.h"
#endif

#include <climits>

// for cwd
#ifdef ECAL_OS_WINDOWS
  #include <direct.h>
  // to remove deprecated warning
  #define getcwd _getcwd
  constexpr int MAXIMUM_PATH_LENGTH = _MAX_PATH;
#endif
#ifdef ECAL_OS_LINUX
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <unistd.h>
  #include <pwd.h>
  constexpr int MAXIMUM_PATH_LENGTH = PATH_MAX;
#endif

#include "ecal_utils/filesystem.h"
#include "util/getenvvar.h"
#include "ecal_utils/ecal_utils.h"
#include "ecal/log.h"

#include <algorithm>
#include <fstream>
#include <cstddef>

namespace
{
  // copied and adapted from ecal_config_reader.cpp
#ifdef ECAL_OS_WINDOWS
  const char path_separator('\\');
#endif /* ECAL_OS_WINDOWS */
#ifdef ECAL_OS_LINUX
  const char path_separator('/');
#endif /* ECAL_OS_LINUX */

  bool setPathSep(std::string& file_path_)
  {
    if (file_path_.empty())
      return false;

    if (file_path_.back() != path_separator)
    {
      file_path_ += path_separator;
    }
    
    return true;
  }

  std::string eCALDataEnvPath()
  {
    std::string ecal_data_path = getEnvVar("ECAL_DATA");
    setPathSep(ecal_data_path);
    return ecal_data_path;
  }

  std::string cwdPath()
  {
    char temp[MAXIMUM_PATH_LENGTH];  

    if (getcwd(temp, sizeof(temp)) != nullptr)
    {
      std::string cwdPath{temp};
      setPathSep(cwdPath);
      return cwdPath;
    }

    return {};
  }

  std::string eCALDataCMakePath()
  {
    std::string cmake_data_path;
#ifdef ECAL_OS_LINUX
    const std::string ecal_install_config_dir(ECAL_INSTALL_CONFIG_DIR);
    const std::string ecal_install_prefix(ECAL_INSTALL_PREFIX);

    if ((!ecal_install_config_dir.empty() && (ecal_install_config_dir[0] == path_separator))
      || ecal_install_prefix.empty())
    {
      cmake_data_path = ecal_install_config_dir;
    }
    else if (!ecal_install_prefix.empty())
    {
      cmake_data_path = ecal_install_prefix + path_separator + ecal_install_config_dir;
    }
    setPathSep(cmake_data_path);
#endif /* ECAL_OS_LINUX */  
    return cmake_data_path;
  }

  std::string eCALDataSystemPath()
  {
    std::string system_data_path;
#ifdef ECAL_OS_WINDOWS
    system_data_path = getEnvVar("ProgramData");
    if(setPathSep(system_data_path))
    {
        system_data_path += std::string("eCAL");
        setPathSep(system_data_path);
    }
#endif /* ECAL_OS_WINDOWS */

#ifdef ECAL_OS_LINUX
    system_data_path = "/etc/ecal";
    setPathSep(system_data_path);
#endif /* ECAL_OS_LINUX */
    return system_data_path;
  }

  bool isValidConfigFilePath(const std::string& path_, const std::string& file_name_)
  {
    const std::string file_path = path_ + file_name_;
    const EcalUtils::Filesystem::FileStatus ecal_ini_status(file_path, EcalUtils::Filesystem::Current);
    return ecal_ini_status.IsOk() && (ecal_ini_status.GetType() == EcalUtils::Filesystem::Type::RegularFile);
  }

  std::string findValidConfigPath(std::vector<std::string> paths_, const std::string& file_name_)
  {
    auto it = std::find_if(paths_.begin(), paths_.end(), [&file_name_](const std::string& path_)
    {
      return isValidConfigFilePath(path_, file_name_);
    });

    // We should have encountered a valid path
    if (it != paths_.end())
      return (*it);

    // If valid path is not encountered, defaults should be used
    return std::string("");
  }

  std::vector<std::string> getEcalDefaultPaths()
  {
    std::vector<std::string> ecal_default_paths;
      // -----------------------------------------------------------
      // precedence 1: ECAL_DATA variable (windows and linux)
      // -----------------------------------------------------------
      ecal_default_paths.emplace_back(eCALDataEnvPath());
      
      // -----------------------------------------------------------
      // precedence 2:  cmake configured data paths (linux only)
      // -----------------------------------------------------------
      ecal_default_paths.emplace_back(eCALDataCMakePath());

      // -----------------------------------------------------------
      // precedence 3: system data path 
      // -----------------------------------------------------------
      ecal_default_paths.emplace_back(eCALDataSystemPath());
      return ecal_default_paths;
  }

  std::string checkForValidConfigFilePath(const std::string& config_file_)
  {
    // -----------------------------------------------------------
    // precedence 0: relative path to executable
    // -----------------------------------------------------------
    const std::string cwd_directory_path = cwdPath();

    std::vector<std::string> ecal_default_paths = getEcalDefaultPaths();
    ecal_default_paths.emplace(ecal_default_paths.begin(), cwd_directory_path);
    
    const std::string found_path = findValidConfigPath(ecal_default_paths, config_file_);

    // check in case user provided whole path
    if (found_path.empty())
    {
      return isValidConfigFilePath(config_file_, "") ? config_file_ : found_path;
    }

    return found_path + config_file_;
  }
}

namespace eCAL 
{
    void Configuration::InitFromFile(const std::string& yaml_path_)
    {
      const std::string yaml_path = checkForValidConfigFilePath(yaml_path_);
      if (!yaml_path.empty())
      {
#ifdef ECAL_CORE_CONFIGURATION
        eCAL::Config::YamlFileToConfig(yaml_path, *this);
        ecal_yaml_file_path = yaml_path;
#else
        std::cout << "Yaml file found at \"" << yaml_path << "\" but eCAL core configuration is not enabled." << "\n";
#endif
      }
      else
      {
        std::cout << "Specified yaml configuration path not valid:\"" << yaml_path_ << "\". Using default configuration." << "\n";
      }
    }

    void Configuration::InitFromConfig()
    {
      InitFromFile(g_default_ini_file);
    }

    Configuration::Configuration() = default;

    std::string Configuration::GetConfigurationFilePath() const
    {
      return ecal_yaml_file_path;
    }

    const Configuration& GetConfiguration()
    {
      return g_ecal_configuration;
    }

    const TransportLayer::Configuration& GetTransportLayerConfiguration()
    {
      return GetConfiguration().transport_layer;
    }

    const Registration::Configuration& GetRegistrationConfiguration()
    {
      return GetConfiguration().registration;
    }

    const Logging::Configuration& GetLoggingConfiguration()
    {
      return GetConfiguration().logging;
    }

    const Subscriber::Configuration& GetSubscriberConfiguration()
    {
      return GetConfiguration().subscriber;
    }

    const Publisher::Configuration& GetPublisherConfiguration()
    {
      return GetConfiguration().publisher;
    }

    const Time::Configuration& GetTimesyncConfiguration()
    {
      return GetConfiguration().timesync;
    }

    const Application::Configuration& GetApplicationConfiguration()
    {
      return GetConfiguration().application;
    }
}


// Utils definitions from former ecal_config_reader.cpp
namespace
{
  bool direxists(const std::string& path_)
  {
    const EcalUtils::Filesystem::FileStatus status(path_, EcalUtils::Filesystem::Current);
    return (status.IsOk() && (status.GetType() == EcalUtils::Filesystem::Type::Dir));
  }

  void createdir(const std::string& path_)
  {
    EcalUtils::Filesystem::MkDir(path_, EcalUtils::Filesystem::Current);
  }

}

namespace eCAL
{
  namespace Util
  {
    std::string GeteCALConfigPath()
    {
      // Check for first directory which contains the ini file.
      const std::vector<std::string> search_directories = getEcalDefaultPaths();

      return findValidConfigPath(search_directories, ECAL_DEFAULT_CFG);
    }

    std::string GeteCALHomePath()
    {
      std::string home_path;

#ifdef ECAL_OS_WINDOWS
      // check ECAL_HOME
      home_path = getEnvVar("ECAL_HOME");
      if (!home_path.empty())
      {
        if (*home_path.rbegin() != path_separator) home_path += path_separator;
      }
      if (!std::string(ECAL_HOME_PATH_WINDOWS).empty()) //-V815
      {
        home_path += path_separator;
        home_path += ECAL_HOME_PATH_WINDOWS;
      }
#endif /* ECAL_OS_WINDOWS */

#ifdef ECAL_OS_LINUX
      const char *hdir = nullptr;
      hdir = getenv("HOME");
      if (hdir == nullptr) {
        hdir = getpwuid(getuid())->pw_dir;
      }
      home_path += hdir;
      if (!std::string(ECAL_HOME_PATH_LINUX).empty())
      {
        home_path += "/";
        home_path += ECAL_HOME_PATH_LINUX;
      }
#endif /* ECAL_OS_LINUX */

      // create if not exists
      if (!direxists(home_path))
      {
        createdir(home_path);
      }

      home_path += path_separator;
      return(home_path);
    }

    std::string GeteCALUserSettingsPath()
    {
      std::string settings_path;
#ifdef ECAL_OS_WINDOWS
      settings_path = GeteCALConfigPath();
#endif /* ECAL_OS_WINDOWS */

#ifdef ECAL_OS_LINUX
      settings_path = GeteCALHomePath();
#endif /* ECAL_OS_LINUX */
      settings_path += std::string(ECAL_SETTINGS_PATH);

      if (!direxists(settings_path))
      {
        createdir(settings_path);
      }

      settings_path += path_separator;
      return(settings_path);
    }

    std::string GeteCALLogPath()
    {
      std::string log_path;
#ifdef ECAL_OS_WINDOWS
      log_path = GeteCALConfigPath();
#endif /* ECAL_OS_WINDOWS */

#ifdef ECAL_OS_LINUX
      log_path = GeteCALHomePath();
#endif /* ECAL_OS_LINUX */

      log_path += std::string(ECAL_LOG_PATH);

      if (!direxists(log_path))
      {
        createdir(log_path);
      }

      log_path += path_separator;
      return(log_path);
    }

    std::string GeteCALActiveIniFile()
    {
      std::string ini_file = GeteCALConfigPath();
      ini_file += ECAL_DEFAULT_CFG;
      return ini_file;
    }

    std::string GeteCALDefaultIniFile()
    {
      return GeteCALActiveIniFile();
    }
  
  }
}  