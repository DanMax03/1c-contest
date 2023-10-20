#pragma once

#include "common_types.hpp"

#include <filesystem>

namespace app {
    struct FileEntry {
        explicit FileEntry(const DirEntry& entry);

        DirEntry entry;
        FileSize size;
    };
}  // namespace app