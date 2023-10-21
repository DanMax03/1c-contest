#include "logic.hpp"

#include "common_types.hpp"
#include "file_entry.hpp"
#include "macros.hpp"

#include <cstdio>
#include <vector>

namespace {
    struct TableEntry {
        int dir2_index{-1};
        bool is_same{false};
        float real_similarity{0.f};
    };

    // Mapping from dir1 files to dir2 files
    using Table = std::vector<std::vector<TableEntry>>;

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

        std::sort(entries.begin(), entries.end());
    }

    void outputOnlyOneDirFiles(const app::DirPaths& dir_paths,
                               const app::FileEntries& dir1_file_entries,
                               const app::FileEntries& dir2_file_entries) {
        for (int i = 0; i < 2; ++i) {
            const auto& file_entries = i == 0 ? dir1_file_entries : dir2_file_entries;
            const std::string order = i == 0 ? "first" : "second";

            printf("Files which are only in the \"");
            printf("%s", dir_paths[i].c_str());
            printf("\" directory:\n");

            for (const auto& fentry : file_entries) {
                printf("%s\n", fentry.entry.path().c_str());
            }
        }
    }

    void compareFileEntries(const app::DirPaths& dir_paths,
                            const app::DirsFileEntries& dirs_file_entries,
                            const float similarity) {
        using app::FileEntry;
        if (dirs_file_entries[0].empty() || dirs_file_entries[1].empty()) {
            outputOnlyOneDirFiles(dir_paths, dirs_file_entries[0], dirs_file_entries[1]);
        }

        auto get_max_possible_similarity = [](const FileEntry& a, const FileEntry& b) {
            auto min_size = static_cast<float>(std::min(a.size, b.size));
            return min_size / (static_cast<float>(a.size + b.size) - min_size);
        };

        Table table(dirs_file_entries[0].size());

        for (int i = 0; i < dirs_file_entries[0].size(); ++i) {
            const auto& dir1_fentry = dirs_file_entries[0][i];
            auto& row = table[i];

            // todo: remove the cycle, use binsearch
            for (int j = 0; j < dirs_file_entries[1].size(); ++j) {
                const auto& dir2_fentry = dirs_file_entries[1][j];

                auto max_possible_similarity = get_max_possible_similarity(dir1_fentry, dir2_fentry);

                if (max_possible_similarity < similarity) {
                    if (dir1_fentry.size < dir2_fentry.size) {
                        break;
                    } else {
                        continue;
                    }
                }

                auto [is_same, real_similarity] = app::compareFileEntriesContents(dir1_fentry, dir2_fentry, similarity);

                if (is_same || real_similarity >= similarity) {
                    row.emplace_back(j, is_same, real_similarity);
                }
            }
        }
    }
}  // namespace

namespace app::logic {
    int main(const DirPaths& dir_paths, const float similarity) {
        DirIterators iters;

        TRY_MAIN(initDirIterators(iters, dir_paths));

        DirsFileEntries dirs_file_entries;

        for (int i = 0; i < dirs_file_entries.size(); ++i) {
            gatherFileEntries(iters[i], dirs_file_entries[i]);
        }

        compareFileEntries(dir_paths, dirs_file_entries, similarity);

        return 0;
    }
}  // namespace app::logic