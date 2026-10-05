// The game disc image (.iso / .xiso, XDVDFS): reading it and extracting it. See disc.cpp.
#pragma once

#include <algorithm>
#include <cstdint>
#include <ctime>
#include <functional>
#include <string>
#include <vector>

namespace xb::disc {

struct Entry {
    uint64_t offset = 0, size = 0;  // bytes, within the game partition
    uint32_t sector = 0;
    bool dir = false;
    std::string name;
};

bool open(const std::string& imagePath);  // false: not an Xbox disc image
bool active();
time_t timestamp();  // the image file's modification time, used for every entry
// Path relative to the disc root ("\\Data\\x.big"; "" is the root), matched without case.
bool find(const std::string& path, Entry& out);
const std::vector<Entry>& list(const Entry& dir);
int64_t read(const Entry& file, void* buf, size_t len, uint64_t offset);
bool readFile(const std::string& path, std::vector<uint8_t>& out);

// Extracts the files the game needs (everything but the dashboard updaters) into `dest`,
// then writes a marker naming the image (size, time) so later runs skip the work.
using Progress = std::function<void(uint64_t done, uint64_t total, const std::string& file)>;
bool extracted(const std::string& image, const std::string& dest);
bool extract(const std::string& image, const std::string& dest, const Progress& progress);

}  // namespace xb::disc
