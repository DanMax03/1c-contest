#pragma once

#include "common_types.hpp"

#include <filesystem>

namespace app {
    struct FileEntry {
        explicit FileEntry(const DirEntry& entry);

        DirEntry entry;
        FileSize size;
    };

    bool operator<(const FileEntry& a, const FileEntry& b);

    bool isSameContents(std::ifstream& a_in, std::ifstream& b_in);
    // (is_same, real_similarity)
    std::pair<bool, float> compareFileEntriesContents(const FileEntry& a, const FileEntry& b, float similarity);
}  // namespace app