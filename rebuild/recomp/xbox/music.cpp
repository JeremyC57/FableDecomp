// PC music: with the PC files setting (pc_textures = <Fable: The Lost Chapters folder>) the music
// tracks the game streams from Data\Sound\*.wma (WMA 128 kbit/s) are played from the PC version's
// Data\Sound\<same name>.ogg (Ogg Vorbis) instead, for every track the PC folder has.
//
// The game's WMA music source (vtable 0x910A4C) wraps an in-memory WMA decoder (WMADEC) fed from
// the game's file stream of the track (opened by the same thread just before):
//   0x5E1D70  constructor   esi = this, (stream, const config*), ret 8
//   0x5E1B20  destructor    edi = this
//   0x5E19B0  read          ecx = this, ({int16* dst, channels}* out, frames) -> frames, ret 8
//   0x5E17E0  seek          ecx = this, (frame, ?) -> frame reached, ret 8 (the decoder seeks in ms)
//   +0x18 decoder object, +0x98 sample rate (44100), +0x9C length in ms
// The WMA source keeps running for everything but the read: seek and length stay the game's, the
// Ogg stream follows the seeks and is resampled to the WMA rate (the PC files are 48 kHz).
// FABLE_DISABLE=pcmusic keeps the WMA music.
#include "settings.hpp"
#include "xhost.hpp"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#define STB_VORBIS_NO_PUSHDATA_API
#include "third_party/stb_vorbis.c"

using namespace xb;
namespace fs = std::filesystem;

namespace {

struct Track {
    stb_vorbis* v = nullptr;
    int channels = 2;
    double step = 1.0;            // source frames per output frame
    std::vector<float> src;       // decoded frames (interleaved, `channels` wide) from frame `base`
    uint64_t base = 0;            // source frame index of src[0]
    double pos = 0.0;             // read position in source frames
    bool eof = false;
    ~Track() {
        if (v) stb_vorbis_close(v);
    }
};

std::mutex g_lock;
std::unordered_map<uint32_t, std::unique_ptr<Track>> g_tracks;  // guest music source -> Ogg stream

std::string upper(std::string s) {
    for (char& ch : s) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    return s;
}

// <pc>/data/Sound/<stem>.ogg, matched case-insensitively (OAKVALE.OGG, Intro.ogg, ...).
std::string pcTrack(const std::string& wmaPath) {
    const std::string pc = settings().pcTextures;
    if (pc.empty()) return {};
    const fs::path w(wmaPath);
    if (upper(w.extension().string()) != ".WMA") return {};
    const std::string want = upper(w.stem().string()) + ".OGG";
    fs::path dir = pc;
    for (const char* part : {"data", "sound"}) {
        std::error_code ec;
        bool found = false;
        for (const auto& e : fs::directory_iterator(dir, ec))
            if (upper(e.path().filename().string()) == upper(part)) {
                dir = e.path();
                found = true;
                break;
            }
        if (!found) return {};
    }
    std::error_code ec;
    for (const auto& e : fs::directory_iterator(dir, ec))
        if (upper(e.path().filename().string()) == want) return e.path().string();
    return {};
}

// Makes src hold source frames [i - 1, i + 2] for the read position; false at the end.
bool ensure(Track& t, uint64_t last) {
    while (t.base + t.src.size() / t.channels <= last) {
        if (t.eof) return false;
        float buf[4096];
        const int n = stb_vorbis_get_samples_float_interleaved(t.v, t.channels, buf, 4096);
        if (n <= 0) {
            t.eof = true;
            return false;
        }
        t.src.insert(t.src.end(), buf, buf + n * t.channels);
    }
    return true;
}

float sampleAt(const Track& t, int64_t i, int ch) {
    const int64_t k = i - static_cast<int64_t>(t.base);
    const int64_t frames = static_cast<int64_t>(t.src.size()) / t.channels;
    if (frames == 0) return 0.0f;
    return t.src[static_cast<size_t>(std::clamp<int64_t>(k, 0, frames - 1)) * t.channels + ch];
}

int16_t toS16(float f) {
    const float s = std::nearbyint(std::clamp(f, -1.0f, 1.0f) * 32767.0f);
    return static_cast<int16_t>(s);
}

uint32_t readFrames(Track& t, uint32_t dst, uint32_t outCh, uint32_t frames) {
    uint32_t done = 0;
    for (; done < frames; ++done) {
        const int64_t i = static_cast<int64_t>(std::floor(t.pos));
        const float x = static_cast<float>(t.pos - static_cast<double>(i));
        if (!ensure(t, static_cast<uint64_t>(i + 2)) && t.base + t.src.size() / t.channels <= static_cast<uint64_t>(i)) break;
        float out[2];
        for (int ch = 0; ch < std::min(t.channels, 2); ++ch) {
            if (t.step == 1.0) {
                out[ch] = sampleAt(t, i, ch);
                continue;
            }
            // Catmull-Rom between frames i and i + 1.
            const float p0 = sampleAt(t, i - 1, ch), p1 = sampleAt(t, i, ch), p2 = sampleAt(t, i + 1, ch), p3 = sampleAt(t, i + 2, ch);
            out[ch] = p1 + 0.5f * x * (p2 - p0 + x * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3 + x * (3.0f * (p1 - p2) + p3 - p0)));
        }
        if (t.channels == 1) out[1] = out[0];
        if (outCh == 1) {
            wr16(dst, static_cast<uint16_t>(toS16(0.5f * (out[0] + out[1]))));
            dst += 2;
        } else {
            for (uint32_t ch = 0; ch < outCh; ++ch, dst += 2) wr16(dst, static_cast<uint16_t>(toS16(ch < 2 ? out[ch] : 0.0f)));
        }
        t.pos += t.step;
    }
    // Drop frames no longer needed (keep one behind the read position for the interpolation).
    const uint64_t keep = static_cast<uint64_t>(std::max(0.0, std::floor(t.pos) - 1.0));
    if (keep > t.base + 8192) {
        const uint64_t drop = std::min<uint64_t>(keep - t.base, t.src.size() / t.channels);
        t.src.erase(t.src.begin(), t.src.begin() + static_cast<std::ptrdiff_t>(drop * t.channels));
        t.base += drop;
    }
    return done;
}

// FABLE_MUSIC_DUMP=<file>: the music the game reads, as raw 16-bit PCM (debugging).
void dump(uint32_t dst, uint32_t ch, uint32_t frames) {
    static std::FILE* f = getenv("FABLE_MUSIC_DUMP") ? std::fopen(getenv("FABLE_MUSIC_DUMP"), "wb") : nullptr;
    if (f && static_cast<int32_t>(frames) > 0) std::fwrite(gp(dst), 2 * ch, frames, f);
}

}  // namespace

extern "C" {

void F_005E1D70_orig(Ctx* c);
void F_005E1B20_orig(Ctx* c);
void F_005E19B0_orig(Ctx* c);
void F_005E17E0_orig(Ctx* c);

void hle_MusicOpen(Ctx* c) {
    static const bool off = featureOff("pcmusic");
    const uint32_t self = c->esi;
    F_005E1D70_orig(c);
    const std::string path = lastWmaOpened();  // the game opens the track just before creating its source
    const uint32_t rate = rd32(self + 0x98);
    if (off || !rd32(self + 0x18) || !rate) {
        XLOG(1, "music: %s", path.c_str());
        return;
    }
    const std::string ogg = pcTrack(path);
    if (ogg.empty()) {
        XLOG(1, "music: %s (no PC track)", path.c_str());
        return;
    }
    int err = 0;
    stb_vorbis* v = stb_vorbis_open_filename(ogg.c_str(), &err, nullptr);
    if (!v) {
        XLOG(0, "music: %s: Ogg open failed (%d)", ogg.c_str(), err);
        return;
    }
    const stb_vorbis_info info = stb_vorbis_get_info(v);
    auto t = std::make_unique<Track>();
    t->v = v;
    t->channels = std::max(1, info.channels);
    t->step = static_cast<double>(info.sample_rate) / rate;
    XLOG(1, "music: %s -> %s (%u Hz, %d ch, played at %u Hz)", path.c_str(), ogg.c_str(), info.sample_rate, info.channels, rate);
    std::lock_guard<std::mutex> l(g_lock);
    g_tracks[self] = std::move(t);
}

void hle_MusicClose(Ctx* c) {
    {
        std::lock_guard<std::mutex> l(g_lock);
        g_tracks.erase(c->edi);
    }
    F_005E1B20_orig(c);
}

void hle_MusicRead(Ctx* c) {
    const uint32_t out = ARG(c, 0), frames = ARG(c, 1);
    const uint32_t dst = rd32(out), ch = std::max(1u, std::min(rd32(out + 4), 8u));
    std::unique_lock<std::mutex> l(g_lock);
    const auto it = g_tracks.find(c->ecx);
    if (it == g_tracks.end()) {
        l.unlock();
        F_005E19B0_orig(c);
        dump(dst, ch, c->eax);
        return;
    }
    c->eax = readFrames(*it->second, dst, ch, frames);
    c->esp += 4 + 8;
    dump(dst, ch, c->eax);
}

void hle_MusicSeek(Ctx* c) {
    const uint32_t self = c->ecx;
    F_005E17E0_orig(c);
    std::lock_guard<std::mutex> l(g_lock);
    const auto it = g_tracks.find(self);
    if (it == g_tracks.end() || static_cast<int32_t>(c->eax) < 0) return;
    Track& t = *it->second;
    const uint64_t frame = static_cast<uint64_t>(static_cast<int32_t>(c->eax) * t.step + 0.5);
    stb_vorbis_seek(t.v, static_cast<unsigned>(frame));
    t.src.clear();
    t.base = frame;
    t.pos = static_cast<double>(frame);
    t.eof = false;
}

}  // extern "C"
