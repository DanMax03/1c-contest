#pragma once

#include <array>
#include <vector>
#include <filesystem>

namespace app {
    struct FileEntry;

    using FileSize = std::uintmax_t;
    using DirEntry = std::filesystem::directory_entry;
    using Path = std::filesystem::path;
    using DirPaths = std::array<Path, 2>;
    using DirIterator = std::filesystem::directory_iterator;
    using DirIterators = std::array<DirIterator, 2>;
    using FileEntries = std::vector<FileEntry>;
    using DirsFileEntries = std::array<FileEntries, 2>;
    using FilesystemError = std::filesystem::filesystem_error;
}  // namespace app