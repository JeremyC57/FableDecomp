// PC texture replacement: see pc_textures.hpp.
#include "pc_textures.hpp"

#include "settings.hpp"
#include "xhost.hpp"

#include "fable/assets/BigArchive.hpp"
#include "fable/assets/Texture.hpp"

#include <zlib.h>

#include <algorithm>
#include <atomic>
#include <cctype>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>
#include <unordered_map>

namespace fs = std::filesystem;
namespace fa = fable::assets;

namespace xb::pctex {
namespace {

std::atomic<bool> g_ready{false};
std::atomic<uint32_t> g_gen{0};
std::unordered_map<uint64_t, std::string> g_names;  // key(hash, size) -> Xbox entry name
std::vector<std::unique_ptr<fa::BigArchive>> g_pc;   // PC banks (textures.big, frontend.big)
std::unordered_map<std::string, const fa::BigEntry*> g_pcByBase;  // upper-case file name -> entry
std::unordered_map<std::string, const fa::BigEntry*> g_pcByName;
std::unordered_map<const fa::BigEntry*, fa::BigArchive*> g_owner;

uint64_t key(uint64_t hash, uint32_t size) { return hash * 0x9E3779B97F4A7C15ull ^ size; }

std::string upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}
// "[\DEV\...\NAME.TGA]" -> "NAME.TGA"
std::string baseName(const std::string& n) {
    std::string s = n;
    if (!s.empty() && s.front() == '[') s.erase(0, 1);
    if (!s.empty() && s.back() == ']') s.pop_back();
    const auto p = s.find_last_of("\\/");
    return upper(p == std::string::npos ? s : s.substr(p + 1));
}

// A path under `base` matched case-insensitively component by component (Android folders keep
// the PC install's mixed case: Data/graphics/pc/textures.big).
std::optional<fs::path> findPath(const fs::path& base, std::initializer_list<const char*> parts) {
    fs::path cur = base;
    for (const char* part : parts) {
        std::error_code ec;
        bool found = false;
        for (const auto& e : fs::directory_iterator(cur, ec)) {
            if (upper(e.path().filename().string()) == upper(part)) {
                cur = e.path();
                found = true;
                break;
            }
        }
        if (!found) return std::nullopt;
    }
    return cur;
}

// "biz!" + u32 size + zlib stream -> plain bank file.
bool inflateBiz(const fs::path& in, const fs::path& out) {
    std::ifstream f(in, std::ios::binary);
    char hdr[8];
    if (!f.read(hdr, 8) || std::memcmp(hdr, "biz!", 4) != 0) return false;
    std::ofstream o(out, std::ios::binary | std::ios::trunc);
    if (!o) return false;
    z_stream z{};
    if (inflateInit(&z) != Z_OK) return false;
    std::vector<unsigned char> ib(1 << 20), ob(1 << 20);
    int r = Z_OK;
    while (r != Z_STREAM_END) {
        f.read(reinterpret_cast<char*>(ib.data()), static_cast<std::streamsize>(ib.size()));
        z.next_in = ib.data();
        z.avail_in = static_cast<uInt>(f.gcount());
        if (!z.avail_in) break;
        do {
            z.next_out = ob.data();
            z.avail_out = static_cast<uInt>(ob.size());
            r = inflate(&z, Z_NO_FLUSH);
            if (r != Z_OK && r != Z_STREAM_END) { inflateEnd(&z); return false; }
            o.write(reinterpret_cast<const char*>(ob.data()), static_cast<std::streamsize>(ob.size() - z.avail_out));
        } while (z.avail_out == 0 && r != Z_STREAM_END);
    }
    inflateEnd(&z);
    return r == Z_STREAM_END;
}

bool isDxt(uint32_t f) { return f == static_cast<uint32_t>(fa::PixelFormat::Dxt1) || f == static_cast<uint32_t>(fa::PixelFormat::Dxt3); }

void indexXbox(const fs::path& bank, std::FILE* cache, size_t& n) {
    auto a = fa::BigArchive::open(bank);
    for (const auto& sb : a.subBanks())
        for (const auto& e : sb.entries) {
            if (e.info.size() != fa::TextureInfo::kSize) continue;
            try {
                const auto info = fa::TextureInfo::parse(e.info);
                if (!isDxt(info.formatIndex)) continue;
                const auto payload = a.readPayload(e);
                const auto mip0 = fa::decodeMip0(info, payload);
                const uint64_t h = hashBytes(reinterpret_cast<const uint8_t*>(mip0.data()), mip0.size());
                g_names[key(h, static_cast<uint32_t>(mip0.size()))] = e.name;
                std::fprintf(cache, "%016llx %zu %s\n", static_cast<unsigned long long>(h), mip0.size(), e.name.c_str());
                ++n;
            } catch (const std::exception&) {
            }
        }
}

void worker(std::string game, std::string cacheDir, std::string pcDir) {
    try {
        // PC banks first: without them there is nothing to do.
        for (const char* name : {"textures.big", "frontend.big"}) {
            const auto p = findPath(pcDir, {"data", "graphics", "pc", name});
            if (!p) continue;
            g_pc.push_back(std::make_unique<fa::BigArchive>(fa::BigArchive::open(*p)));
            for (const auto& sb : g_pc.back()->subBanks())
                for (const auto& e : sb.entries) {
                    if (e.info.size() != fa::TextureInfo::kSize) continue;
                    g_pcByName.emplace(upper(e.name), &e);
                    g_pcByBase.emplace(baseName(e.name), &e);
                    g_owner[&e] = g_pc.back().get();
                }
        }
        if (g_pc.empty()) {
            XLOG(0, "PC textures: no data/graphics/pc/textures.big under %s", pcDir.c_str());
            return;
        }
        // Xbox name index, cached across runs.
        const fs::path cachePath = fs::path(cacheDir) / "pc_texture_names_v2.txt";
        size_t n = 0;
        if (std::FILE* f = std::fopen(cachePath.string().c_str(), "r")) {
            char line[1024];
            while (std::fgets(line, sizeof line, f)) {
                unsigned long long h;
                size_t sz;
                int off = 0;
                if (std::sscanf(line, "%llx %zu %n", &h, &sz, &off) == 2 && off > 0) {
                    std::string name = line + off;
                    while (!name.empty() && (name.back() == '\n' || name.back() == '\r')) name.pop_back();
                    g_names[key(h, static_cast<uint32_t>(sz))] = name;
                    ++n;
                }
            }
            std::fclose(f);
        }
        if (!n) {
            const fs::path tmp = cachePath.string() + ".tmp";
            std::FILE* out = std::fopen(tmp.string().c_str(), "w");
            if (!out) return;
            for (const char* name : {"textures.biz", "frontend.biz"}) {
                const auto biz = findPath(game, {"data", "graphics", "xbox", name});
                if (!biz) continue;
                const fs::path big = fs::path(cacheDir) / (std::string("xbox_") + name + ".big");
                XLOG(0, "PC textures: indexing %s (first run only)", biz->string().c_str());
                if (inflateBiz(*biz, big)) indexXbox(big, out, n);
                std::error_code ec;
                fs::remove(big, ec);
            }
            std::fclose(out);
            std::error_code ec;
            fs::rename(tmp, cachePath, ec);
        }
        XLOG(0, "PC textures: %zu Xbox textures indexed, %zu PC textures available", n, g_pcByName.size());
        g_ready = true;
        ++g_gen;
    } catch (const std::exception& e) {
        XLOG(0, "PC textures: disabled (%s)", e.what());
    }
}

}  // namespace

uint64_t hashBytes(const uint8_t* p, size_t n) {
    uint64_t h = 0xCBF29CE484222325ull ^ n;
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        uint64_t w;
        std::memcpy(&w, p + i, 8);
        h = (h ^ w) * 0x100000001B3ull;
        h ^= h >> 29;
    }
    for (; i < n; ++i) h = (h ^ p[i]) * 0x100000001B3ull;
    return h;
}

uint32_t generation() { return g_gen.load(std::memory_order_acquire); }

void init(const char* gameDir, const char* cacheDir) {
    const std::string pc = settings().pcTextures;
    if (pc.empty() || !gameDir || !cacheDir) return;
    std::thread(worker, std::string(gameDir), std::string(cacheDir), pc).detach();
}

bool lookup(uint64_t hash, uint32_t mip0Bytes, uint32_t w, uint32_t h, Replacement& out) {
    if (!g_ready.load(std::memory_order_acquire)) return false;
    static std::atomic<uint64_t> asked{0}, named{0};
    const auto it = g_names.find(key(hash, mip0Bytes));
    ++asked;
    if (it != g_names.end()) ++named;
    if (asked % 500 == 0) XLOG(1, "PC textures: %llu DXT uploads looked up, %llu matched a bank entry", static_cast<unsigned long long>(asked.load()),
                               static_cast<unsigned long long>(named.load()));
    if (it == g_names.end()) {
        static const bool logMiss = getenv("FABLE_PCTEX_MISS") != nullptr;  // debugging: what is not matched
        static int nMiss = 0;
        if (logMiss && nMiss < 200) {
            ++nMiss;
            XLOG(0, "PC textures: no match for %ux%u (%u bytes, hash %016llx)", w, h, mip0Bytes, static_cast<unsigned long long>(hash));
        }
        return false;
    }
    auto pe = g_pcByName.find(upper(it->second));
    const fa::BigEntry* e = pe != g_pcByName.end() ? pe->second : nullptr;
    if (!e) {
        const auto pb = g_pcByBase.find(baseName(it->second));
        if (pb == g_pcByBase.end()) return false;
        e = pb->second;
    }
    try {
        const auto info = fa::TextureInfo::parse(e->info);
        const uint32_t pw = info.allocWidth, ph = info.allocHeight;
        // Only a strictly larger image of the same shape, up to 2048 texels a side.
        if (pw * ph <= w * h || pw > 2048 || ph > 2048 || static_cast<uint64_t>(pw) * h != static_cast<uint64_t>(ph) * w) return false;
        const auto payload = g_owner[e]->readPayload(*e);
        const auto mip0 = fa::decodeMip0(info, payload);
        auto rgba = fa::toRgba8(info.format(), pw, ph, mip0);
        if (rgba.size() < static_cast<size_t>(pw) * ph * 4) return false;
        for (size_t i = 0; i + 3 < rgba.size(); i += 4) std::swap(rgba[i], rgba[i + 2]);  // RGBA -> BGRA
        out.w = pw;
        out.h = ph;
        out.bgra = std::move(rgba);
        static std::atomic<int> logged{0};
        if (logged++ < 5) XLOG(1, "PC textures: %s %ux%u -> %ux%u", it->second.c_str(), w, h, pw, ph);
        return true;
    } catch (const std::exception&) {
        return false;
    }
}

} // namespace xb::pctex
