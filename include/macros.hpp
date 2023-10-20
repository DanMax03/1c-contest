#pragma once

#define TRY_MAIN(exp) {                \
try {                                  \
    exp;                               \
}                                      \
catch (const FilesystemError& error) { \
    printf("%s\n", error.what());      \
    return error.code().value();       \
}                                      \
} (void)0
