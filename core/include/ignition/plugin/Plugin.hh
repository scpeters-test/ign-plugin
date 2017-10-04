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


#ifndef IGNITION_COMMON_PLUGIN_HH_
#define IGNITION_COMMON_PLUGIN_HH_

#include <memory>
#include <map>
#include <string>

namespace ignition
{
  namespace common
  {
    // Forward declarations
    struct PluginInfo;
    class PluginPrivate;
    class PluginLoader;

    class Plugin
    {
      // -------------------- Public API ---------------------

      /// \brief Get an interface of the specified type. Note that this function
      /// only works when the Interface type is specialized using the macro
      /// IGN_COMMON_SPECIALIZE_INTERFACE. For more general  interfaces which do
      /// not meet this condition, use Interface<I>(_interfaceName).
      public: template <class I>
      I *Interface();

      /// \brief const-qualified version of Interface<I>()
      public: template <class I>
      const I * Interface() const;

      /// \brief Get an interface with the given name, casted to the specified
      /// class type.
      public: template <class I>
      I *Interface(const std::string &_interfaceName);

      /// \brief const-qualified version of Interface<I>()
      public: template <class I>
      const I * Interface(const std::string &_interfaceName) const;

      public: using InterfaceMap = std::map<std::string, void*>;

      /// \brief Returns true if this Plugin has the specified type of
      /// interface. Note that this function only works when the Interface type
      /// is specialized using the macro IGN_COMMON_SPECIALIZE_INTERFACE. For
      /// more general interfaces which do not meet this condition, use
      /// Interface<Interface>(_interfaceName).
      public: template <class Interface>
              bool HasInterface() const;

      /// \brief Returns true if this PluginPtr has the specified type of
      /// interface.
      public: bool HasInterface(const std::string &_interfaceName) const;

      // -------------------- Private API -----------------------
      friend class PluginLoader;

      /// \brief Default constructor. This is kept private to ensure that
      /// Plugins are always managed by a PluginLoader object
      private: Plugin();

      /// \brief Construct a plugin using plugin info from loader
      private: explicit Plugin(const PluginInfo &_info);

      /// \brief Type-agnostic retriever for interfaces
      private: void *PrivateGetInterface(
                  const std::string &_interfaceName) const;

      /// \brief PIMPL pointer to the implementation of this class.
      private: const std::unique_ptr<PluginPrivate> dataPtr;

      /// \brief Virtual destructor
      public: virtual ~Plugin();
    };
  }
}

#include "ignition/common/detail/Plugin.hh"

#endif
