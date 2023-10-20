#include "file_entry.hpp"

namespace app {
    FileEntry::FileEntry(const DirEntry& entry)
        : entry(entry)
        , size(std::filesystem::file_size(entry)) {
    }
}  // namespace app