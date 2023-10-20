#include "logic.hpp"

#include "common_types.hpp"
#include "file_entry.hpp"
#include "macros.hpp"

#include <cstdio>
#include <vector>

namespace {
    void initDirIterators(app::DirIterators& iters, const app::DirPaths& dir_paths) {
        const auto dir_options = std::filesystem::directory_options::follow_directory_symlink;

        for (int i = 0; i < iters.size(); ++i) {
            iters[i] = app::DirIterator(dir_paths[i], dir_options);
        }
    }

    void gatherFileEntries(app::DirIterator& iter, app::FileEntries& entries) {
        using std::filesystem::perms;

        for (const auto& entry : iter) {
            if (!entry.is_regular_file()) {
                continue;
            }

            const auto& entry_perms = entry.status().permissions();

            static auto has_permission = [](perms p, perms test_perm) -> bool {
                return (p & test_perm) != perms::none;
            };

            if (has_permission(entry_perms, perms::owner_read)
                && has_permission(entry_perms, perms::others_read)) {
                entries.emplace_back(entry);
            }
        }

        std::sort(entries.begin(), entries.end(), [](const app::FileEntry& fentry1, const app::FileEntry& fentry2) {
            return fentry1.size < fentry2.size;
        });
    }
}  // namespace

namespace app::logic {
    int main(const DirPaths& dir_paths, float similarity) {
        DirIterators iters;

        TRY_MAIN(initDirIterators(iters, dir_paths));

        DirsFileEntries dirs_file_entries;

        for (int i = 0; i < dirs_file_entries.size(); ++i) {
            gatherFileEntries(iters[i], dirs_file_entries[i]);
        }

        // compareFileEntries(dirs_file_entries);

        return 0;
    }
}  // namespace app::logic