#include "file_entry.hpp"

#include <climits>
#include <fstream>
#include <cstring>

namespace app {
    FileEntry::FileEntry(const DirEntry& entry)
        : entry(entry)
        , size(std::filesystem::file_size(entry)) {
    }

    bool operator<(const FileEntry& a, const FileEntry& b) {
        return a.size < b.size;
    }

    bool isSameContents(std::ifstream& a_in, std::ifstream& b_in/*, std::streamsize size*/) {
        char a_ch;
        char b_ch;

        while (!a_in.eof()) {
            a_in >> a_ch;
            b_in >> b_ch;

            if (a_ch != b_ch) {
                return false;
            }
        }

        return true;

//        static const std::streamsize chunk_size = 4096;
//
//        char a_chunk[chunk_size];
//        char b_chunk[chunk_size];
//
//        const auto full_chunk_count = size / chunk_size;
//
//        for (int i = 0; i < full_chunk_count; ++i) {
//            a_in.read(a_chunk, chunk_size);
//            b_in.read(b_chunk, chunk_size);
//
//            if (std::memcmp(a_chunk, b_chunk, chunk_size) == 0) {
//                return false;
//            }
//        }
//
//        const auto chunk_rem = size - chunk_size * full_chunk_count;
//
//        a_in.read(a_chunk, chunk_rem);
//        b_in.read(b_chunk, chunk_rem);
//
//        return std::memcmp(a_chunk, b_chunk, chunk_size);
    }

    std::pair<bool, float> compareFileEntriesContents(const FileEntry& a, const FileEntry& b, const float similarity) {
        // similarity argument is used to knock out comparison as soon as we find
        // that files are more unsimilar than (1 - similarity)
        static const std::streamsize chunk_size = 4096;
        static char a_chunk[chunk_size];
        static char b_chunk[chunk_size];

        std::ifstream a_in(a.entry.path().c_str(), std::ios::in | std::ios::binary);
        std::ifstream b_in(a.entry.path().c_str(), std::ios::in | std::ios::binary);

        // todo: more efficient method for reading data?
        a_in.rdbuf()->pubsetbuf(a_chunk, chunk_size);
        b_in.rdbuf()->pubsetbuf(b_chunk, chunk_size);

        bool is_same = a.size == b.size;

        if (is_same) {
            is_same = isSameContents(a_in, b_in/*, static_cast<std::streamsize>(a.size)*/);

            if (is_same) {
                return {is_same, 1.f};
            }

            a_in.clear();
            a_in.seekg(0, std::ios::beg);
        }

        const FileEntry& min_fentry = a.size < b.size ? a : b;
        const FileEntry& max_fentry = a.size < b.size ? b : a;
        const auto min_size = min_fentry.size;
        const auto max_size = max_fentry.size;

        std::vector<std::vector<ssize_t>> leven(min_size + 1, std::vector<ssize_t>(max_size + 1));

        for (ssize_t i = 0; i <= min_size; ++i) {
            leven[i][0] = i;
        }

        for (ssize_t j = 1; j <= max_size; ++j) {
            leven[0][j] = j;
        }

        for (ssize_t i = 1; i <= min_size; ++i) {
            char a_ch;
            char b_ch;
            a_in >> a_ch;

            b_in.clear();
            b_in.seekg(0, std::ios::beg);

            for (ssize_t j = 1; j <= max_size; ++j) {
                leven[i][j] = std::min(leven[i][j - 1], leven[i - 1][j]) + 1;

                b_in >> b_ch;

                leven[i][j] = std::min(leven[i][j], leven[i - 1][j - 1] + (a_ch != b_ch));

                // todo: use similarity argument
            }
        }

        ssize_t dist = leven[min_size][max_size];
        auto real_similarity = static_cast<float>(static_cast<double>(dist) / static_cast<double>(max_size));

        if (real_similarity < similarity) {
            return {is_same, 0.f};
        }

        return {is_same, real_similarity};
    }
}  // namespace app