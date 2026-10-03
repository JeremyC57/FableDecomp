// DirectShow for the game's movies, implemented by the host.
//
// Fable plays every .wmv like the DirectX 9 "Texture3D9" sample: it creates
// CLSID_FilterGraph, adds its own CBaseVideoRenderer ("Fable Texture Render", guest
// code built on the DirectShow base classes) and calls RenderFile. The renderer only
// accepts MEDIATYPE_Video / MEDIASUBTYPE_RGB24 / FORMAT_VideoInfo and copies each
// sample into a D3D texture.
//
// Instead of bridging 64-bit DirectShow to 32-bit filters, the filter graph here is a
// host object: Media Foundation decodes the file, video frames go to the guest renderer
// through its own x86 interfaces (IPin::ReceiveConnection, IMemInputPin::Receive,
// IPin::EndOfStream, IBaseFilter::Pause/Run/Stop) and the audio plays through waveOut.
// The renderer has no reference clock, so it draws each sample as it arrives; this
// file paces delivery against the movie's timestamps.
#include "com.hpp"

#include <dshow.h>
#include <mfapi.h>
#include <mferror.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mmsystem.h>

#include <atomic>
#include <cmath>
#include <condition_variable>
#include <deque>
#include <mutex>
#include <thread>
#include <vector>

using namespace host;

namespace {

// ---------------------------------------------------------------------------
// guest COM helpers
// ---------------------------------------------------------------------------
uint32_t vfn(uint32_t obj, int slot) { return rd32(rd32(obj) + 4 * slot); }
uint32_t gcall(uint32_t obj, int slot, std::initializer_list<uint32_t> args) {
    std::vector<uint32_t> a{obj};
    a.insert(a.end(), args);
    switch (a.size()) {  // guestCall takes an initializer_list
    case 1: return guestCall(vfn(obj, slot), {a[0]});
    case 2: return guestCall(vfn(obj, slot), {a[0], a[1]});
    case 3: return guestCall(vfn(obj, slot), {a[0], a[1], a[2]});
    case 4: return guestCall(vfn(obj, slot), {a[0], a[1], a[2], a[3]});
    default: return guestCall(vfn(obj, slot), {a[0], a[1], a[2], a[3], a[4]});
    }
}
void gAddRef(uint32_t o) { if (o) gcall(o, 1, {}); }
void gRelease(uint32_t o) { if (o) gcall(o, 2, {}); }
uint32_t gQuery(uint32_t o, const GUID& iid) {
    const uint32_t g = gmemdup(&iid, sizeof iid), out = gcalloc(1, 4);
    const uint32_t hr = gcall(o, 0, {g, out});
    const uint32_t r = static_cast<int32_t>(hr) >= 0 ? rd32(out) : 0;
    gfree(out);
    gfree(g);
    return r;
}
uint32_t gwstr(const wchar_t* s) { return gmemdup(s, static_cast<uint32_t>((wcslen(s) + 1) * 2)); }

// IBaseFilter slots
enum { BF_Stop = 4, BF_Pause = 5, BF_Run = 6, BF_FindPin = 11, BF_JoinFilterGraph = 13 };
// IPin slots
enum { PIN_ReceiveConnection = 4, PIN_Disconnect = 5, PIN_EndOfStream = 14, PIN_BeginFlush = 15, PIN_EndFlush = 16 };
// IMemInputPin slots
enum { MIP_Receive = 6 };

// ---------------------------------------------------------------------------
// the graph
// ---------------------------------------------------------------------------
// Guest layout of a graph: one 8-byte slot per interface, { vtable, graph id }.
enum Iface { I_Graph, I_Control, I_Position, I_Seeking, I_Event, I_Sink, I_Audio, I_Pin, I_Sample, I_Count };

struct VideoFrame {
    LONGLONG time;
    std::vector<uint8_t> rgb24;  // bottom-up DIB rows, stride `stride`
};

struct Event {
    long code;
    uint32_t p1, p2;
};

struct Graph {
    uint32_t id = 0, guest = 0;
    std::atomic<long> refs{1};

    // filters added by the game (AddRef'd), and the connection to its renderer
    std::vector<std::pair<uint32_t, std::wstring>> filters;
    uint32_t rendererFilter = 0, rendererPin = 0, memInput = 0;
    uint32_t mediaType = 0, videoInfo = 0, sampleBuf = 0;  // guest AM_MEDIA_TYPE, VIDEOINFOHEADER, sample data

    // source
    IMFSourceReader* reader = nullptr;
    bool hasVideo = false, hasAudio = false;
    UINT32 width = 0, height = 0, stride = 0;
    enum { SRC_RGB32, SRC_NV12, SRC_YUY2 } srcFormat = SRC_RGB32;
    LONGLONG duration = 0;
    WAVEFORMATEX wfx{};

    // state (guarded by m)
    std::mutex m;
    std::condition_variable cv;
    enum State { Stopped, Paused, Running } state = Stopped;
    bool quit = false, seekPending = true, complete = false, videoEos = false, audioEos = false;
    LONGLONG seekTo = 0, stopAt = 0;  // stopAt 0 = end
    LONGLONG clockPos = 0;            // media time at clockQpc (or while not running)
    LARGE_INTEGER clockQpc{};
    long volume = 0;                  // IBasicAudio, hundredths of dB
    std::deque<VideoFrame> frames;
    std::thread worker;

    // audio
    HWAVEOUT wave = nullptr;
    std::deque<WAVEHDR*> waveQueue;
    uint64_t waveBytesWritten = 0;

    // events
    std::mutex em;
    std::deque<Event> events;
    HANDLE eventHandle = CreateEventW(nullptr, TRUE, FALSE, nullptr);
    HWND notifyWnd = nullptr;
    UINT notifyMsg = 0;
    uint32_t notifyParam = 0;

    LONGLONG nowLocked() const {
        if (state != Running) return clockPos;
        LARGE_INTEGER q, f;
        QueryPerformanceCounter(&q);
        QueryPerformanceFrequency(&f);
        return clockPos + (q.QuadPart - clockQpc.QuadPart) * 10000000 / f.QuadPart;
    }
    void setClockLocked(LONGLONG pos) {
        clockPos = pos;
        QueryPerformanceCounter(&clockQpc);
    }
    void postEvent(long code, uint32_t p1, uint32_t p2) {
        {
            std::lock_guard<std::mutex> l(em);
            events.push_back({code, p1, p2});
            SetEvent(eventHandle);
        }
        if (notifyWnd) PostMessageW(notifyWnd, notifyMsg, 0, static_cast<LPARAM>(static_cast<int32_t>(notifyParam)));
        HLOG(1, "movie event 0x%lX", code);
    }
};

std::mutex g_graphsLock;
std::vector<Graph*> g_graphs;
com::Class* g_cls[I_Count];

Graph* graphOf(Ctx* c) {
    std::lock_guard<std::mutex> l(g_graphsLock);
    const uint32_t id = rd32(arg(c, 0) + 4);
    return id < g_graphs.size() ? g_graphs[id] : nullptr;
}
uint32_t iface(Graph* g, Iface i) { return g->guest + 8 * i; }

// ---------------------------------------------------------------------------
// audio (waveOut)
// ---------------------------------------------------------------------------
void waveReclaim(Graph* g, bool all) {
    while (!g->waveQueue.empty()) {
        WAVEHDR* h = g->waveQueue.front();
        if (!all && !(h->dwFlags & WHDR_DONE)) break;
        waveOutUnprepareHeader(g->wave, h, sizeof *h);
        delete[] h->lpData;
        delete h;
        g->waveQueue.pop_front();
    }
}
uint64_t wavePlayedBytes(Graph* g) {
    if (!g->wave) return 0;
    MMTIME t{};
    t.wType = TIME_BYTES;
    waveOutGetPosition(g->wave, &t, sizeof t);
    return t.wType == TIME_BYTES ? t.u.cb : 0;
}
void waveWrite(Graph* g, const uint8_t* p, DWORD n) {
    if (!g->wave || !n) return;
    auto* h = new WAVEHDR{};
    h->lpData = new char[n];
    std::memcpy(h->lpData, p, n);
    if (g->volume < 0 && g->wfx.wBitsPerSample == 16) {
        const float gain = std::pow(10.0f, g->volume / 2000.0f);
        auto* s = reinterpret_cast<int16_t*>(h->lpData);
        for (DWORD i = 0; i < n / 2; ++i) s[i] = static_cast<int16_t>(s[i] * gain);
    }
    h->dwBufferLength = n;
    waveOutPrepareHeader(g->wave, h, sizeof *h);
    waveOutWrite(g->wave, h, sizeof *h);
    g->waveQueue.push_back(h);
    g->waveBytesWritten += n;
}
void waveFlush(Graph* g) {
    if (!g->wave) return;
    waveOutReset(g->wave);
    waveReclaim(g, true);
    g->waveBytesWritten = 0;
}

// ---------------------------------------------------------------------------
// Media Foundation source
// ---------------------------------------------------------------------------
bool configureVideo(Graph* g) {
    const GUID tries[] = {MFVideoFormat_RGB32, MFVideoFormat_NV12, MFVideoFormat_YUY2};
    for (const GUID& sub : tries) {
        IMFMediaType* t = nullptr;
        MFCreateMediaType(&t);
        t->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
        t->SetGUID(MF_MT_SUBTYPE, sub);
        const HRESULT hr = g->reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, nullptr, t);
        t->Release();
        if (SUCCEEDED(hr)) {
            g->srcFormat = sub == MFVideoFormat_RGB32 ? Graph::SRC_RGB32 : sub == MFVideoFormat_NV12 ? Graph::SRC_NV12 : Graph::SRC_YUY2;
            IMFMediaType* cur = nullptr;
            g->reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &cur);
            MFGetAttributeSize(cur, MF_MT_FRAME_SIZE, &g->width, &g->height);
            cur->Release();
            return g->width && g->height;
        }
    }
    return false;
}
bool configureAudio(Graph* g) {
    IMFMediaType* t = nullptr;
    MFCreateMediaType(&t);
    t->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
    t->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
    t->SetUINT32(MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
    HRESULT hr = g->reader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, t);
    t->Release();
    if (FAILED(hr)) return false;
    IMFMediaType* cur = nullptr;
    g->reader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &cur);
    WAVEFORMATEX* w = nullptr;
    UINT32 size = 0;
    hr = MFCreateWaveFormatExFromMFMediaType(cur, &w, &size);
    cur->Release();
    if (FAILED(hr)) return false;
    g->wfx = *w;
    g->wfx.wFormatTag = WAVE_FORMAT_PCM;
    g->wfx.cbSize = 0;
    CoTaskMemFree(w);
    return waveOutOpen(&g->wave, WAVE_MAPPER, &g->wfx, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR;
}

uint8_t clamp8(int v) { return static_cast<uint8_t>(v < 0 ? 0 : v > 255 ? 255 : v); }
void yuvToBgr(int y, int u, int v, uint8_t* o) {
    const int c = y - 16, d = u - 128, e = v - 128;
    o[2] = clamp8((298 * c + 409 * e + 128) >> 8);
    o[1] = clamp8((298 * c - 100 * d - 208 * e + 128) >> 8);
    o[0] = clamp8((298 * c + 516 * d + 128) >> 8);
}

// Decoded frame -> RGB24 bottom-up DIB (what DirectShow hands an RGB24 renderer).
void convertFrame(Graph* g, IMFSample* s, std::vector<uint8_t>& out) {
    IMFMediaBuffer* b = nullptr;
    if (FAILED(s->ConvertToContiguousBuffer(&b))) return;
    BYTE* p = nullptr;
    DWORD len = 0;
    b->Lock(&p, nullptr, &len);
    const UINT32 w = g->width, h = g->height;
    out.assign(static_cast<size_t>(g->stride) * h, 0);
    if (g->srcFormat == Graph::SRC_RGB32) {
        const DWORD pitch = len / h >= w * 4 ? len / h : w * 4;
        for (UINT32 y = 0; y < h && (y + 1) * pitch <= len; ++y) {
            const uint8_t* src = p + y * pitch;
            uint8_t* dst = out.data() + static_cast<size_t>(h - 1 - y) * g->stride;
            for (UINT32 x = 0; x < w; ++x) std::memcpy(dst + 3 * x, src + 4 * x, 3);
        }
    } else if (g->srcFormat == Graph::SRC_NV12) {
        const DWORD pitch = w;  // contiguous NV12: Y plane then interleaved UV
        if (len >= pitch * h * 3 / 2)
            for (UINT32 y = 0; y < h; ++y) {
                const uint8_t* yr = p + y * pitch;
                const uint8_t* uv = p + pitch * h + (y / 2) * pitch;
                uint8_t* dst = out.data() + static_cast<size_t>(h - 1 - y) * g->stride;
                for (UINT32 x = 0; x < w; ++x) yuvToBgr(yr[x], uv[x & ~1u], uv[x | 1u], dst + 3 * x);
            }
    } else {
        const DWORD pitch = w * 2;
        if (len >= pitch * h)
            for (UINT32 y = 0; y < h; ++y) {
                const uint8_t* r = p + y * pitch;
                uint8_t* dst = out.data() + static_cast<size_t>(h - 1 - y) * g->stride;
                for (UINT32 x = 0; x < w; ++x) yuvToBgr(r[2 * x], r[(4 * (x / 2)) + 1], r[(4 * (x / 2)) + 3], dst + 3 * x);
            }
    }
    b->Unlock();
    b->Release();
}

HRESULT openSource(Graph* g, const wchar_t* file) {
    static std::once_flag once;
    std::call_once(once, [] { MFStartup(MF_VERSION, MFSTARTUP_FULL); });
    wchar_t full[MAX_PATH * 2];
    GetFullPathNameW(file, MAX_PATH * 2, full, nullptr);
    IMFAttributes* a = nullptr;
    MFCreateAttributes(&a, 2);
    a->SetUINT32(MF_SOURCE_READER_ENABLE_VIDEO_PROCESSING, TRUE);
    const HRESULT hr = MFCreateSourceReaderFromURL(full, a, &g->reader);
    a->Release();
    if (FAILED(hr)) {
        log("movie: cannot open %s (0x%08lX)", narrow(full).c_str(), static_cast<unsigned long>(hr));
        return VFW_E_NOT_FOUND;
    }
    g->reader->SetStreamSelection(MF_SOURCE_READER_ALL_STREAMS, FALSE);
    g->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, TRUE);
    g->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, TRUE);
    g->hasVideo = configureVideo(g);
    g->hasAudio = configureAudio(g);
    if (!g->hasVideo) g->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_VIDEO_STREAM, FALSE);
    if (!g->hasAudio) g->reader->SetStreamSelection(MF_SOURCE_READER_FIRST_AUDIO_STREAM, FALSE);
    PROPVARIANT v;
    PropVariantInit(&v);
    if (SUCCEEDED(g->reader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &v))) g->duration = v.uhVal.QuadPart;
    PropVariantClear(&v);
    g->stride = (g->width * 3 + 3) & ~3u;
    log("movie: %s %ux%u (%s) %s, %.2f s", narrow(full).c_str(), g->width, g->height,
        g->srcFormat == Graph::SRC_RGB32 ? "RGB32" : g->srcFormat == Graph::SRC_NV12 ? "NV12" : "YUY2", g->hasAudio ? "with audio" : "no audio",
        g->duration / 1e7);
    return g->hasVideo || g->hasAudio ? S_OK : VFW_E_CANNOT_RENDER;
}

// Connects our output pin to the game's renderer ("In" pin) with RGB24 / VIDEOINFOHEADER.
HRESULT connectRenderer(Graph* g) {
    if (!g->hasVideo) return S_OK;
    // x86 AM_MEDIA_TYPE (72 bytes) and VIDEOINFOHEADER (88 bytes)
    const uint32_t vih = gcalloc(1, 88), mt = gcalloc(1, 72);
    wr32(vih + 8, g->width), wr32(vih + 12, g->height);   // rcSource
    wr32(vih + 24, g->width), wr32(vih + 28, g->height);  // rcTarget
    wr64(vih + 40, 333333);                              // AvgTimePerFrame
    wr32(vih + 48, 40);                                  // biSize
    wr32(vih + 52, g->width), wr32(vih + 56, g->height);
    wr16(vih + 60, 1), wr16(vih + 62, 24);
    wr32(vih + 68, g->stride * g->height);  // biSizeImage
    std::memcpy(gp(mt), &MEDIATYPE_Video, 16);
    std::memcpy(gp(mt + 16), &MEDIASUBTYPE_RGB24, 16);
    wr32(mt + 32, 1);                       // bFixedSizeSamples
    wr32(mt + 40, g->stride * g->height);   // lSampleSize
    std::memcpy(gp(mt + 44), &FORMAT_VideoInfo, 16);
    wr32(mt + 64, 88), wr32(mt + 68, vih);  // cbFormat, pbFormat
    g->mediaType = mt, g->videoInfo = vih;
    g->sampleBuf = gcalloc(1, g->stride * g->height);

    const uint32_t name = gwstr(L"In"), out = gcalloc(1, 4);
    HRESULT hr = VFW_E_CANNOT_RENDER;
    for (auto& [f, n] : g->filters) {
        wr32(out, 0);
        if (static_cast<int32_t>(gcall(f, BF_FindPin, {name, out})) < 0 || !rd32(out)) continue;
        const uint32_t pin = rd32(out);
        hr = static_cast<HRESULT>(gcall(pin, PIN_ReceiveConnection, {iface(g, I_Pin), mt}));
        HLOG(1, "movie: %s.In ReceiveConnection(RGB24 %ux%u) -> 0x%08lX", narrow(n.c_str()).c_str(), g->width, g->height,
             static_cast<unsigned long>(hr));
        if (SUCCEEDED(hr)) {
            g->rendererFilter = f, g->rendererPin = pin;
            g->memInput = gQuery(pin, IID_IMemInputPin);
            break;
        }
        gRelease(pin);
    }
    gfree(out);
    gfree(name);
    return SUCCEEDED(hr) ? S_OK : VFW_S_PARTIAL_RENDER;
}

// ---------------------------------------------------------------------------
// streaming thread
// ---------------------------------------------------------------------------
void deliver(Graph* g, VideoFrame& f) {
    if (!g->memInput) return;
    std::memcpy(gp(g->sampleBuf), f.rgb24.data(), f.rgb24.size());
    // the sample's times (read through IMediaSample::GetTime)
    wr64(g->guest + 8 * I_Count, static_cast<uint64_t>(f.time));
    gcall(g->memInput, MIP_Receive, {iface(g, I_Sample)});
}

void streamThread(Graph* g) {
    std::unique_lock<std::mutex> l(g->m);
    while (!g->quit) {
        if (g->seekPending) {
            const LONGLONG to = g->seekTo;
            g->seekPending = false, g->complete = false, g->videoEos = !g->hasVideo, g->audioEos = !g->hasAudio;
            g->frames.clear();
            waveFlush(g);
            PROPVARIANT v;
            PropVariantInit(&v);
            v.vt = VT_I8;
            v.hVal.QuadPart = to;
            g->reader->SetCurrentPosition(GUID_NULL, v);
            g->setClockLocked(to);
            if (g->wave) { waveOutPause(g->wave); if (g->state == Graph::Running) waveOutRestart(g->wave); }
        }
        if (g->state != Graph::Running || g->complete) {
            g->cv.wait(l);
            continue;
        }
        // decode ahead: a few frames of video and ~half a second of audio
        const LONGLONG now = g->nowLocked();
        while (!(g->videoEos && g->audioEos) && g->frames.size() < 30) {
            const double audioAhead = g->hasAudio && !g->audioEos && g->wfx.nAvgBytesPerSec
                                          ? static_cast<double>(g->waveBytesWritten - wavePlayedBytes(g)) / g->wfx.nAvgBytesPerSec
                                          : 1.0;
            if ((g->videoEos || g->frames.size() >= 4) && (audioAhead >= 0.5 || g->audioEos)) break;
            DWORD stream = 0, flags = 0;
            LONGLONG ts = 0;
            IMFSample* s = nullptr;
            l.unlock();
            const HRESULT hr = g->reader->ReadSample(MF_SOURCE_READER_ANY_STREAM, 0, &stream, &flags, &ts, &s);
            l.lock();
            if (g->seekPending || g->quit) { if (s) s->Release(); break; }
            if (FAILED(hr)) { g->videoEos = g->audioEos = true; break; }
            // map the actual stream index to video/audio by its major type
            IMFMediaType* t = nullptr;
            bool isVideo = false;
            if (SUCCEEDED(g->reader->GetCurrentMediaType(stream, &t))) {
                GUID major{};
                t->GetGUID(MF_MT_MAJOR_TYPE, &major);
                isVideo = major == MFMediaType_Video;
                t->Release();
            }
            if (flags & MF_SOURCE_READERF_ERROR) g->videoEos = g->audioEos = true;
            if (flags & MF_SOURCE_READERF_ENDOFSTREAM) (isVideo ? g->videoEos : g->audioEos) = true;
            if (s) {
                if (isVideo) {
                    VideoFrame f{ts, {}};
                    convertFrame(g, s, f.rgb24);
                    g->frames.push_back(std::move(f));
                } else {
                    IMFMediaBuffer* b = nullptr;
                    if (SUCCEEDED(s->ConvertToContiguousBuffer(&b))) {
                        BYTE* p = nullptr;
                        DWORD n = 0;
                        b->Lock(&p, nullptr, &n);
                        waveWrite(g, p, n);
                        b->Unlock();
                        b->Release();
                    }
                }
                s->Release();
            }
        }
        if (g->wave) waveReclaim(g, false);
        if (g->seekPending || g->quit) continue;
        if (g->stopAt && now >= g->stopAt) g->videoEos = g->audioEos = true, g->frames.clear();

        // present due frames (drop ones more than 150 ms late, but always show the newest due one)
        if (!g->frames.empty() && g->frames.front().time <= now) {
            while (g->frames.size() > 1 && g->frames[1].time <= now && now - g->frames.front().time > 1500000) g->frames.pop_front();
            VideoFrame f = std::move(g->frames.front());
            g->frames.pop_front();
            l.unlock();
            deliver(g, f);
            l.lock();
            continue;
        }
        const bool audioDone = !g->hasAudio || wavePlayedBytes(g) >= g->waveBytesWritten;
        if (g->videoEos && g->audioEos && g->frames.empty() && audioDone) {
            g->complete = true;
            l.unlock();
            if (g->rendererPin) gcall(g->rendererPin, PIN_EndOfStream, {});
            g->postEvent(EC_COMPLETE, S_OK, 0);
            l.lock();
            continue;
        }
        LONGLONG wait = g->frames.empty() ? 50000 : g->frames.front().time - now;
        if (wait > 100000) wait = 100000;
        if (wait < 10000) wait = 10000;
        g->cv.wait_for(l, std::chrono::microseconds(wait / 10));
    }
}

// ---------------------------------------------------------------------------
// state changes
// ---------------------------------------------------------------------------
void filtersCall(Graph* g, int slot, std::initializer_list<uint32_t> args) {
    for (auto& f : g->filters) gcall(f.first, slot, args);
}

HRESULT doPause(Graph* g) {
    std::unique_lock<std::mutex> l(g->m);
    if (g->state == Graph::Running) {
        g->clockPos = g->nowLocked();
        if (g->wave) waveOutPause(g->wave);
    }
    g->state = Graph::Paused;
    l.unlock();
    filtersCall(g, BF_Pause, {});
    g->cv.notify_all();
    return S_OK;
}
HRESULT doRun(Graph* g) {
    if (g->state == Graph::Stopped) doPause(g);
    filtersCall(g, BF_Run, {0, 0});
    {
        std::lock_guard<std::mutex> l(g->m);
        g->setClockLocked(g->clockPos);
        g->state = Graph::Running;
        if (g->wave) waveOutRestart(g->wave);
        if (!g->worker.joinable() && g->reader) g->worker = std::thread(streamThread, g);
    }
    g->cv.notify_all();
    return S_OK;
}
HRESULT doStop(Graph* g) {
    {
        std::lock_guard<std::mutex> l(g->m);
        if (g->state == Graph::Running) g->clockPos = g->nowLocked();
        g->state = Graph::Stopped;
        if (g->wave) waveOutPause(g->wave);
    }
    filtersCall(g, BF_Stop, {});  // releases a Receive blocked in the renderer
    g->cv.notify_all();
    return S_OK;
}
// A seek flushes the renderer (which also clears its end-of-stream state), like the
// graph manager does.
void doSeek(Graph* g, LONGLONG to) {
    if (g->rendererPin) gcall(g->rendererPin, PIN_BeginFlush, {});
    {
        std::lock_guard<std::mutex> l(g->m);
        g->seekTo = to < 0 ? 0 : to;
        g->seekPending = true;
        g->clockPos = g->seekTo;
    }
    g->cv.notify_all();
    if (g->rendererPin) gcall(g->rendererPin, PIN_EndFlush, {});
}
LONGLONG position(Graph* g) {
    std::lock_guard<std::mutex> l(g->m);
    if (g->seekPending) return g->seekTo;
    const LONGLONG p = g->nowLocked();
    return g->duration && p > g->duration ? g->duration : p;
}

void destroy(Graph* g) {
    doStop(g);
    {
        std::lock_guard<std::mutex> l(g->m);
        g->quit = true;
    }
    g->cv.notify_all();
    if (g->worker.joinable()) g->worker.join();
    if (g->rendererPin) {
        gcall(g->rendererPin, PIN_Disconnect, {});
        gRelease(g->memInput);
        gRelease(g->rendererPin);
    }
    for (auto& f : g->filters) {
        gcall(f.first, BF_JoinFilterGraph, {0, 0});
        gRelease(f.first);
    }
    g->filters.clear();
    if (g->wave) { waveFlush(g); waveOutClose(g->wave); }
    if (g->reader) g->reader->Release();
    CloseHandle(g->eventHandle);
    HLOG(1, "movie graph %u destroyed", g->id);
    // The guest block and the Graph record stay allocated: stale guest pointers then hit
    // a harmless object rather than reused memory.
    g->reader = nullptr;
}

// ---------------------------------------------------------------------------
// IUnknown (all interfaces)
// ---------------------------------------------------------------------------
void ret(Ctx* c, HRESULT hr, int n) { retStd(c, static_cast<uint32_t>(hr), n); }

void qi(Ctx* c) {
    Graph* g = graphOf(c);
    const GUID& iid = *gp<GUID>(arg(c, 1));
    const uint32_t out = arg(c, 2);
    const uint32_t self = arg(c, 0);
    const bool isPin = self == iface(g, I_Pin), isSample = self == iface(g, I_Sample);
    int slot = -1;
    if (isPin) slot = iid == IID_IUnknown || iid == IID_IPin ? I_Pin : -1;
    else if (isSample) slot = iid == IID_IUnknown || iid == IID_IMediaSample ? I_Sample : -1;
    else if (iid == IID_IUnknown || iid == IID_IGraphBuilder || iid == IID_IFilterGraph) slot = I_Graph;
    else if (iid == IID_IMediaControl) slot = I_Control;
    else if (iid == IID_IMediaPosition) slot = I_Position;
    else if (iid == IID_IMediaSeeking) slot = I_Seeking;
    else if (iid == IID_IMediaEventEx || iid == IID_IMediaEvent) slot = I_Event;
    else if (iid == IID_IMediaEventSink) slot = I_Sink;
    else if (iid == IID_IBasicAudio) slot = I_Audio;
    if (slot < 0) {
        if (out) wr32(out, 0);
        HLOG(2, "movie: QueryInterface({%08lX-...}) -> E_NOINTERFACE", iid.Data1);
        ret(c, E_NOINTERFACE, 3);
        return;
    }
    if (slot != I_Pin && slot != I_Sample) ++g->refs;
    if (out) wr32(out, iface(g, static_cast<Iface>(slot)));
    ret(c, S_OK, 3);
}
void addRef(Ctx* c) {
    Graph* g = graphOf(c);
    const bool inner = arg(c, 0) == iface(g, I_Pin) || arg(c, 0) == iface(g, I_Sample);
    retStd(c, inner ? 2 : static_cast<uint32_t>(++g->refs), 1);
}
void release(Ctx* c) {
    Graph* g = graphOf(c);
    if (arg(c, 0) == iface(g, I_Pin) || arg(c, 0) == iface(g, I_Sample)) { retStd(c, 1, 1); return; }
    const long n = --g->refs;
    if (n == 0) destroy(g);
    retStd(c, static_cast<uint32_t>(n < 0 ? 0 : n), 1);
}
void notImpl(Ctx* c, uintptr_t args) {
    HLOG(1, "movie: unimplemented method called from 0x%08X", rd32(c->esp));
    ret(c, E_NOTIMPL, static_cast<int>(args));
}

// IDispatch (4 methods) for the automation interfaces
void dispTypeInfoCount(Ctx* c) { if (arg(c, 1)) wr32(arg(c, 1), 0); ret(c, S_OK, 2); }

// ---------------------------------------------------------------------------
// IGraphBuilder
// ---------------------------------------------------------------------------
void gbAddFilter(Ctx* c) {
    Graph* g = graphOf(c);
    const uint32_t f = arg(c, 1);
    const std::wstring name = arg(c, 2) ? argp<wchar_t>(c, 2) : L"";
    gAddRef(f);
    g->filters.emplace_back(f, name);
    const uint32_t gname = gwstr(name.c_str());
    const uint32_t hr = gcall(f, BF_JoinFilterGraph, {iface(g, I_Graph), gname});
    HLOG(1, "movie: AddFilter(%s) -> JoinFilterGraph 0x%08X", narrow(name.c_str()).c_str(), hr);
    ret(c, S_OK, 3);
}
void gbRemoveFilter(Ctx* c) {
    Graph* g = graphOf(c);
    for (auto it = g->filters.begin(); it != g->filters.end(); ++it)
        if (it->first == arg(c, 1)) {
            if (it->first == g->rendererFilter && g->rendererPin) {
                gcall(g->rendererPin, PIN_Disconnect, {});
                gRelease(g->memInput), gRelease(g->rendererPin);
                g->memInput = g->rendererPin = g->rendererFilter = 0;
            }
            gcall(it->first, BF_JoinFilterGraph, {0, 0});
            gRelease(it->first);
            g->filters.erase(it);
            ret(c, S_OK, 2);
            return;
        }
    ret(c, VFW_E_NOT_FOUND, 2);
}
void gbFindFilterByName(Ctx* c) {
    Graph* g = graphOf(c);
    const std::wstring n = argp<wchar_t>(c, 1);
    for (auto& f : g->filters)
        if (f.second == n) { gAddRef(f.first); wr32(arg(c, 2), f.first); ret(c, S_OK, 3); return; }
    wr32(arg(c, 2), 0);
    ret(c, VFW_E_NOT_FOUND, 3);
}
HRESULT renderFile(Graph* g, const wchar_t* file) {
    if (g->reader) return VFW_E_ALREADY_CONNECTED;
    HRESULT hr = openSource(g, file);
    if (FAILED(hr)) return hr;
    hr = connectRenderer(g);
    return hr;
}
void gbRenderFile(Ctx* c) { ret(c, renderFile(graphOf(c), argp<wchar_t>(c, 1)), 3); }
void gbSetDefaultSyncSource(Ctx* c) { ret(c, S_OK, 1); }
void gbAbort(Ctx* c) { ret(c, S_OK, 1); }
void gbShouldContinue(Ctx* c) { ret(c, S_OK, 1); }
void gbSetLogFile(Ctx* c) { ret(c, S_OK, 2); }

// ---------------------------------------------------------------------------
// IMediaControl
// ---------------------------------------------------------------------------
void mcRun(Ctx* c) { ret(c, doRun(graphOf(c)), 1); }
void mcPause(Ctx* c) { ret(c, doPause(graphOf(c)), 1); }
void mcStop(Ctx* c) { ret(c, doStop(graphOf(c)), 1); }
void mcGetState(Ctx* c) {
    Graph* g = graphOf(c);
    if (arg(c, 2)) wr32(arg(c, 2), g->state == Graph::Running ? State_Running : g->state == Graph::Paused ? State_Paused : State_Stopped);
    ret(c, S_OK, 3);
}
void mcRenderFile(Ctx* c) { ret(c, renderFile(graphOf(c), argp<wchar_t>(c, 1)), 2); }
void mcStopWhenReady(Ctx* c) { ret(c, doStop(graphOf(c)), 1); }

// ---------------------------------------------------------------------------
// IMediaPosition (REFTIME = double seconds)
// ---------------------------------------------------------------------------
void mpGetDuration(Ctx* c) { wrf64(arg(c, 1), graphOf(c)->duration / 1e7); ret(c, S_OK, 2); }
void mpPutCurrent(Ctx* c) { doSeek(graphOf(c), static_cast<LONGLONG>(rdf64(c->esp + 8) * 1e7)); ret(c, S_OK, 3); }
void mpGetCurrent(Ctx* c) { wrf64(arg(c, 1), position(graphOf(c)) / 1e7); ret(c, S_OK, 2); }
void mpGetStop(Ctx* c) {
    Graph* g = graphOf(c);
    wrf64(arg(c, 1), (g->stopAt ? g->stopAt : g->duration) / 1e7);
    ret(c, S_OK, 2);
}
void mpPutStop(Ctx* c) {
    Graph* g = graphOf(c);
    const LONGLONG t = static_cast<LONGLONG>(rdf64(c->esp + 8) * 1e7);
    g->stopAt = t >= g->duration ? 0 : t;
    ret(c, S_OK, 3);
}
void mpGetPreroll(Ctx* c) { wrf64(arg(c, 1), 0.0); ret(c, S_OK, 2); }
void mpPutDouble(Ctx* c) { ret(c, S_OK, 3); }
void mpGetRate(Ctx* c) { wrf64(arg(c, 1), 1.0); ret(c, S_OK, 2); }
void mpCanSeek(Ctx* c) { if (arg(c, 1)) wr32(arg(c, 1), static_cast<uint32_t>(-1)); ret(c, S_OK, 2); }  // OATRUE

// ---------------------------------------------------------------------------
// IMediaSeeking (TIME_FORMAT_MEDIA_TIME only)
// ---------------------------------------------------------------------------
constexpr DWORD kSeekCaps = AM_SEEKING_CanSeekAbsolute | AM_SEEKING_CanSeekForwards | AM_SEEKING_CanSeekBackwards |
                            AM_SEEKING_CanGetCurrentPos | AM_SEEKING_CanGetStopPos | AM_SEEKING_CanGetDuration;
void msGetCaps(Ctx* c) { wr32(arg(c, 1), kSeekCaps); ret(c, S_OK, 2); }
void msCheckCaps(Ctx* c) {
    const DWORD want = rd32(arg(c, 1)), have = want & kSeekCaps;
    wr32(arg(c, 1), have);
    ret(c, have == want ? S_OK : have ? S_FALSE : E_FAIL, 2);
}
void msIsFormatSupported(Ctx* c) { ret(c, *gp<GUID>(arg(c, 1)) == TIME_FORMAT_MEDIA_TIME ? S_OK : S_FALSE, 2); }
void msGetFormat(Ctx* c) { std::memcpy(gp(arg(c, 1)), &TIME_FORMAT_MEDIA_TIME, 16); ret(c, S_OK, 2); }
void msSetFormat(Ctx* c) { ret(c, *gp<GUID>(arg(c, 1)) == TIME_FORMAT_MEDIA_TIME ? S_OK : E_INVALIDARG, 2); }
void msGetDuration(Ctx* c) { wr64(arg(c, 1), graphOf(c)->duration); ret(c, S_OK, 2); }
void msGetStop(Ctx* c) {
    Graph* g = graphOf(c);
    wr64(arg(c, 1), g->stopAt ? g->stopAt : g->duration);
    ret(c, S_OK, 2);
}
void msGetCurrent(Ctx* c) { wr64(arg(c, 1), position(graphOf(c))); ret(c, S_OK, 2); }
void msConvertTimeFormat(Ctx* c) { wr64(arg(c, 1), rd64(c->esp + 16)); ret(c, S_OK, 6); }  // (this, out, target fmt, 8-byte src, src fmt)
// SetPositions(this, pCurrent, dwCurrentFlags, pStop, dwStopFlags)
void msSetPositions(Ctx* c) {
    Graph* g = graphOf(c);
    const DWORD cf = arg(c, 2), sf = arg(c, 4);
    if ((cf & AM_SEEKING_PositioningBitsMask) && arg(c, 1)) {
        LONGLONG t = static_cast<LONGLONG>(rd64(arg(c, 1)));
        if ((cf & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning) t += position(g);
        doSeek(g, t);
    }
    if ((sf & AM_SEEKING_PositioningBitsMask) && arg(c, 3)) {
        LONGLONG t = static_cast<LONGLONG>(rd64(arg(c, 3)));
        if ((sf & AM_SEEKING_PositioningBitsMask) == AM_SEEKING_RelativePositioning) t += g->stopAt ? g->stopAt : g->duration;
        g->stopAt = t >= g->duration ? 0 : t;
    }
    ret(c, S_OK, 5);
}
void msGetPositions(Ctx* c) {
    Graph* g = graphOf(c);
    if (arg(c, 1)) wr64(arg(c, 1), position(g));
    if (arg(c, 2)) wr64(arg(c, 2), g->stopAt ? g->stopAt : g->duration);
    ret(c, S_OK, 3);
}
void msGetAvailable(Ctx* c) {
    if (arg(c, 1)) wr64(arg(c, 1), 0);
    if (arg(c, 2)) wr64(arg(c, 2), graphOf(c)->duration);
    ret(c, S_OK, 3);
}
void msSetRate(Ctx* c) { ret(c, rdf64(c->esp + 8) == 1.0 ? S_OK : E_NOTIMPL, 3); }
void msGetRate(Ctx* c) { wrf64(arg(c, 1), 1.0); ret(c, S_OK, 2); }
void msGetPreroll(Ctx* c) { wr64(arg(c, 1), 0); ret(c, S_OK, 2); }

// ---------------------------------------------------------------------------
// IMediaEventEx / IMediaEventSink
// ---------------------------------------------------------------------------
void meGetEventHandle(Ctx* c) { wr32(arg(c, 1), gh(graphOf(c)->eventHandle)); ret(c, S_OK, 2); }
// GetEvent(this, &code, &p1, &p2, msTimeout)
void meGetEvent(Ctx* c) {
    Graph* g = graphOf(c);
    if (WaitForSingleObject(g->eventHandle, arg(c, 4)) != WAIT_OBJECT_0) { ret(c, E_ABORT, 5); return; }
    std::lock_guard<std::mutex> l(g->em);
    if (g->events.empty()) { ResetEvent(g->eventHandle); ret(c, E_ABORT, 5); return; }
    const Event e = g->events.front();
    g->events.pop_front();
    if (g->events.empty()) ResetEvent(g->eventHandle);
    wr32(arg(c, 1), static_cast<uint32_t>(e.code));
    wr32(arg(c, 2), e.p1);
    wr32(arg(c, 3), e.p2);
    ret(c, S_OK, 5);
}
// WaitForCompletion(this, msTimeout, &code)
void meWaitForCompletion(Ctx* c) {
    Graph* g = graphOf(c);
    const DWORD timeout = arg(c, 1), start = GetTickCount();
    for (;;) {
        {
            std::lock_guard<std::mutex> l(g->m);
            if (g->complete) { if (arg(c, 2)) wr32(arg(c, 2), EC_COMPLETE); ret(c, S_OK, 3); return; }
        }
        if (timeout != INFINITE && GetTickCount() - start >= timeout) break;
        Sleep(10);
    }
    if (arg(c, 2)) wr32(arg(c, 2), 0);
    ret(c, E_ABORT, 3);
}
void meCancelDefault(Ctx* c) { ret(c, S_OK, 2); }
void meFreeParams(Ctx* c) { ret(c, S_OK, 4); }
// SetNotifyWindow(this, OAHWND, msg, LONG_PTR)
void meSetNotifyWindow(Ctx* c) {
    Graph* g = graphOf(c);
    g->notifyWnd = static_cast<HWND>(hh(arg(c, 1)));
    g->notifyMsg = arg(c, 2);
    g->notifyParam = arg(c, 3);
    ret(c, S_OK, 4);
}
void meSetNotifyFlags(Ctx* c) { ret(c, S_OK, 2); }
void meGetNotifyFlags(Ctx* c) { wr32(arg(c, 1), 0); ret(c, S_OK, 2); }
// Notify(this, code, p1, p2) from the guest renderer. The graph manager's default
// handling keeps renderer events from the application; EC_COMPLETE is posted by the
// streaming thread once every stream has finished.
void sinkNotify(Ctx* c) {
    HLOG(2, "movie: renderer event 0x%X", arg(c, 1));
    const uint32_t code = arg(c, 1);
    if (code == EC_ERRORABORT || code == EC_USERABORT) graphOf(c)->postEvent(code, arg(c, 2), arg(c, 3));
    ret(c, S_OK, 4);
}

// ---------------------------------------------------------------------------
// IBasicAudio
// ---------------------------------------------------------------------------
void baPutVolume(Ctx* c) { graphOf(c)->volume = static_cast<int32_t>(arg(c, 1)); ret(c, S_OK, 2); }
void baGetVolume(Ctx* c) { wr32(arg(c, 1), static_cast<uint32_t>(graphOf(c)->volume)); ret(c, S_OK, 2); }
void baPutBalance(Ctx* c) { ret(c, S_OK, 2); }
void baGetBalance(Ctx* c) { wr32(arg(c, 1), 0); ret(c, S_OK, 2); }

// ---------------------------------------------------------------------------
// our output pin (the renderer queries it while connecting)
// ---------------------------------------------------------------------------
void pinConnectedTo(Ctx* c) {
    Graph* g = graphOf(c);
    if (g->rendererPin) gAddRef(g->rendererPin);
    wr32(arg(c, 1), g->rendererPin);
    ret(c, g->rendererPin ? S_OK : VFW_E_NOT_CONNECTED, 2);
}
void copyMediaType(Graph* g, uint32_t dst) {
    std::memcpy(gp(dst), gp(g->mediaType), 72);
    const uint32_t fmt = gmalloc(88);  // CoTaskMemAlloc'd by contract; the guest frees it with CoTaskMemFree
    std::memcpy(gp(fmt), gp(g->videoInfo), 88);
    wr32(dst + 68, fmt);
}
void pinConnectionMediaType(Ctx* c) {
    Graph* g = graphOf(c);
    if (!g->mediaType) { ret(c, VFW_E_NOT_CONNECTED, 2); return; }
    copyMediaType(g, arg(c, 1));
    ret(c, S_OK, 2);
}
void pinQueryPinInfo(Ctx* c) {
    const uint32_t o = arg(c, 1);  // x86 PIN_INFO: pFilter, dir, achName[128]
    std::memset(gp(o), 0, 8 + 256);
    wr32(o + 4, PINDIR_OUTPUT);
    std::memcpy(gp(o + 8), L"Output", 14);
    ret(c, S_OK, 2);
}
void pinQueryDirection(Ctx* c) { wr32(arg(c, 1), PINDIR_OUTPUT); ret(c, S_OK, 2); }
void pinQueryId(Ctx* c) {
    const uint32_t s = gmalloc(14);
    std::memcpy(gp(s), L"Output", 14);
    wr32(arg(c, 1), s);
    ret(c, S_OK, 2);
}
void pinQueryAccept(Ctx* c) { ret(c, S_FALSE, 2); }
void pinOkay1(Ctx* c) { ret(c, S_OK, 1); }
void pinNewSegment(Ctx* c) { ret(c, S_OK, 7); }  // (this, start 8, stop 8, rate 8)

// ---------------------------------------------------------------------------
// the media sample (one per graph, reused; data in sampleBuf, times after the slots)
// ---------------------------------------------------------------------------
void smpGetPointer(Ctx* c) { wr32(arg(c, 1), graphOf(c)->sampleBuf); ret(c, S_OK, 2); }
void smpGetSize(Ctx* c) { Graph* g = graphOf(c); retStd(c, g->stride * g->height, 1); }
void smpGetTime(Ctx* c) {
    Graph* g = graphOf(c);
    const uint64_t t = rd64(g->guest + 8 * I_Count);
    if (arg(c, 1)) wr64(arg(c, 1), t);
    if (arg(c, 2)) wr64(arg(c, 2), t + 1);
    ret(c, S_OK, 3);
}
void smpSetTime(Ctx* c) { ret(c, S_OK, 3); }
void smpIsSyncPoint(Ctx* c) { ret(c, S_OK, 1); }
void smpSetFlag(Ctx* c) { ret(c, S_OK, 2); }
void smpIsFalse(Ctx* c) { ret(c, S_FALSE, 1); }
void smpGetActualLength(Ctx* c) { Graph* g = graphOf(c); retStd(c, g->stride * g->height, 1); }
void smpGetMediaType(Ctx* c) { wr32(arg(c, 1), 0); ret(c, S_FALSE, 2); }
void smpGetMediaTime(Ctx* c) { ret(c, VFW_E_MEDIA_TIME_NOT_SET, 3); }

// ---------------------------------------------------------------------------
// classes
// ---------------------------------------------------------------------------
#define NI(n) {"movie::unimplemented", nullptr, n}
struct M {
    const char* name;
    Handler fn;
    int nargs;  // for unimplemented methods: stack arguments including `this`
};

com::Class* makeIface(const char* name, std::vector<M> methods, bool dispatch = false) {
    std::vector<M> all{{"QueryInterface", qi, 0}, {"AddRef", addRef, 0}, {"Release", release, 0}};
    if (dispatch) {
        all.push_back({"GetTypeInfoCount", dispTypeInfoCount, 0});
        all.push_back(NI(4)), all.push_back(NI(6)), all.push_back(NI(9));  // GetTypeInfo, GetIDsOfNames, Invoke
    }
    all.insert(all.end(), methods.begin(), methods.end());
    auto* cls = new com::Class;
    cls->name = name;
    cls->vtbl = gcalloc(static_cast<uint32_t>(all.size()) + 1, 4);
    for (size_t i = 0; i < all.size(); ++i) {
        const std::string label = std::string(name) + "::" + all[i].name;
        const uint32_t t = all[i].fn ? addTrap(strdup(label.c_str()), all[i].fn)
                                     : addTrap(strdup(label.c_str()), notImpl, static_cast<uintptr_t>(all[i].nargs));
        wr32(cls->vtbl + 4 * static_cast<uint32_t>(i), t);
    }
    return cls;
}

void makeClasses() {
    static std::once_flag once;
    std::call_once(once, [] {
        g_cls[I_Graph] = makeIface("IGraphBuilder", {
            {"AddFilter", gbAddFilter, 0}, {"RemoveFilter", gbRemoveFilter, 0}, NI(2), {"FindFilterByName", gbFindFilterByName, 0},
            NI(4), NI(2), NI(2), {"SetDefaultSyncSource", gbSetDefaultSyncSource, 0},
            NI(3), NI(2), {"RenderFile", gbRenderFile, 0}, NI(4), {"SetLogFile", gbSetLogFile, 0}, {"Abort", gbAbort, 0},
            {"ShouldOperationContinue", gbShouldContinue, 0},
        });
        g_cls[I_Control] = makeIface("IMediaControl", {
            {"Run", mcRun, 0}, {"Pause", mcPause, 0}, {"Stop", mcStop, 0}, {"GetState", mcGetState, 0},
            {"RenderFile", mcRenderFile, 0}, NI(3), NI(2), NI(2), {"StopWhenReady", mcStopWhenReady, 0},
        }, true);
        g_cls[I_Position] = makeIface("IMediaPosition", {
            {"get_Duration", mpGetDuration, 0}, {"put_CurrentPosition", mpPutCurrent, 0}, {"get_CurrentPosition", mpGetCurrent, 0},
            {"get_StopTime", mpGetStop, 0}, {"put_StopTime", mpPutStop, 0}, {"get_PrerollTime", mpGetPreroll, 0},
            {"put_PrerollTime", mpPutDouble, 0}, {"put_Rate", mpPutDouble, 0}, {"get_Rate", mpGetRate, 0},
            {"CanSeekForward", mpCanSeek, 0}, {"CanSeekBackward", mpCanSeek, 0},
        }, true);
        g_cls[I_Seeking] = makeIface("IMediaSeeking", {
            {"GetCapabilities", msGetCaps, 0}, {"CheckCapabilities", msCheckCaps, 0}, {"IsFormatSupported", msIsFormatSupported, 0},
            {"QueryPreferredFormat", msGetFormat, 0}, {"GetTimeFormat", msGetFormat, 0}, {"IsUsingTimeFormat", msIsFormatSupported, 0},
            {"SetTimeFormat", msSetFormat, 0}, {"GetDuration", msGetDuration, 0}, {"GetStopPosition", msGetStop, 0},
            {"GetCurrentPosition", msGetCurrent, 0}, {"ConvertTimeFormat", msConvertTimeFormat, 0},
            {"SetPositions", msSetPositions, 0}, {"GetPositions", msGetPositions, 0}, {"GetAvailable", msGetAvailable, 0},
            {"SetRate", msSetRate, 0}, {"GetRate", msGetRate, 0}, {"GetPreroll", msGetPreroll, 0},
        });
        g_cls[I_Event] = makeIface("IMediaEventEx", {
            {"GetEventHandle", meGetEventHandle, 0}, {"GetEvent", meGetEvent, 0}, {"WaitForCompletion", meWaitForCompletion, 0},
            {"CancelDefaultHandling", meCancelDefault, 0}, {"RestoreDefaultHandling", meCancelDefault, 0},
            {"FreeEventParams", meFreeParams, 0}, {"SetNotifyWindow", meSetNotifyWindow, 0},
            {"SetNotifyFlags", meSetNotifyFlags, 0}, {"GetNotifyFlags", meGetNotifyFlags, 0},
        }, true);
        g_cls[I_Sink] = makeIface("IMediaEventSink", {{"Notify", sinkNotify, 0}});
        g_cls[I_Audio] = makeIface("IBasicAudio", {
            {"put_Volume", baPutVolume, 0}, {"get_Volume", baGetVolume, 0}, {"put_Balance", baPutBalance, 0}, {"get_Balance", baGetBalance, 0},
        }, true);
        g_cls[I_Pin] = makeIface("IPin(source)", {
            NI(3), NI(3), {"Disconnect", pinOkay1, 0}, {"ConnectedTo", pinConnectedTo, 0},
            {"ConnectionMediaType", pinConnectionMediaType, 0}, {"QueryPinInfo", pinQueryPinInfo, 0},
            {"QueryDirection", pinQueryDirection, 0}, {"QueryId", pinQueryId, 0}, {"QueryAccept", pinQueryAccept, 0},
            NI(2), NI(3), {"EndOfStream", pinOkay1, 0}, {"BeginFlush", pinOkay1, 0}, {"EndFlush", pinOkay1, 0},
            {"NewSegment", pinNewSegment, 0},
        });
        g_cls[I_Sample] = makeIface("IMediaSample", {
            {"GetPointer", smpGetPointer, 0}, {"GetSize", smpGetSize, 0}, {"GetTime", smpGetTime, 0}, {"SetTime", smpSetTime, 0},
            {"IsSyncPoint", smpIsSyncPoint, 0}, {"SetSyncPoint", smpSetFlag, 0}, {"IsPreroll", smpIsFalse, 0},
            {"SetPreroll", smpSetFlag, 0}, {"GetActualDataLength", smpGetActualLength, 0}, {"SetActualDataLength", smpSetFlag, 0},
            {"GetMediaType", smpGetMediaType, 0}, NI(2), {"IsDiscontinuity", smpIsFalse, 0}, {"SetDiscontinuity", smpSetFlag, 0},
            {"GetMediaTime", smpGetMediaTime, 0}, NI(3),
        });
    });
}

// CoCreateInstance(CLSID_FilterGraph)
HRESULT createGraph(REFIID iid, uint32_t out) {
    makeClasses();
    auto* g = new Graph;
    g->guest = gcalloc(1, 8 * I_Count + 8);  // + the current sample time
    {
        std::lock_guard<std::mutex> l(g_graphsLock);
        g->id = static_cast<uint32_t>(g_graphs.size());
        g_graphs.push_back(g);
    }
    for (int i = 0; i < I_Count; ++i) {
        wr32(g->guest + 8 * i, g_cls[i]->vtbl);
        wr32(g->guest + 8 * i + 4, g->id);
    }
    Iface slot = I_Graph;
    if (iid == IID_IMediaControl) slot = I_Control;
    else if (iid == IID_IMediaEventEx || iid == IID_IMediaEvent) slot = I_Event;
    else if (!(iid == IID_IGraphBuilder || iid == IID_IFilterGraph || iid == IID_IUnknown)) {
        if (out) wr32(out, 0);
        return E_NOINTERFACE;
    }
    if (out) wr32(out, iface(g, slot));
    return S_OK;
}
com::HostClassReg reg_FilterGraph(CLSID_FilterGraph, createGraph);

}  // namespace
