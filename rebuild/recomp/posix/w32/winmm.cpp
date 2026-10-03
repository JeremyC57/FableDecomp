// WINMM: timers and waveOut (on the shared SDL mixer).
#include "w32sdl.hpp"

#include <chrono>
#include <cstring>
#include <deque>
#include <map>
#include <mmsystem.h>
#include <thread>

namespace w32 {
namespace {

// ---- multimedia timers ----------------------------------------------------------------------
struct Timer {
    std::atomic<bool> alive{true};
};
std::mutex g_timerLock;
std::map<UINT, std::shared_ptr<Timer>> g_timers;
UINT g_nextTimer = 1;

// ---- waveOut ------------------------------------------------------------------------------
struct WaveOut : AudioSource {
    PcmFormat fmt;
    std::deque<WAVEHDR*> queue;
    size_t offset = 0;  // bytes consumed of queue.front()
    double frac = 0;    // resampling phase
    uint64_t played = 0;  // source frames consumed
    bool paused = false;
    void mix(float* out, int frames, int rate) override {
        if (paused) return;
        const double step = static_cast<double>(fmt.rate) / rate;
        const int bpf = fmt.channels * fmt.bits / 8;
        for (int i = 0; i < frames && !queue.empty(); ++i) {
            WAVEHDR* h = queue.front();
            float l, r;
            pcmFrame(fmt, reinterpret_cast<const uint8_t*>(h->lpData) + offset, l, r);
            out[2 * i] += l;
            out[2 * i + 1] += r;
            frac += step;
            while (frac >= 1.0 && !queue.empty()) {
                frac -= 1.0;
                offset += static_cast<size_t>(bpf);
                ++played;
                if (offset + static_cast<size_t>(bpf) > queue.front()->dwBufferLength) {
                    queue.front()->dwFlags = (queue.front()->dwFlags & ~WHDR_INQUEUE) | WHDR_DONE;
                    queue.pop_front();
                    offset = 0;
                }
            }
        }
    }
};
std::map<HWAVEOUT, std::shared_ptr<WaveOut>> g_waves;
uintptr_t g_nextWave = 0x7000;

std::shared_ptr<WaveOut> wave(HWAVEOUT h) {
    std::lock_guard<std::recursive_mutex> l(audioLock());
    auto it = g_waves.find(h);
    return it == g_waves.end() ? nullptr : it->second;
}

}  // namespace
}  // namespace w32

using namespace w32;

extern "C" {
DWORD WINAPI timeGetTime(void) { return GetTickCount(); }
MMRESULT WINAPI timeBeginPeriod(UINT) { return TIMERR_NOERROR; }
MMRESULT WINAPI timeEndPeriod(UINT) { return TIMERR_NOERROR; }
MMRESULT WINAPI timeSetEvent(UINT delay, UINT, LPTIMECALLBACK cb, DWORD_PTR user, UINT flags) {
    auto t = std::make_shared<Timer>();
    UINT id;
    {
        std::lock_guard<std::mutex> l(g_timerLock);
        id = g_nextTimer++;
        g_timers[id] = t;
    }
    std::thread([t, id, delay, cb, user, flags] {
        auto next = std::chrono::steady_clock::now();
        do {
            next += std::chrono::milliseconds(delay ? delay : 1);
            std::this_thread::sleep_until(next);
            if (!t->alive) break;
            if (flags & TIME_CALLBACK_EVENT_SET) SetEvent(reinterpret_cast<HANDLE>(cb));
            else if (flags & TIME_CALLBACK_EVENT_PULSE) PulseEvent(reinterpret_cast<HANDLE>(cb));
            else cb(id, 0, user, 0, 0);
        } while ((flags & TIME_PERIODIC) && t->alive);
    }).detach();
    return id;
}
MMRESULT WINAPI timeKillEvent(UINT id) {
    std::lock_guard<std::mutex> l(g_timerLock);
    auto it = g_timers.find(id);
    if (it == g_timers.end()) return MMSYSERR_INVALPARAM;
    it->second->alive = false;
    g_timers.erase(it);
    return TIMERR_NOERROR;
}

UINT WINAPI waveOutGetNumDevs(void) { return 1; }
MMRESULT WINAPI waveOutOpen(LPHWAVEOUT out, UINT, LPCWAVEFORMATEX f, DWORD_PTR, DWORD_PTR, DWORD flags) {
    if (f->wFormatTag != WAVE_FORMAT_PCM || (f->wBitsPerSample != 8 && f->wBitsPerSample != 16) || f->nChannels < 1 || f->nChannels > 2)
        return WAVERR_BADFORMAT;
    if (flags & WAVE_FORMAT_QUERY) return MMSYSERR_NOERROR;
    auto w = std::make_shared<WaveOut>();
    w->fmt = {static_cast<int>(f->nSamplesPerSec), f->nChannels, f->wBitsPerSample};
    {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        *out = reinterpret_cast<HWAVEOUT>(g_nextWave);
        g_nextWave += 4;
        g_waves[*out] = w;
    }
    audioAdd(w);
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutClose(HWAVEOUT h) {
    auto w = wave(h);
    if (!w) return MMSYSERR_INVALHANDLE;
    audioRemove(w.get());
    std::lock_guard<std::recursive_mutex> l(audioLock());
    g_waves.erase(h);
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutPrepareHeader(HWAVEOUT, LPWAVEHDR h, UINT) { h->dwFlags |= WHDR_PREPARED; return MMSYSERR_NOERROR; }
MMRESULT WINAPI waveOutUnprepareHeader(HWAVEOUT, LPWAVEHDR h, UINT) {
    if (h->dwFlags & WHDR_INQUEUE) return WAVERR_STILLPLAYING;
    h->dwFlags &= ~WHDR_PREPARED;
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutWrite(HWAVEOUT hw, LPWAVEHDR h, UINT) {
    auto w = wave(hw);
    if (!w) return MMSYSERR_INVALHANDLE;
    std::lock_guard<std::recursive_mutex> l(audioLock());
    h->dwFlags = (h->dwFlags & ~WHDR_DONE) | WHDR_INQUEUE;
    w->queue.push_back(h);
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutReset(HWAVEOUT hw) {
    auto w = wave(hw);
    if (!w) return MMSYSERR_INVALHANDLE;
    std::lock_guard<std::recursive_mutex> l(audioLock());
    for (WAVEHDR* h : w->queue) h->dwFlags = (h->dwFlags & ~WHDR_INQUEUE) | WHDR_DONE;
    w->queue.clear();
    w->offset = 0;
    w->played = 0;
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutPause(HWAVEOUT hw) {
    auto w = wave(hw);
    if (!w) return MMSYSERR_INVALHANDLE;
    std::lock_guard<std::recursive_mutex> l(audioLock());
    w->paused = true;
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutRestart(HWAVEOUT hw) {
    auto w = wave(hw);
    if (!w) return MMSYSERR_INVALHANDLE;
    std::lock_guard<std::recursive_mutex> l(audioLock());
    w->paused = false;
    return MMSYSERR_NOERROR;
}
MMRESULT WINAPI waveOutGetPosition(HWAVEOUT hw, LPMMTIME t, UINT) {
    auto w = wave(hw);
    if (!w) return MMSYSERR_INVALHANDLE;
    std::lock_guard<std::recursive_mutex> l(audioLock());
    const uint64_t frames = w->played;
    if (t->wType == TIME_SAMPLES) t->u.sample = static_cast<DWORD>(frames);
    else if (t->wType == TIME_MS) t->u.ms = static_cast<DWORD>(frames * 1000 / static_cast<uint64_t>(w->fmt.rate));
    else { t->wType = TIME_BYTES; t->u.cb = static_cast<DWORD>(frames * static_cast<uint64_t>(w->fmt.channels * w->fmt.bits / 8)); }
    return MMSYSERR_NOERROR;
}
}  // extern "C"
