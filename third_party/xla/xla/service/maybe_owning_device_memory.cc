/* Copyright 2018 The OpenXLA Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "xla/service/maybe_owning_device_memory.h"

#include <optional>
#include <utility>
#include <variant>

#include "xla/stream_executor/device_memory.h"
#include "xla/stream_executor/device_memory_allocator.h"

namespace xla {

tensorflow::se::DeviceMemoryBase MaybeOwningDeviceMemory::AsDeviceMemoryBase()
    const {
  if (HasOwnership()) {
    return *std::get<tensorflow::se::OwningDeviceMemory>(mem_);
  }
  return std::get<tensorflow::se::DeviceMemoryBase>(mem_);
}

bool MaybeOwningDeviceMemory::HasOwnership() const {
  return std::holds_alternative<tensorflow::se::OwningDeviceMemory>(mem_);
}

std::optional<tensorflow::se::OwningDeviceMemory>
MaybeOwningDeviceMemory::Release() {
  if (!HasOwnership()) {
    return {};
  }
  return std::move(std::get<tensorflow::se::OwningDeviceMemory>(mem_));
}

const tensorflow::se::OwningDeviceMemory*
MaybeOwningDeviceMemory::AsOwningDeviceMemory() const {
  return HasOwnership() ? &std::get<tensorflow::se::OwningDeviceMemory>(mem_)
                        : nullptr;
}

}  // namespace xla
