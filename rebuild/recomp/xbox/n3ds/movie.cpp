// New 3DS movies: Fable's XMV cutscenes, converted once by convert_movies.py to H.264 + IMA ADPCM
// (.fmv in sdmc:/3ds/fable/movies), play through the hardware video decoder (MVDSTD) straight into
// the top screen's framebuffer, with the sound on the DSP (ndsp). The game never decodes them: its
// open of the .xmv waits here until the movie ends (or A / B / START skips it), then sees no file.
#include "../xhost.hpp"

#include <3ds.h>

#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <mutex>
#include <string>
#include <sys/stat.h>
#include <vector>

namespace xb {
std::mutex& screenLock();  // platform.cpp: the screens belong to whoever holds this

namespace {
struct FmvHeader {
    char magic[4];
    uint32_t version, width, height, fpsNum, fpsDen, frames, audioRate, audioChannels, blockAlign;
    uint32_t tableOff, videoOff, videoSize, audioOff, audioSize, reserved;
};
static_assert(sizeof(FmvHeader) == 64, "FMV3 header");

// ---- IMA ADPCM (WAV blocks: per channel a 4-byte header, then 4-byte groups of 8 nibbles) ----
const int kStep[89] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130,
                       143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724, 796, 876, 963, 1060, 1166, 1282,
                       1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026, 4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630,
                       9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500, 20350, 22385, 24623, 27086, 29794, 32767};
const int kIndex[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};

struct Ima {
    int pred = 0, index = 0;
    int16_t step(int nib) {
        const int s = kStep[index];
        int diff = s >> 3;
        if (nib & 1) diff += s >> 2;
        if (nib & 2) diff += s >> 1;
        if (nib & 4) diff += s;
        pred = std::clamp(nib & 8 ? pred - diff : pred + diff, -32768, 32767);
        index = std::clamp(index + kIndex[nib], 0, 88);
        return static_cast<int16_t>(pred);
    }
};

// One block -> interleaved PCM; returns frames written.
size_t decodeBlock(const uint8_t* b, size_t n, int ch, int16_t* out) {
    if (ch < 1 || ch > 2 || n < 4u * ch) return 0;
    Ima st[2];
    for (int c = 0; c < ch; ++c) {
        st[c].pred = static_cast<int16_t>(b[4 * c] | b[4 * c + 1] << 8);
        st[c].index = std::min<int>(b[4 * c + 2], 88);
    }
    size_t frames = 0;
    for (int c = 0; c < ch; ++c) out[c] = static_cast<int16_t>(st[c].pred);
    ++frames;
    const uint8_t* p = b + 4 * ch;
    const size_t groups = (n - 4 * ch) / (4 * ch);
    for (size_t g = 0; g < groups; ++g) {
        for (int c = 0; c < ch; ++c) {
            const uint8_t* q = p + 4 * c;
            for (int i = 0; i < 8; ++i) out[(frames + i) * ch + c] = st[c].step((q[i >> 1] >> ((i & 1) * 4)) & 0xF);
        }
        p += 4 * ch;
        frames += 8;
    }
    return frames;
}

// ---- requests from the guest thread to the main thread -----------------------------------------
std::mutex g_reqLock;
std::condition_variable g_reqCv;
std::string g_request;  // .fmv path to play
bool g_playing = false, g_done = false;
bool g_ndspOk = false, g_ndspTried = false;

std::string fmvFor(const std::string& xmvHost) {
    const size_t slash = xmvHost.find_last_of("/\\");
    std::string stem = xmvHost.substr(slash == std::string::npos ? 0 : slash + 1);
    stem.resize(stem.size() - 4);
    const std::string dir = std::string(g_dataDir) + "/movies/";
    struct stat st;
    for (int pass = 0; pass < 2; ++pass) {
        const std::string p = dir + stem + ".fmv";
        if (stat(p.c_str(), &st) == 0) return p;
        // The converter keeps one copy of NAME / NAME_PAL: try the other name.
        if (stem.size() > 4 && strcasecmp(stem.c_str() + stem.size() - 4, "_PAL") == 0) stem.resize(stem.size() - 4);
        else break;
    }
    return {};
}

bool skipPressed() {
    hidScanInput();
    return hidKeysDown() & (KEY_A | KEY_B | KEY_START);
}

void play(const std::string& path) {
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return;
    FmvHeader h{};
    std::vector<uint32_t> table;
    if (std::fread(&h, sizeof h, 1, f) != 1 || std::memcmp(h.magic, "FMV3", 4) != 0 || !h.frames || !h.fpsNum) {
        std::fclose(f);
        XLOG(0, "movie %s: not an FMV3 file", path.c_str());
        return;
    }
    table.resize(2 * h.frames);
    std::fseek(f, static_cast<long>(h.tableOff), SEEK_SET);
    if (std::fread(table.data(), 8, h.frames, f) != h.frames) {
        std::fclose(f);
        return;
    }
    uint32_t biggest = 0;
    for (uint32_t i = 0; i < h.frames; ++i) biggest = std::max(biggest, table[2 * i + 1]);

    // Decoder work buffer sized for this stream (240x400, the converter's 5 reference frames)
    // rather than the default 9 MB, which the linear heap does not have.
    MVDSTD_CalculateWorkBufSizeConfig calc{};
    calc.ref_frames_a.enable = 1;
    calc.ref_frames_a.ref_frames = 5;
    calc.width = h.width;
    calc.height = h.height;
    u32 work = 0;
    if (R_FAILED(mvdstdCalculateBufferSize(&calc, &work)) || !work) work = MVD_DEFAULT_WORKBUF_SIZE;
    auto* in = static_cast<uint8_t*>(linearMemAlign((biggest + 0xFFF) & ~0xFFFu, 0x40));
    std::vector<uint8_t> frame(biggest);
    if (!in || R_FAILED(mvdstdInit(MVDMODE_VIDEOPROCESSING, MVD_INPUT_H264, MVD_OUTPUT_BGR565, work, nullptr))) {
        XLOG(0, "movie %s: no video decoder (work buffer %lu KB, linear free %lu KB)", path.c_str(), static_cast<unsigned long>(work / 1024),
             static_cast<unsigned long>(linearSpaceFree() / 1024));
        if (in) linearFree(in);
        mvdstdExit();
        std::fclose(f);
        return;
    }
    MVDSTD_Config cfg;
    mvdstdGenerateDefaultConfig(&cfg, h.width, h.height, h.width, h.height, nullptr, nullptr, nullptr);

    // Sound: the whole ADPCM track decoded block by block into a ring of DSP buffers.
    if (!g_ndspTried) {
        g_ndspTried = true;
        g_ndspOk = R_SUCCEEDED(ndspInit());
        if (!g_ndspOk) XLOG(0, "movies: no DSP firmware (sdmc:/3ds/dspfirm.cdc), playing without sound");
    }
    constexpr int kBufs = 4;
    ndspWaveBuf wave[kBufs] = {};
    int16_t* pcm[kBufs] = {};
    std::vector<uint8_t> adpcm;
    uint32_t audioPos = 0;
    const int ch = static_cast<int>(h.audioChannels);
    const uint32_t blocksPerBuf = 8;
    const size_t framesPerBlock = h.blockAlign > 4u * ch ? 1 + (h.blockAlign - 4u * ch) / (4u * ch) * 8 : 0;
    const bool sound = g_ndspOk && framesPerBlock && h.audioSize && (ch == 1 || ch == 2);
    if (sound) {
        ndspChnReset(0);
        ndspChnSetInterp(0, NDSP_INTERP_LINEAR);
        ndspChnSetRate(0, static_cast<float>(h.audioRate));
        ndspChnSetFormat(0, ch == 2 ? NDSP_FORMAT_STEREO_PCM16 : NDSP_FORMAT_MONO_PCM16);
        float mix[12] = {1.0f, 1.0f};
        ndspChnSetMix(0, mix);
        adpcm.resize(h.blockAlign * blocksPerBuf);
        for (int i = 0; i < kBufs; ++i) {
            pcm[i] = static_cast<int16_t*>(linearAlloc(framesPerBlock * blocksPerBuf * ch * 2));
            wave[i].status = NDSP_WBUF_DONE;
        }
    }
    auto feedAudio = [&] {
        if (!sound) return;
        for (int i = 0; i < kBufs; ++i) {
            if (wave[i].status != NDSP_WBUF_DONE || !pcm[i] || audioPos >= h.audioSize) continue;
            const uint32_t want = std::min<uint32_t>(h.blockAlign * blocksPerBuf, h.audioSize - audioPos);
            std::fseek(f, static_cast<long>(h.audioOff + audioPos), SEEK_SET);
            const size_t got = std::fread(adpcm.data(), 1, want, f);
            audioPos += want;
            size_t frames = 0;
            for (size_t o = 0; o + h.blockAlign <= got; o += h.blockAlign) frames += decodeBlock(&adpcm[o], h.blockAlign, ch, pcm[i] + frames * ch);
            if (!frames) continue;
            wave[i].data_vaddr = pcm[i];
            wave[i].nsamples = static_cast<u32>(frames);
            wave[i].looping = false;
            DSP_FlushDataCache(pcm[i], frames * ch * 2);
            ndspChnWaveBufAdd(0, &wave[i]);
        }
    };

    XLOG(0, "movie %s: %lu frames, decoder work buffer %lu KB", path.c_str(), static_cast<unsigned long>(h.frames),
         static_cast<unsigned long>(work / 1024));
    const double frameMs = 1000.0 * h.fpsDen / h.fpsNum;
    const uint64_t start = osGetTime();
    feedAudio();
    for (uint32_t i = 0; i < h.frames; ++i) {
        if (skipPressed()) break;
        const uint32_t off = table[2 * i], size = table[2 * i + 1];
        std::fseek(f, static_cast<long>(h.videoOff + off), SEEK_SET);
        if (std::fread(frame.data(), 1, size, f) != size) break;
        // Each NAL unit with a 3-byte start code, as the decoder takes them (devkitPro's example);
        // x264 writes 4-byte codes before parameter sets and 3-byte ones elsewhere.
        bool ready = false;
        uint32_t p = 0;
        auto nextCode = [&](uint32_t from) {
            for (uint32_t q = from; q + 3 <= size; ++q)
                if (frame[q] == 0 && frame[q + 1] == 0 && frame[q + 2] == 1) return q;
            return size;
        };
        for (p = nextCode(0); p < size;) {
            uint32_t q = nextCode(p + 3);
            uint32_t end = q;
            if (end < size && end > p + 3 && frame[end - 1] == 0) --end;  // the next code's 4th byte
            const int type = p + 3 < size ? frame[p + 3] & 0x1F : 0;
            if (type != 9 && type != 6 && end > p + 3) {  // delimiters and SEI carry nothing for it
                const uint32_t n = end - p;
                std::memcpy(in, &frame[p], n);
                GSPGPU_FlushDataCache(in, n);
                MVDSTD_ProcessNALUnitOut out;
                const Result r = mvdstdProcessVideoFrame(in, n, 0, &out);
                if ((type == 1 || type == 5) && r != MVD_STATUS_PARAMSET && r != MVD_STATUS_INCOMPLETEPROCESSING) ready = true;
            }
            p = q;
        }
        if (ready) {
            cfg.physaddr_outdata0 = osConvertVirtToPhys(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, nullptr, nullptr));
            mvdstdRenderVideoFrame(&cfg, true);
            gfxSwapBuffersGpu();
        }
        feedAudio();
        // Keep to the movie's clock (the decoder is far faster than 30 frames a second).
        const double due = start + (i + 1) * frameMs;
        while (static_cast<double>(osGetTime()) < due) {
            feedAudio();
            svcSleepThread(2000000);
        }
    }
    if (sound) {
        ndspChnWaveBufClear(0);
        for (int i = 0; i < kBufs; ++i)
            if (pcm[i]) linearFree(pcm[i]);
    }
    mvdstdExit();
    linearFree(in);
    std::fclose(f);
}
}  // namespace

// Guest side (an open of a .xmv, GIL held): true when the host handled the movie.
bool hostPlaysMovie(const std::string& hostPath) {
    const std::string fmv = fmvFor(hostPath);
    if (fmv.empty()) {
        // Not converted: skip it (the game's own decoder would take about a second per frame).
        XLOG(0, "movie %s: no converted .fmv in %s/movies, skipped", hostPath.c_str(), g_dataDir);
        return true;
    }
    GilRelease unlocked;
    std::unique_lock<std::mutex> l(g_reqLock);
    g_reqCv.wait(l, [] { return g_request.empty() && !g_playing; });
    g_request = fmv;
    g_done = false;
    g_reqCv.notify_all();
    g_reqCv.wait(l, [] { return g_done; });
    return true;
}

// Main thread, every loop: plays a requested movie (owns the screens meanwhile).
void moviePoll() {
    std::string path;
    {
        std::lock_guard<std::mutex> l(g_reqLock);
        if (g_request.empty()) return;
        path.swap(g_request);
        g_playing = true;
    }
    {
        std::lock_guard<std::mutex> s(screenLock());
        play(path);
        // Leave the top screen black for the status text that follows.
        for (int i = 0; i < 2; ++i) {
            std::memset(gfxGetFramebuffer(GFX_TOP, GFX_LEFT, nullptr, nullptr), 0, 400 * 240 * 2);
            gfxSwapBuffersGpu();
        }
    }
    std::lock_guard<std::mutex> l(g_reqLock);
    g_playing = false;
    g_done = true;
    g_reqCv.notify_all();
}
}  // namespace xb
