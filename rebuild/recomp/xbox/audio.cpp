// DirectSound HLE: the XDK's DSOUND library (statically linked, it drives the MCPX APU
// directly) is replaced at its public entry points (lift.sh --hook). Buffers and streams
// become host voices mixed into an SDL audio device at 48 kHz stereo.
//
// - Buffers: PCM 8/16-bit or Xbox ADPCM data in guest memory (allocated here, or the
//   caller's), looping or one-shot, with volume, frequency, mix-bin panning and a simple
//   3D model (positions are listener relative; inverse-distance rolloff, pan from x).
// - Streams (XMediaObject, used by the movie player and music): packets are copied when
//   submitted and completed on the kernel worker (event or callback) once played.
// - Effects (the DSP image, I3DL2 reverb, envelopes, filters, LFOs) are accepted and ignored.
//
// Without an audio device (headless) the mixer still runs in real time so streams drain.
#include "settings.hpp"
#include "xhost.hpp"

#include <SDL.h>

#include <algorithm>
#include <atomic>
#include <cmath>
#include <deque>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

namespace xb {
namespace {

constexpr int kRate = 48000;
constexpr uint32_t DS_OK = 0, DSERR_INVALIDCALL = 0x88780032, E_OUTOFMEMORY = 0x8007000E;
constexpr uint32_t XMP_SUCCESS = 0, XMP_PENDING = 0x8000000A, XMP_FLUSHED = 0x80004004;
constexpr uint32_t DSBCAPS_CTRL3D = 0x10, DSBCAPS_MIXIN = 0x2000, DSBCAPS_FXIN = 0x80000;
constexpr uint32_t DSBPLAY_LOOPING = 1;
constexpr uint32_t DSBSTATUS_PLAYING = 1, DSBSTATUS_LOOPING = 4;
constexpr uint32_t DSSTREAMSTATUS_READY = 1, DSSTREAMSTATUS_PLAYING = 0x10000, DSSTREAMSTATUS_PAUSED = 0x20000,
                   DSSTREAMSTATUS_STARVED = 0x40000;

struct Format {
    uint16_t tag = 1, channels = 1, bits = 16, blockAlign = 2;
    uint32_t rate = kRate;
    bool adpcm() const { return tag == 0x69; }
    uint32_t framesPerBlock() const { return adpcm() ? 64 : 1; }
    uint32_t bytesToFrames(uint32_t b) const { return b / blockAlign * framesPerBlock(); }
    uint32_t framesToBytes(uint32_t f) const { return f / framesPerBlock() * blockAlign; }
};

Format readFormat(uint32_t pwfx) {
    Format f;
    if (!pwfx) return f;
    f.tag = rd16(pwfx);
    f.channels = std::max<uint16_t>(1, rd16(pwfx + 2));
    f.rate = rd32(pwfx + 4);
    f.blockAlign = rd16(pwfx + 12);
    f.bits = rd16(pwfx + 14);
    if (f.adpcm()) f.blockAlign = static_cast<uint16_t>(36 * f.channels);
    else if (!f.blockAlign) f.blockAlign = static_cast<uint16_t>(f.channels * std::max<uint16_t>(8, f.bits) / 8);
    if (!f.rate) f.rate = kRate;
    return f;
}

// ---- Xbox ADPCM (IMA, 36-byte blocks per channel, 64 samples; the header sample seeds) ----
const int kStep[89] = {7, 8, 9, 10, 11, 12, 13, 14, 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41, 45, 50, 55, 60, 66, 73, 80, 88, 97,
                       107, 118, 130, 143, 157, 173, 190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658, 724,
                       796, 876, 963, 1060, 1166, 1282, 1411, 1552, 1707, 1878, 2066, 2272, 2499, 2749, 3024, 3327, 3660, 4026,
                       4428, 4871, 5358, 5894, 6484, 7132, 7845, 8630, 9493, 10442, 11487, 12635, 13899, 15289, 16818, 18500,
                       20350, 22385, 24623, 27086, 29794, 32767};
const int kIndex[16] = {-1, -1, -1, -1, 2, 4, 6, 8, -1, -1, -1, -1, 2, 4, 6, 8};

// Decodes one block (guest address) into out[64 * channels] (interleaved).
void decodeAdpcmBlock(const uint8_t* b, int ch, int16_t* out) {
    for (int c = 0; c < ch; ++c) {
        int pred = static_cast<int16_t>(b[4 * c] | (b[4 * c + 1] << 8));
        int idx = std::clamp<int>(b[4 * c + 2], 0, 88);
        int n = 0;
        for (int chunk = 0; chunk < 8; ++chunk) {
            const uint8_t* d = b + 4 * ch + chunk * 4 * ch + 4 * c;
            for (int k = 0; k < 8; ++k) {
                const int nib = (d[k >> 1] >> ((k & 1) * 4)) & 15;
                const int step = kStep[idx];
                int diff = step >> 3;
                if (nib & 1) diff += step >> 2;
                if (nib & 2) diff += step >> 1;
                if (nib & 4) diff += step;
                pred = std::clamp(nib & 8 ? pred - diff : pred + diff, -32768, 32767);
                idx = std::clamp(idx + kIndex[nib], 0, 88);
                out[(n++) * ch + c] = static_cast<int16_t>(pred);
            }
        }
    }
}

// ---- voices -----------------------------------------------------------------------------
struct Packet {
    // XMEDIAPACKET fields, captured at Process (callers pass packets on their stack)
    uint32_t pCompleted = 0, pStatus = 0, event = 0;
    std::vector<uint8_t> data;
    uint32_t frames = 0, played = 0;
    bool done = false;
};

struct Voice {
    uint32_t guest = 0;  // the guest object address (handle)
    bool stream = false;
    Format fmt;
    uint32_t flags = 0;
    // buffer data
    uint32_t data = 0, bytes = 0;
    bool ownData = false;
    // playback
    bool playing = false, looping = false, paused = false;
    double pos = 0;  // frames
    uint32_t freq = 0;  // 0: format rate
    int32_t volume = 0;  // hundredths of a dB
    float binL = 1, binR = 1;
    float x = 0, y = 0, z = 0, minDist = 1, maxDist = 1e9f;
    bool positioned = false;
    // stream
    uint32_t maxPackets = 0, callback = 0, context = 0;
    std::deque<Packet> packets;
    // ADPCM block cache
    int64_t cachedBlock = -1;
    const uint8_t* cachedSrc = nullptr;
    int16_t block[64 * 6];
};

std::mutex g_lock;  // voices (mixer thread vs guest threads)
std::unordered_map<uint32_t, Voice*> g_voices;
uint32_t g_dsound = 0, g_effectDesc = 0;
float g_rolloff = 1, g_masterGain = 1;
std::atomic<uint64_t> g_mixedFrames{0};
bool g_started = false;

Voice* voice(uint32_t obj) {
    auto it = g_voices.find(obj);
    return it == g_voices.end() ? nullptr : it->second;
}

float dbGain(int32_t hundredths) { return hundredths <= -10000 ? 0.0f : std::pow(10.0f, hundredths / 2000.0f); }

// Fetches frame `f` (no interpolation) as float L/R. Returns false past the end.
bool fetchBuffer(Voice& v, uint32_t f, float& l, float& r) {
    const Format& fm = v.fmt;
    const uint32_t total = fm.bytesToFrames(v.bytes);
    if (f >= total) return false;
    const uint8_t* base = gp(v.data);
    int s0, s1;
    if (fm.adpcm()) {
        const int64_t blk = f / 64;
        if (blk != v.cachedBlock || v.cachedSrc != base) {
            decodeAdpcmBlock(base + blk * fm.blockAlign, std::min<int>(fm.channels, 6), v.block);
            v.cachedBlock = blk;
            v.cachedSrc = base;
        }
        const int16_t* s = v.block + (f % 64) * std::min<int>(fm.channels, 6);
        s0 = s[0];
        s1 = fm.channels > 1 ? s[1] : s0;
    } else if (fm.bits == 8) {
        const uint8_t* s = base + static_cast<size_t>(f) * fm.blockAlign;
        s0 = (s[0] - 128) << 8;
        s1 = fm.channels > 1 ? (s[1] - 128) << 8 : s0;
    } else {
        const uint8_t* s = base + static_cast<size_t>(f) * fm.blockAlign;
        s0 = static_cast<int16_t>(s[0] | (s[1] << 8));
        s1 = fm.channels > 1 ? static_cast<int16_t>(s[2] | (s[3] << 8)) : s0;
    }
    l = s0 / 32768.0f;
    r = s1 / 32768.0f;
    return true;
}

bool fetchStream(Voice& v, float& l, float& r) {
    for (;;) {
        Packet* p = nullptr;
        for (auto& q : v.packets)
            if (!q.done) { p = &q; break; }
        if (!p) return false;
        if (p->played >= p->frames) { p->done = true; continue; }
        const Format& fm = v.fmt;
        const uint32_t f = p->played;
        int s0, s1;
        if (fm.adpcm()) {
            const int64_t blk = f / 64;
            if (blk != v.cachedBlock || v.cachedSrc != p->data.data()) {
                decodeAdpcmBlock(p->data.data() + blk * fm.blockAlign, std::min<int>(fm.channels, 6), v.block);
                v.cachedBlock = blk;
                v.cachedSrc = p->data.data();
            }
            const int16_t* s = v.block + (f % 64) * std::min<int>(fm.channels, 6);
            s0 = s[0];
            s1 = fm.channels > 1 ? s[1] : s0;
        } else if (fm.bits == 8) {
            const uint8_t* s = p->data.data() + static_cast<size_t>(f) * fm.blockAlign;
            s0 = (s[0] - 128) << 8;
            s1 = fm.channels > 1 ? (s[1] - 128) << 8 : s0;
        } else {
            const uint8_t* s = p->data.data() + static_cast<size_t>(f) * fm.blockAlign;
            s0 = static_cast<int16_t>(s[0] | (s[1] << 8));
            s1 = fm.channels > 1 ? static_cast<int16_t>(s[2] | (s[3] << 8)) : s0;
        }
        l = s0 / 32768.0f;
        r = s1 / 32768.0f;
        return true;
    }
}

void mix(float* out, int frames) {
    std::fill(out, out + frames * 2, 0.0f);
    std::lock_guard<std::mutex> l(g_lock);
    for (auto& [obj, vp] : g_voices) {
        Voice& v = *vp;
        if (!v.playing || v.paused) continue;
        if (!v.stream && (!v.data || !v.bytes)) continue;
        float gain = dbGain(v.volume) * g_masterGain, gl = v.binL, gr = v.binR;
        if (v.flags & DSBCAPS_CTRL3D) {
            if (v.positioned) {
                const float d = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
                if (d > v.maxDist) gain = 0;
                else if (d > v.minDist) gain *= v.minDist / (v.minDist + g_rolloff * (d - v.minDist));
                const float pan = d > 1e-4f ? std::clamp(v.x / d, -1.0f, 1.0f) : 0.0f;
                gl = std::sqrt(0.5f * (1 - pan));
                gr = std::sqrt(0.5f * (1 + pan));
            } else {
                gl = gr = 0.7071f;
            }
        }
        const double step = static_cast<double>(v.freq ? v.freq : v.fmt.rate) / kRate;
        for (int i = 0; i < frames; ++i) {
            float a, b;
            if (v.stream) {
                if (!fetchStream(v, a, b)) break;
                // streams advance packet-wise
                Packet* p = nullptr;
                for (auto& q : v.packets)
                    if (!q.done) { p = &q; break; }
                v.pos += step;
                while (p && v.pos >= 1.0) {
                    v.pos -= 1.0;
                    if (++p->played >= p->frames) {
                        p->done = true;
                        p = nullptr;
                        for (auto& q : v.packets)
                            if (!q.done) { p = &q; break; }
                    }
                }
            } else {
                const uint32_t total = v.fmt.bytesToFrames(v.bytes);
                if (v.pos >= total) {
                    if (v.looping && total) v.pos = std::fmod(v.pos, static_cast<double>(total));
                    else { v.playing = false; v.pos = 0; break; }
                }
                if (!fetchBuffer(v, static_cast<uint32_t>(v.pos), a, b)) { v.playing = false; v.pos = 0; break; }
                v.pos += step;
            }
            if (v.fmt.channels == 1 || (v.flags & DSBCAPS_CTRL3D)) {
                const float m = v.fmt.channels == 1 ? a : 0.5f * (a + b);
                out[2 * i] += m * gain * gl;
                out[2 * i + 1] += m * gain * gr;
            } else {
                out[2 * i] += a * gain;
                out[2 * i + 1] += b * gain;
            }
        }
    }
    g_mixedFrames += static_cast<uint64_t>(frames);
}

void sdlCallback(void*, Uint8* stream, int len) {
    const int frames = len / 4;
    static std::vector<float> buf;
    buf.resize(static_cast<size_t>(frames) * 2);
    mix(buf.data(), frames);
    auto* o = reinterpret_cast<int16_t*>(stream);
    for (int i = 0; i < frames * 2; ++i) o[i] = static_cast<int16_t>(std::clamp(buf[i], -1.0f, 1.0f) * 32767.0f);
}

void silentMixer() {
    std::vector<float> buf(480 * 2);
    auto next = std::chrono::steady_clock::now();
    for (;;) {
        mix(buf.data(), 480);
        next += std::chrono::milliseconds(10);
        std::this_thread::sleep_until(next);
    }
}

// GIL held: reports a packet done (completed size, status, then the callback or event).
void completePacket(const Packet& p, uint32_t callback, uint32_t context, uint32_t size, uint32_t status) {
    if (p.pCompleted) wr32(p.pCompleted, size);
    if (p.pStatus) wr32(p.pStatus, status);
    if (callback) guestCall(callback, {context, p.event, status});
    else if (p.event) {
        if (Handle* h = handleGet(p.event)) keSetEvent(h->object);
    }
}

// Worker service (GIL held): completes played stream packets.
void service() {
    struct Done { Packet p; uint32_t cb, ctx; };
    std::vector<Done> done;
    {
        std::lock_guard<std::mutex> l(g_lock);
        for (auto& [obj, v] : g_voices) {
            if (!v->stream) continue;
            while (!v->packets.empty() && v->packets.front().done) {
                Packet& p = v->packets.front();
                done.push_back({std::move(p), v->callback, v->context});
                v->packets.pop_front();
                v->cachedBlock = -1;
            }
        }
    }
    for (const Done& d : done) completePacket(d.p, d.cb, d.ctx, static_cast<uint32_t>(d.p.data.size()), XMP_SUCCESS);
}

void start() {
    if (g_started) return;
    g_started = true;
    g_masterGain = std::clamp(settings().volume, 0, 100) / 100.0f;
    workerAddService(service);
    SDL_AudioSpec want{}, have{};
    want.freq = kRate;
    want.format = AUDIO_S16SYS;
    want.channels = 2;
    want.samples = 512;
    want.callback = sdlCallback;
    if (!getenv("FABLE_NO_AUDIO") && SDL_InitSubSystem(SDL_INIT_AUDIO) == 0) {
        const SDL_AudioDeviceID dev = SDL_OpenAudioDevice(nullptr, 0, &want, &have, 0);
        if (dev) {
            SDL_PauseAudioDevice(dev, 0);
            XLOG(1, "audio: %d Hz, %d channels (%s)", have.freq, have.channels, SDL_GetCurrentAudioDriver());
            return;
        }
    }
    XLOG(1, "audio: no device (%s), mixing silently", SDL_GetError());
    std::thread(silentMixer).detach();
}

// ---- guest objects ----------------------------------------------------------------------
uint32_t g_streamVtbl = 0;

void ret(Ctx* c, uint32_t value, int args) {
    c->eax = value;
    c->esp += 4 + 4u * static_cast<uint32_t>(args);
}
uint32_t arg(Ctx* c, int i) { return rd32(c->esp + 4 + 4 * i); }
float argf(Ctx* c, int i) {
    const uint32_t u = arg(c, i);
    float f;
    std::memcpy(&f, &u, 4);
    return f;
}

void readMixBins(Voice& v, uint32_t pMixBins) {
    if (!pMixBins) return;
    const uint32_t n = std::min<uint32_t>(rd32(pMixBins), 32), pairs = rd32(pMixBins + 4);
    float l = 0, r = 0;
    for (uint32_t i = 0; i < n && pairs; ++i) {
        const uint32_t bin = rd32(pairs + 8 * i);
        const float g = dbGain(static_cast<int32_t>(rd32(pairs + 8 * i + 4)));
        if (bin == 0 || bin == 4) l = std::max(l, g);
        else if (bin == 1 || bin == 5) r = std::max(r, g);
        else if (bin == 2) { l = std::max(l, g * 0.7071f); r = std::max(r, g * 0.7071f); }
    }
    if (l > 0 || r > 0) { v.binL = l; v.binR = r; }
}

uint32_t newObject(uint32_t vtbl) {
    const uint32_t o = poolAllocZero(0x40);
    wr32(o, vtbl);
    wr32(o + 4, 1);
    return o;
}

// ---- stream vtable (XMediaObject) -------------------------------------------------------
uint32_t s_AddRef(Ctx* c) { return rd32(arg(c, 0) + 4) + 1; }
uint32_t s_Release(Ctx* c) { return 0; }
uint32_t s_GetInfo(Ctx* c) {
    const uint32_t p = arg(c, 1);
    std::lock_guard<std::mutex> l(g_lock);
    Voice* v = voice(arg(c, 0));
    if (p) {
        wr32(p, 0);
        wr32(p + 4, v ? v->fmt.blockAlign : 0);
        wr32(p + 8, 0);
        wr32(p + 12, 0);
    }
    return DS_OK;
}
uint32_t s_GetStatus(Ctx* c) {
    const uint32_t p = arg(c, 1);
    std::lock_guard<std::mutex> l(g_lock);
    Voice* v = voice(arg(c, 0));
    uint32_t st = 0;
    if (v) {
        if (v->packets.size() < std::max<uint32_t>(v->maxPackets, 1)) st |= DSSTREAMSTATUS_READY;
        if (!v->packets.empty()) st |= DSSTREAMSTATUS_PLAYING;
        else st |= DSSTREAMSTATUS_STARVED;
        if (v->paused) st |= DSSTREAMSTATUS_PAUSED;
    }
    if (p) wr32(p, st);
    return DS_OK;
}
uint32_t s_Process(Ctx* c) {
    const uint32_t pkt = arg(c, 1);
    if (!pkt) return DS_OK;
    const uint32_t buf = rd32(pkt), size = rd32(pkt + 4), pStatus = rd32(pkt + 12);
    if (pStatus) wr32(pStatus, XMP_PENDING);
    XLOG(2, "DSStream %08X Process(packet %08X: buf %08X, %u bytes, status@%08X, done@%08X, ev %08X)", arg(c, 0), pkt, buf, size,
         pStatus, rd32(pkt + 8), rd32(pkt + 16));
    std::lock_guard<std::mutex> l(g_lock);
    Voice* v = voice(arg(c, 0));
    if (!v) return DSERR_INVALIDCALL;
    Packet p;
    p.pCompleted = rd32(pkt + 8);
    p.pStatus = pStatus;
    p.event = rd32(pkt + 16);
    p.data.assign(gp(buf), gp(buf) + size);
    p.frames = v->fmt.bytesToFrames(size);
    v->packets.push_back(std::move(p));
    v->playing = true;
    return DS_OK;
}
void flushStream(Voice* v, uint32_t status) {
    std::deque<Packet> pk;
    {
        std::lock_guard<std::mutex> l(g_lock);
        pk.swap(v->packets);
        v->cachedBlock = -1;
        v->pos = 0;
    }
    XLOG(2, "DSStream %08X flush: %zu packets", v->guest, pk.size());
    for (Packet& p : pk) completePacket(p, v->callback, v->context, 0, status);
}
uint32_t s_Discontinuity(Ctx* c) { return DS_OK; }
uint32_t s_Flush(Ctx* c) {
    if (Voice* v = voice(arg(c, 0))) flushStream(v, XMP_FLUSHED);
    return DS_OK;
}

Voice* newVoice(uint32_t obj) {
    auto* v = new Voice;
    v->guest = obj;
    std::lock_guard<std::mutex> l(g_lock);
    g_voices[obj] = v;
    return v;
}

}  // namespace
}  // namespace xb

using namespace xb;

extern "C" {

// HRESULT DirectSoundCreate(LPGUID, LPDIRECTSOUND8*, LPUNKNOWN)
void hle_DirectSoundCreate(Ctx* c) {
    start();
    if (!g_dsound) g_dsound = newObject(0);
    if (arg(c, 1)) wr32(arg(c, 1), g_dsound);
    XLOG(1, "HLE DirectSoundCreate -> %08X", g_dsound);
    ret(c, DS_OK, 3);
}

// HRESULT DirectSoundCreateStream(LPCDSSTREAMDESC, LPDIRECTSOUNDSTREAM*)
// DSSTREAMDESC: dwFlags, dwMaxAttachedPackets, lpwfxFormat, lpfnCallback, lpvContext, lpMixBins
void hle_DirectSoundCreateStream(Ctx* c) {
    start();
    if (!g_streamVtbl) {
        g_streamVtbl = poolAllocZero(0x20);
        const KFn fns[7] = {s_AddRef, s_Release, s_GetInfo, s_GetStatus, s_Process, s_Discontinuity, s_Flush};
        const int args[7] = {1, 1, 2, 2, 3, 1, 1};
        const char* names[7] = {"DSStream::AddRef", "DSStream::Release", "DSStream::GetInfo", "DSStream::GetStatus",
                                "DSStream::Process", "DSStream::Discontinuity", "DSStream::Flush"};
        for (int i = 0; i < 7; ++i) wr32(g_streamVtbl + 4 * i, hostTrap(fns[i], args[i], names[i]));
    }
    const uint32_t d = arg(c, 0);
    const uint32_t obj = newObject(g_streamVtbl);
    Voice* v = newVoice(obj);
    v->stream = true;
    v->flags = rd32(d);
    v->maxPackets = rd32(d + 4);
    v->fmt = readFormat(rd32(d + 8));
    v->callback = rd32(d + 12);
    v->context = rd32(d + 16);
    readMixBins(*v, rd32(d + 20));
    if (arg(c, 1)) wr32(arg(c, 1), obj);
    XLOG(1, "HLE DirectSoundCreateStream: tag %u, %u ch, %u Hz, %u packets -> %08X", v->fmt.tag, v->fmt.channels, v->fmt.rate,
         v->maxPackets, obj);
    ret(c, DS_OK, 2);
}

void hle_DirectSoundDoWork(Ctx* c) { ret(c, 0, 0); }
void hle_DirectSoundUseLightHRTF(Ctx* c) { ret(c, 0, 0); }

// ---- IDirectSound ----
void hle_IDirectSound_Release(Ctx* c) { ret(c, 1, 1); }
void hle_IDirectSound_CommitDeferredSettings(Ctx* c) { ret(c, DS_OK, 1); }
void hle_IDirectSound_SynchPlayback(Ctx* c) { ret(c, DS_OK, 1); }
void hle_IDirectSound_EnableHeadphones(Ctx* c) { ret(c, DS_OK, 2); }
void hle_IDirectSound_SetDistanceFactor(Ctx* c) { ret(c, DS_OK, 3); }
void hle_IDirectSound_SetDopplerFactor(Ctx* c) { ret(c, DS_OK, 3); }
void hle_IDirectSound_SetRolloffFactor(Ctx* c) {
    g_rolloff = std::max(0.0f, argf(c, 1));
    ret(c, DS_OK, 3);
}
void hle_IDirectSound_SetI3DL2Listener(Ctx* c) { ret(c, DS_OK, 3); }
void hle_IDirectSound_SetMixBinHeadroom(Ctx* c) { ret(c, DS_OK, 3); }
void hle_IDirectSound_SetEffectData(Ctx* c) { ret(c, DS_OK, 6); }

// HRESULT GetEffectData(this, dwEffectIndex, dwOffset, LPVOID pvData, DWORD dwDataSize)
void hle_IDirectSound_GetEffectData(Ctx* c) {
    if (arg(c, 3) && arg(c, 4)) std::memset(gp(arg(c, 3)), 0, arg(c, 4));
    ret(c, DS_OK, 5);
}

// HRESULT DownloadEffectsImage(this, LPCVOID pvImageBuffer, DWORD dwImageSize,
//                              LPCDSEFFECTIMAGELOC pImageLoc, LPDSEFFECTIMAGEDESC* ppImageDesc)
// DSEFFECTIMAGEDESC: dwEffectCount, dwTotalScratchSize, DSEFFECTMAP[]{code, codeSize, state,
// stateSize, yMem, yMemSize, scratch, scratchSize}. The maps point at zeroed scratch memory.
void hle_IDirectSound_DownloadEffectsImage(Ctx* c) {
    if (!g_effectDesc) {
        constexpr uint32_t kMaps = 32, kState = 0x1000;
        g_effectDesc = poolAllocZero(8 + kMaps * 32);
        wr32(g_effectDesc, kMaps);
        for (uint32_t i = 0; i < kMaps; ++i) {
            const uint32_t m = g_effectDesc + 8 + 32 * i;
            wr32(m + 8, poolAllocZero(kState));
            wr32(m + 12, kState);
        }
    }
    if (arg(c, 4)) wr32(arg(c, 4), g_effectDesc);
    XLOG(1, "HLE DownloadEffectsImage (%u bytes): effects ignored", arg(c, 2));
    ret(c, DS_OK, 5);
}

// HRESULT GetCaps(this, LPDSCAPS): dwFree2DBuffers, dwFree3DBuffers, dwFreeBufferSGEs, dwMemoryAllocated
void hle_IDirectSound_GetCaps(Ctx* c) {
    const uint32_t p = arg(c, 1);
    if (p) {
        wr32(p, 192);
        wr32(p + 4, 64);
        wr32(p + 8, 2047);
        wr32(p + 12, 0);
    }
    ret(c, DS_OK, 2);
}

// HRESULT GetTime(this, REFERENCE_TIME*)
void hle_IDirectSound_GetTime(Ctx* c) {
    const uint64_t t = g_mixedFrames.load() * 10000000ull / kRate;
    if (arg(c, 1)) {
        wr32(arg(c, 1), static_cast<uint32_t>(t));
        wr32(arg(c, 1) + 4, static_cast<uint32_t>(t >> 32));
    }
    ret(c, DS_OK, 2);
}

// HRESULT CreateSoundBuffer(this, LPCDSBUFFERDESC, LPDIRECTSOUNDBUFFER*, LPUNKNOWN)
// DSBUFFERDESC: dwSize, dwFlags, dwBufferBytes, lpwfxFormat, lpMixBins, dwInputMixBin
void hle_IDirectSound_CreateSoundBuffer(Ctx* c) {
    const uint32_t d = arg(c, 1);
    const uint32_t obj = newObject(0);
    Voice* v = newVoice(obj);
    v->flags = rd32(d + 4);
    v->bytes = rd32(d + 8);
    v->fmt = readFormat(rd32(d + 12));
    readMixBins(*v, rd32(d + 16));
    if (v->bytes && !(v->flags & (DSBCAPS_MIXIN | DSBCAPS_FXIN))) {
        v->data = physAlloc((v->bytes + 0xFFF) & ~0xFFFu, 0x1000, 0, kPhysSize, true);
        if (!v->data) {
            ret(c, E_OUTOFMEMORY, 4);
            return;
        }
        std::memset(gp(v->data), 0, v->bytes);
        v->ownData = true;
    }
    if (arg(c, 2)) wr32(arg(c, 2), obj);
    XLOG(2, "HLE CreateSoundBuffer: flags %08X, %u bytes, tag %u, %u ch, %u Hz -> %08X", v->flags, v->bytes, v->fmt.tag,
         v->fmt.channels, v->fmt.rate, obj);
    ret(c, DS_OK, 4);
}

// ---- IDirectSoundBuffer ----
#define BUF(n)                                                  \
    std::lock_guard<std::mutex> lock(g_lock);                   \
    Voice* v = voice(arg(c, 0));                                \
    if (!v) { ret(c, DSERR_INVALIDCALL, n); return; }

void hle_IDirectSoundBuffer_Release(Ctx* c) {
    Voice* v;
    {
        std::lock_guard<std::mutex> l(g_lock);
        v = voice(arg(c, 0));
        if (v) g_voices.erase(arg(c, 0));
    }
    if (v) {
        if (v->ownData) physFree(v->data);
        poolFree(v->guest);
        delete v;
    }
    ret(c, 0, 1);
}

// HRESULT Play(this, DWORD, DWORD, DWORD dwFlags)
void hle_IDirectSoundBuffer_Play(Ctx* c) {
    BUF(4);
    v->playing = true;
    v->looping = arg(c, 3) & DSBPLAY_LOOPING;
    ret(c, DS_OK, 4);
}
// HRESULT PlayEx(this, REFERENCE_TIME rtTimeStamp, DWORD dwFlags)
void hle_IDirectSoundBuffer_PlayEx(Ctx* c) {
    BUF(4);
    v->playing = true;
    v->looping = arg(c, 3) & DSBPLAY_LOOPING;
    ret(c, DS_OK, 4);
}
void hle_IDirectSoundBuffer_Stop(Ctx* c) {
    BUF(1);
    v->playing = false;
    ret(c, DS_OK, 1);
}
// HRESULT StopEx(this, REFERENCE_TIME rtTimeStamp, DWORD dwFlags)
void hle_IDirectSoundBuffer_StopEx(Ctx* c) {
    BUF(4);
    v->playing = false;
    ret(c, DS_OK, 4);
}
// HRESULT GetStatus(this, LPDWORD)
void hle_IDirectSoundBuffer_GetStatus(Ctx* c) {
    BUF(2);
    if (arg(c, 1)) wr32(arg(c, 1), (v->playing ? DSBSTATUS_PLAYING : 0) | (v->playing && v->looping ? DSBSTATUS_LOOPING : 0));
    ret(c, DS_OK, 2);
}
// HRESULT GetCurrentPosition(this, LPDWORD pdwPlay, LPDWORD pdwWrite)
void hle_IDirectSoundBuffer_GetCurrentPosition(Ctx* c) {
    BUF(3);
    const uint32_t play = v->fmt.framesToBytes(static_cast<uint32_t>(v->pos));
    if (arg(c, 1)) wr32(arg(c, 1), play);
    if (arg(c, 2)) wr32(arg(c, 2), v->bytes ? (play + v->fmt.blockAlign * 64u) % v->bytes : 0);
    ret(c, DS_OK, 3);
}
// HRESULT SetCurrentPosition(this, DWORD dwPlayCursor)
void hle_IDirectSoundBuffer_SetCurrentPosition(Ctx* c) {
    BUF(2);
    v->pos = v->fmt.bytesToFrames(arg(c, 1));
    ret(c, DS_OK, 2);
}
// HRESULT Lock(this, DWORD dwOffset, DWORD dwBytes, LPVOID* pp1, LPDWORD pc1, LPVOID* pp2,
//              LPDWORD pc2, DWORD dwFlags)
void hle_IDirectSoundBuffer_Lock(Ctx* c) {
    BUF(8);
    uint32_t off = arg(c, 1), n = arg(c, 2);
    const uint32_t flags = arg(c, 7);
    if (flags & 1) off = v->fmt.framesToBytes(static_cast<uint32_t>(v->pos));  // DSBLOCK_FROMWRITECURSOR
    if ((flags & 2) || n > v->bytes) n = v->bytes;                              // DSBLOCK_ENTIREBUFFER
    if (v->bytes) off %= v->bytes;
    const uint32_t first = std::min(n, v->bytes - off);
    if (arg(c, 3)) wr32(arg(c, 3), v->data + off);
    if (arg(c, 4)) wr32(arg(c, 4), first);
    if (arg(c, 5)) wr32(arg(c, 5), n > first ? v->data : 0);
    if (arg(c, 6)) wr32(arg(c, 6), n - first);
    v->cachedBlock = -1;
    ret(c, DS_OK, 8);
}
void hle_IDirectSoundBuffer_SetVolume(Ctx* c) {
    BUF(2);
    v->volume = static_cast<int32_t>(arg(c, 1));
    ret(c, DS_OK, 2);
}
void hle_IDirectSoundBuffer_SetFrequency(Ctx* c) {
    BUF(2);
    v->freq = arg(c, 1);
    ret(c, DS_OK, 2);
}
void hle_IDirectSoundBuffer_SetMixBinVolumes_8(Ctx* c) {
    BUF(2);
    readMixBins(*v, arg(c, 1));
    ret(c, DS_OK, 2);
}
// HRESULT SetPosition(this, FLOAT x, FLOAT y, FLOAT z, DWORD dwApply)
void hle_IDirectSoundBuffer_SetPosition(Ctx* c) {
    BUF(5);
    v->x = argf(c, 1);
    v->y = argf(c, 2);
    v->z = argf(c, 3);
    v->positioned = true;
    ret(c, DS_OK, 5);
}
void hle_IDirectSoundBuffer_SetMinDistance(Ctx* c) {
    BUF(3);
    v->minDist = std::max(0.01f, argf(c, 1));
    ret(c, DS_OK, 3);
}
void hle_IDirectSoundBuffer_SetMaxDistance(Ctx* c) {
    BUF(3);
    v->maxDist = std::max(0.01f, argf(c, 1));
    ret(c, DS_OK, 3);
}
void hle_IDirectSoundBuffer_SetHeadroom(Ctx* c) { ret(c, DS_OK, 2); }
void hle_IDirectSoundBuffer_SetLFO(Ctx* c) { ret(c, DS_OK, 2); }
void hle_IDirectSoundBuffer_SetEG(Ctx* c) { ret(c, DS_OK, 2); }
void hle_IDirectSoundBuffer_SetFilter(Ctx* c) { ret(c, DS_OK, 2); }
void hle_IDirectSoundBuffer_SetOutputBuffer(Ctx* c) { ret(c, DS_OK, 2); }
void hle_IDirectSoundBuffer_SetI3DL2Source(Ctx* c) { ret(c, DS_OK, 3); }
void hle_IDirectSoundBuffer_SetConeAngles(Ctx* c) { ret(c, DS_OK, 4); }
void hle_IDirectSoundBuffer_SetConeOrientation(Ctx* c) { ret(c, DS_OK, 5); }
void hle_IDirectSoundBuffer_SetConeOutsideVolume(Ctx* c) { ret(c, DS_OK, 3); }

// ---- IDirectSoundStream (non-virtual helpers) ----
// HRESULT SetVolume(this, LONG)
void hle_IDirectSoundStream_SetVolume(Ctx* c) {
    BUF(2);
    v->volume = static_cast<int32_t>(arg(c, 1));
    ret(c, DS_OK, 2);
}
// HRESULT Pause(this, DWORD dwPause): 0 resume, 1 pause
void hle_IDirectSoundStream_Pause(Ctx* c) {
    BUF(2);
    v->paused = arg(c, 1) == 1;
    ret(c, DS_OK, 2);
}
// HRESULT FlushEx(this, REFERENCE_TIME rtTimeStamp, DWORD dwFlags)
void hle_IDirectSoundStream_FlushEx(Ctx* c) {
    Voice* v;
    {
        std::lock_guard<std::mutex> l(g_lock);
        v = voice(arg(c, 0));
    }
    if (v) flushStream(v, XMP_FLUSHED);
    ret(c, DS_OK, 4);
}

}  // extern "C"
