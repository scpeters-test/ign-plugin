/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#include "ignition/common/Plugin.hh"
#include "ignition/common/PluginInfo.hh"
#include "ignition/common/Console.hh"
#include "PluginUtils.hh"

namespace ignition
{
  namespace common
  {
    class PluginPrivate
    {
      /// \brief Map from interface names to their locations within the plugin
      /// instance
      public: Plugin::InterfaceMap interfaces;

      /// \brief shared_ptr which manages the lifecycle of the plugin instance.
      std::shared_ptr<void> pluginInstancePtr;

      /// \brief Clear this PluginPtrPrivate without invaliding any map entry
      /// iterators.
      public: void Clear()
              {
                this->pluginInstancePtr.reset();
                this->interfaces.clear();
              }

      /// \brief Initialize this PluginPtrPrivate using some PluginInfo instance
      public: void Initialize(const PluginInfo *_info)
              {
                this->Clear();

                if (!_info)
                  return;

                this->pluginInstancePtr =
                    std::shared_ptr<void>(_info->factory(), _info->deleter);

                if (this->pluginInstancePtr)
                {
                  for (const auto &entry : _info->interfaces)
                  {
                    // entry.first:  name of the interface
                    // entry.second: function which casts the pluginInstance
                    //               pointer to the correct location of the
                    //               interface within the plugin
                    this->interfaces[entry.first] =
                        entry.second(this->pluginInstancePtr.get());
                  }
                }
              }
    };

    //////////////////////////////////////////////////
    bool Plugin::HasInterface(
        const std::string &_interfaceName) const
    {
      const std::string interfaceName = NormalizeName(_interfaceName);
      return (this->dataPtr->interfaces.count(interfaceName) != 0);
    }

    //////////////////////////////////////////////////
    Plugin::Plugin()
      : dataPtr(new PluginPrivate)
    {
      // Do nothing
    }

    //////////////////////////////////////////////////
    Plugin::Plugin(const PluginInfo &_info) : Plugin()
    {
      this->dataPtr->Initialize(&_info);
    }

    //////////////////////////////////////////////////
    void *Plugin::PrivateGetInterface(
        const std::string &_interfaceName) const
    {
      const std::string interfaceName = NormalizeName(_interfaceName);
      const auto &it = this->dataPtr->interfaces.find(interfaceName);
      if (this->dataPtr->interfaces.end() == it)
        return nullptr;

      return it->second;
    }

    //////////////////////////////////////////////////
    Plugin::~Plugin()
    {
      // Do nothing. We need this definition to be in a source file so that
      // the destructor of PluginPrivate is visible to std::unique_ptr.
    }
  }
}
