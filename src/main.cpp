#include "common_types.hpp"
#include "input.hpp"
#include "logic.hpp"
#include "macros.hpp"

#include <array>
#include <cstdio>
#include <filesystem>

int main(int argc, char* argv[]) {
    using app::DirPaths;
    using app::FilesystemError;

    DirPaths dir_paths;
    float similarity = 0.f;

    TRY_MAIN(app::input::main(dir_paths, similarity, argc, argv));

    return app::logic::main(dir_paths, similarity);
}
