#include "input.hpp"

#include <stdexcept>

namespace {
    const std::string help_str =
        "Usage: app DIR1_PATH DIR2_PATH SIMILARITY_PARAM\n"
        "Descripton:\n"
        "An app for Tony Stark\n"
        "Arguments:\n"
        "DIR1_PATH, DIR2_PATH - directory paths from the app takes binary files\n"
        "SIMILARITY_PARAM - binary files' similarity parameter from 0 to 100";
}  // namespace

namespace app::input {
    void main(DirPaths& dir_paths, float& similarity, int argc, char* argv[]) {
        if (argc == 1) {
            throw std::filesystem::filesystem_error(help_str, std::error_code(0, std::generic_category()));
        }

        if (argc != 4) {
            throw std::filesystem::filesystem_error("Expected 3 arguments, got " + std::to_string(argc - 1),
                                                    std::error_code(1, std::generic_category()));
        }

        using std::filesystem::exists;
        using std::filesystem::is_directory;

        for (int i = 0; i < 2; ++i) {
            dir_paths[i] = Path(argv[i + 1]);

            if (!exists(dir_paths[i])) {
                throw std::filesystem::filesystem_error("Given path does not exist",
                                                        std::error_code(1, std::system_category()));
            }

            if (!is_directory(dir_paths[i])) {
                throw std::filesystem::filesystem_error("Given path is not a directory path",
                                                        std::error_code(1, std::system_category()));
            }
        }

        int similarity_percent = static_cast<int>(std::strtol(argv[3], nullptr, 10));

        if (similarity_percent < 0 || similarity_percent > 100) {
            throw std::filesystem::filesystem_error("The third argument must be a non-negative number from 0 to 100",
                                                    std::error_code(1, std::system_category()));
        }

        similarity = static_cast<float>(similarity_percent) / 100.f;
    }
}  // namespace app::input