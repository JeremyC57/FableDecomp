// The game disc image (XDVDFS, the Xbox disc file system): the files the game needs are
// extracted from it once, into a folder the game then runs from. Accepts an xiso (extract-xiso / xdvdfs output, file system
// at offset 0) and full dumps with the video partition in front (XGD1/2/3 redump images).
//
// XDVDFS: 2048-byte sectors; the volume descriptor is sector 32 of the game partition
// ("MICROSOFT*XBOX*MEDIA" at +0 and +0x7EC, root directory sector at +0x14, its size at +0x18).
// A directory is a binary search tree of entries, 4-byte aligned, that never cross a sector:
//   u16 left, u16 right (offsets in dwords from the start of the table, 0 = none),
//   u32 start sector, u32 size, u8 attributes (0x10 = directory), u8 name length, name.
#include "disc.hpp"

#include <fcntl.h>
#include <map>
#include <mutex>
#include <strings.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>
#include <cstring>

namespace xb::disc {
namespace {
constexpr uint64_t kSector = 2048;
const char kMagic[] = "MICROSOFT*XBOX*MEDIA";

int g_fd = -1;
uint64_t g_base = 0;  // byte offset of the game partition in the image
Entry g_root;
time_t g_time = 0;
std::mutex g_lock;
std::map<uint32_t, std::vector<Entry>> g_dirs;  // directory sector -> entries, sorted by name

bool readAt(void* buf, size_t len, uint64_t off) {
    auto* p = static_cast<uint8_t*>(buf);
    while (len) {
        const ssize_t n = pread(g_fd, p, len, static_cast<off_t>(off));
        if (n <= 0) return false;
        p += n, off += static_cast<uint64_t>(n), len -= static_cast<size_t>(n);
    }
    return true;
}

uint16_t le16(const uint8_t* p) { return static_cast<uint16_t>(p[0] | p[1] << 8); }
uint32_t le32(const uint8_t* p) { return p[0] | p[1] << 8 | p[2] << 16 | static_cast<uint32_t>(p[3]) << 24; }

// Walks the tree of one directory table (iteratively; a corrupt table cannot loop forever).
std::vector<Entry> readDir(const Entry& dir) {
    std::vector<Entry> out;
    if (!dir.size) return out;
    std::vector<uint8_t> t(dir.size);
    if (!readAt(t.data(), t.size(), g_base + dir.offset)) return out;
    std::vector<uint32_t> todo{0};
    std::vector<bool> seen(t.size() / 4 + 1);
    while (!todo.empty()) {
        const uint32_t o = todo.back();
        todo.pop_back();
        if (o + 14 > t.size() || seen[o / 4]) continue;
        seen[o / 4] = true;
        const uint8_t* e = &t[o];
        if (le16(e) == 0xFFFF) continue;  // empty directory / padding
        const uint8_t len = e[13];
        if (o + 14 + len > t.size()) continue;
        Entry x;
        x.sector = le32(e + 4);
        x.offset = x.sector * kSector;
        x.size = le32(e + 8);
        x.dir = (e[12] & 0x10) != 0;
        x.name.assign(reinterpret_cast<const char*>(e + 14), len);
        // Never let a name write outside the extraction folder.
        if (x.name.empty() || x.name == "." || x.name == ".." || x.name.find('/') != std::string::npos) continue;
        out.push_back(std::move(x));
        if (le16(e)) todo.push_back(le16(e) * 4u);
        if (le16(e + 2)) todo.push_back(le16(e + 2) * 4u);
    }
    std::sort(out.begin(), out.end(), [](const Entry& a, const Entry& b) { return strcasecmp(a.name.c_str(), b.name.c_str()) < 0; });
    return out;
}
}  // namespace

bool open(const std::string& path) {
    const int fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);
    if (fd < 0) return false;
    g_fd = fd;
    struct stat st;
    g_time = fstat(fd, &st) == 0 ? st.st_mtime : 0;
    // xiso, XGD1 (original Xbox discs), XGD2, XGD3.
    for (const uint64_t base : {0ull, 0x18300000ull, 0xFD90000ull, 0x2080000ull}) {
        uint8_t vd[kSector];
        if (!readAt(vd, sizeof vd, base + 32 * kSector)) continue;
        if (std::memcmp(vd, kMagic, 20) != 0 || std::memcmp(vd + 0x7EC, kMagic, 20) != 0) continue;
        g_base = base;
        g_root.dir = true;
        g_root.sector = le32(vd + 0x14);
        g_root.offset = g_root.sector * kSector;
        g_root.size = le32(vd + 0x18);
        std::printf("disc: %s (XDVDFS at 0x%llX)\n", path.c_str(), static_cast<unsigned long long>(base));
        return true;
    }
    ::close(fd);
    g_fd = -1;
    return false;
}

bool active() { return g_fd >= 0; }
time_t timestamp() { return g_time; }

const std::vector<Entry>& list(const Entry& dir) {
    std::lock_guard<std::mutex> l(g_lock);
    auto it = g_dirs.find(dir.sector);
    if (it == g_dirs.end()) it = g_dirs.emplace(dir.sector, readDir(dir)).first;
    return it->second;
}

bool find(const std::string& path, Entry& out) {
    Entry cur = g_root;
    size_t s = 0;
    while (s < path.size()) {
        while (s < path.size() && (path[s] == '\\' || path[s] == '/')) ++s;
        size_t e = path.find_first_of("\\/", s);
        if (e == std::string::npos) e = path.size();
        if (e == s) break;
        const std::string comp = path.substr(s, e - s);
        s = e;
        if (!cur.dir) return false;
        bool found = false;
        for (const Entry& x : list(cur))
            if (strcasecmp(x.name.c_str(), comp.c_str()) == 0) {
                cur = x;
                found = true;
                break;
            }
        if (!found) return false;
    }
    out = cur;
    return true;
}

int64_t read(const Entry& e, void* buf, size_t len, uint64_t off) {
    if (e.dir || off >= e.size) return 0;
    len = static_cast<size_t>(std::min<uint64_t>(len, e.size - off));
    return readAt(buf, len, g_base + e.offset + off) ? static_cast<int64_t>(len) : -1;
}

bool readFile(const std::string& path, std::vector<uint8_t>& out) {
    Entry e;
    if (!find(path, e) || e.dir) return false;
    out.resize(e.size);
    return read(e, out.data(), out.size(), 0) == static_cast<int64_t>(e.size);
}

namespace {
// Not needed to play: the dashboard updaters on the disc root.
bool skipped(const std::string& dir, const Entry& e) {
    return dir.empty() && !e.dir && (strcasecmp(e.name.c_str(), "dashupdate.xbe") == 0 || strcasecmp(e.name.c_str(), "update.xbe") == 0);
}
uint64_t totalSize(const Entry& dir, const std::string& rel) {
    uint64_t n = 0;
    for (const Entry& e : list(dir))
        if (!skipped(rel, e)) n += e.dir ? totalSize(e, rel + "/" + e.name) : e.size;
    return n;
}
bool extractDir(const Entry& dir, const std::string& dest, const std::string& rel, uint64_t& done, uint64_t total,
                const Progress& progress, std::vector<uint8_t>& buf) {
    mkdir((dest + rel).c_str(), 0755);
    for (const Entry& e : list(dir)) {
        if (skipped(rel, e)) continue;
        const std::string path = rel + "/" + e.name;
        if (e.dir) {
            if (!extractDir(e, dest, path, done, total, progress, buf)) return false;
            continue;
        }
        const int fd = ::open((dest + path).c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_CLOEXEC, 0644);
        if (fd < 0) {
            std::printf("disc: cannot create %s%s: %s\n", dest.c_str(), path.c_str(), std::strerror(errno));
            return false;
        }
        for (uint64_t off = 0; off < e.size;) {
            const int64_t n = read(e, buf.data(), buf.size(), off);
            if (n <= 0 || ::write(fd, buf.data(), static_cast<size_t>(n)) != n) {
                std::printf("disc: writing %s%s failed: %s\n", dest.c_str(), path.c_str(), n <= 0 ? "read error" : std::strerror(errno));
                ::close(fd);
                return false;
            }
            off += static_cast<uint64_t>(n);
            done += static_cast<uint64_t>(n);
            if (progress) progress(done, total, path);
        }
        ::close(fd);
    }
    return true;
}
std::string markerOf(const std::string& dest) { return dest + "/.extracted"; }
std::string stampOf(const std::string& image) {
    struct stat st;
    if (stat(image.c_str(), &st) != 0) return {};
    return std::to_string(static_cast<long long>(st.st_size)) + " " + std::to_string(static_cast<long long>(st.st_mtime)) + "\n";
}
}  // namespace

bool extracted(const std::string& image, const std::string& dest) {
    FILE* f = std::fopen(markerOf(dest).c_str(), "r");
    if (!f) return false;
    char line[64] = {};
    const bool ok = std::fgets(line, sizeof line, f) && line == stampOf(image);
    std::fclose(f);
    return ok;
}

bool extract(const std::string& image, const std::string& dest, const Progress& progress) {
    if (!active() && !open(image)) return false;
    std::remove(markerOf(dest).c_str());
    mkdir(dest.c_str(), 0755);
    uint64_t done = 0;
    const uint64_t total = totalSize(g_root, "");
    std::vector<uint8_t> buf(4 << 20);
    if (!extractDir(g_root, dest, "", done, total, progress, buf)) return false;
    FILE* f = std::fopen(markerOf(dest).c_str(), "w");
    if (!f) return false;
    std::fputs(stampOf(image).c_str(), f);
    std::fclose(f);
    return true;
}

}  // namespace xb::disc
