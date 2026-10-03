// DirectSound on the shared SDL mixer: secondary buffers (8/16-bit PCM, mono/stereo, any
// rate) with volume, pan, frequency, looping, play notifications and 3D positioning
// relative to the primary buffer's listener. Plus device enumeration (one render device)
// through DirectSoundEnumerate and the private property set the game queries.
#include <SDL.h>

#include <cmath>
#include <vector>

#include "comobj.hpp"
#include "w32sdl.hpp"
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include <dsconf.h>

#ifndef E_PROP_ID_UNSUPPORTED
#define E_PROP_ID_UNSUPPORTED HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
#endif

namespace w32 {
void registerHostModule(const char* dll, const char* fn, void* addr);
void registerComClass(REFCLSID clsid, std::function<HRESULT(REFIID, void**)> create);

namespace {

// {6D8B0E2C-6E3A-4F5A-8C7B-46B1A9F0D201}: the one output device.
const GUID kDevice = {0x6D8B0E2C, 0x6E3A, 0x4F5A, {0x8C, 0x7B, 0x46, 0xB1, 0xA9, 0xF0, 0xD2, 0x01}};

float mbToGain(LONG mb) { return mb <= DSBVOLUME_MIN ? 0.0f : std::pow(10.0f, static_cast<float>(mb) / 2000.0f); }

struct Vec { float x = 0, y = 0, z = 0; };
Vec operator-(Vec a, Vec b) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
float dot(Vec a, Vec b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec cross(Vec a, Vec b) { return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x}; }
float len(Vec a) { return std::sqrt(dot(a, a)); }
Vec vec(const D3DVECTOR& v) { return {v.x, v.y, v.z}; }
D3DVECTOR d3d(Vec v) { return {v.x, v.y, v.z}; }

struct Listener {
    DS3DLISTENER p{};
    Listener() {
        p.dwSize = sizeof p;
        p.vOrientFront = {0, 0, 1};
        p.vOrientTop = {0, 1, 0};
        p.flDistanceFactor = DS3D_DEFAULTDISTANCEFACTOR;
        p.flRolloffFactor = DS3D_DEFAULTROLLOFFFACTOR;
        p.flDopplerFactor = DS3D_DEFAULTDOPPLERFACTOR;
    }
};

struct Buffer;
struct Device {
    std::vector<Buffer*> buffers;  // playing or not; under audioLock
    Listener listener;
    WAVEFORMATEX primary{WAVE_FORMAT_PCM, 2, 44100, 44100 * 4, 4, 16, 0};
};

// ---- buffer ---------------------------------------------------------------------------------
struct Buffer : AudioSource {
    Device* dev = nullptr;
    bool primary = false;
    DWORD flags = 0;
    WAVEFORMATEX fmt{};
    std::vector<uint8_t> data;
    double pos = 0;  // play position in frames
    bool playing = false, looping = false;
    LONG volume = 0, pan = 0;
    DWORD freq = 0;  // 0 = the format's rate
    DS3DBUFFER p3{};
    bool is3d = false;
    std::vector<DSBPOSITIONNOTIFY> notify;
    int bpf() const { return fmt.nBlockAlign ? fmt.nBlockAlign : 1; }
    size_t frames() const { return data.size() / static_cast<size_t>(bpf()); }

    void fire(double from, double to, bool wrapped) {
        for (auto& n : notify) {
            if (n.dwOffset == DSBPN_OFFSETSTOP) continue;
            const double f = static_cast<double>(n.dwOffset) / bpf();
            if ((!wrapped && f >= from && f < to) || (wrapped && (f >= from || f < to))) SetEvent(n.hEventNotify);
        }
    }
    void stopped() {
        for (auto& n : notify)
            if (n.dwOffset == DSBPN_OFFSETSTOP) SetEvent(n.hEventNotify);
    }

    void gains(float& gl, float& gr) {
        float g = mbToGain(volume);
        float panL = 1, panR = 1;
        if (pan < 0) panR = mbToGain(pan);
        if (pan > 0) panL = mbToGain(-pan);
        if (is3d && p3.dwMode != DS3DMODE_DISABLE) {
            const DS3DLISTENER& L = dev->listener.p;
            Vec rel = vec(p3.vPosition);
            if (p3.dwMode == DS3DMODE_NORMAL) rel = rel - vec(L.vPosition);
            const float dist = len(rel) * (L.flDistanceFactor > 0 ? L.flDistanceFactor : 1.0f);
            const float minD = p3.flMinDistance > 0 ? p3.flMinDistance : 1.0f;
            const float maxD = p3.flMaxDistance > minD ? p3.flMaxDistance : 1e9f;
            const float d = std::min(std::max(dist, minD), maxD);
            const float rolloff = L.flRolloffFactor;
            g *= minD / (minD + rolloff * (d - minD));
            if (len(rel) > 1e-4f) {
                const Vec right = cross(vec(L.vOrientTop), vec(L.vOrientFront));  // left-handed
                const float side = dot(rel, right) / (len(rel) * std::max(len(right), 1e-6f));
                panL *= std::sqrt(std::clamp(0.5f - 0.5f * side, 0.0f, 1.0f)) * 1.41421356f;
                panR *= std::sqrt(std::clamp(0.5f + 0.5f * side, 0.0f, 1.0f)) * 1.41421356f;
            }
        }
        gl = g * std::min(panL, 1.0f);
        gr = g * std::min(panR, 1.0f);
    }

    void mix(float* out, int count, int rate) override {
        if (primary || !playing || data.empty()) return;
        const double step = static_cast<double>(freq ? freq : fmt.nSamplesPerSec) / rate;
        const PcmFormat pf{static_cast<int>(fmt.nSamplesPerSec), fmt.nChannels, fmt.wBitsPerSample};
        float gl, gr;
        gains(gl, gr);
        const double total = static_cast<double>(frames());
        const double start = pos;
        bool wrapped = false;
        for (int i = 0; i < count; ++i) {
            const size_t f = static_cast<size_t>(pos);
            float l, r;
            pcmFrame(pf, data.data() + f * static_cast<size_t>(bpf()), l, r);
            if (is3d && fmt.nChannels == 1) r = l;
            out[2 * i] += l * gl;
            out[2 * i + 1] += r * gr;
            pos += step;
            if (pos >= total) {
                if (looping) pos -= total, wrapped = true;
                else {
                    pos = 0;
                    playing = false;
                    fire(start, total, false);
                    stopped();
                    return;
                }
            }
        }
        if (!notify.empty()) fire(start, pos, wrapped);
    }
};

// ---- interfaces -----------------------------------------------------------------------------
struct BufferObj;
struct Notify final : ComObject<IDirectSoundNotify> {
    BufferObj* owner;
    explicit Notify(BufferObj* o);
    ~Notify() override;
    bool supports(REFIID iid) const override { return iid == IID_IDirectSoundNotify; }
    HRESULT STDMETHODCALLTYPE SetNotificationPositions(DWORD n, LPCDSBPOSITIONNOTIFY p) override;
};
struct Buffer3D final : ComObject<IDirectSound3DBuffer> {
    BufferObj* owner;
    explicit Buffer3D(BufferObj* o);
    ~Buffer3D() override;
    bool supports(REFIID iid) const override { return iid == IID_IDirectSound3DBuffer; }
    DS3DBUFFER& P();
    HRESULT STDMETHODCALLTYPE GetAllParameters(LPDS3DBUFFER p) override { std::lock_guard<std::recursive_mutex> l(audioLock()); *p = P(); return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetConeAngles(LPDWORD a, LPDWORD b) override { *a = P().dwInsideConeAngle; *b = P().dwOutsideConeAngle; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetConeOrientation(D3DVECTOR* v) override { *v = P().vConeOrientation; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetConeOutsideVolume(LPLONG v) override { *v = P().lConeOutsideVolume; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetMaxDistance(D3DVALUE* v) override { *v = P().flMaxDistance; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetMinDistance(D3DVALUE* v) override { *v = P().flMinDistance; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetMode(LPDWORD v) override { *v = P().dwMode; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetPosition(D3DVECTOR* v) override { *v = P().vPosition; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetVelocity(D3DVECTOR* v) override { *v = P().vVelocity; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetAllParameters(LPCDS3DBUFFER p, DWORD) override { std::lock_guard<std::recursive_mutex> l(audioLock()); P() = *p; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetConeAngles(DWORD a, DWORD b, DWORD) override { P().dwInsideConeAngle = a; P().dwOutsideConeAngle = b; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetConeOrientation(D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD) override { P().vConeOrientation = {x, y, z}; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetConeOutsideVolume(LONG v, DWORD) override { P().lConeOutsideVolume = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetMaxDistance(D3DVALUE v, DWORD) override { P().flMaxDistance = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetMinDistance(D3DVALUE v, DWORD) override { P().flMinDistance = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetMode(DWORD v, DWORD) override { P().dwMode = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        P().vPosition = {x, y, z};
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD) override { P().vVelocity = {x, y, z}; return DS_OK; }
};
struct ListenerObj final : ComObject<IDirectSound3DListener> {
    Device* dev;
    IUnknown* keep;
    ListenerObj(Device* d, IUnknown* k) : dev(d), keep(k) { keep->AddRef(); }
    ~ListenerObj() override { keep->Release(); }
    bool supports(REFIID iid) const override { return iid == IID_IDirectSound3DListener; }
    DS3DLISTENER& P() { return dev->listener.p; }
    HRESULT STDMETHODCALLTYPE GetAllParameters(LPDS3DLISTENER p) override { *p = P(); return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetDistanceFactor(D3DVALUE* v) override { *v = P().flDistanceFactor; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetDopplerFactor(D3DVALUE* v) override { *v = P().flDopplerFactor; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetOrientation(D3DVECTOR* f, D3DVECTOR* t) override { *f = P().vOrientFront; *t = P().vOrientTop; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetPosition(D3DVECTOR* v) override { *v = P().vPosition; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetRolloffFactor(D3DVALUE* v) override { *v = P().flRolloffFactor; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetVelocity(D3DVECTOR* v) override { *v = P().vVelocity; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetAllParameters(LPCDS3DLISTENER p, DWORD) override { std::lock_guard<std::recursive_mutex> l(audioLock()); P() = *p; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetDistanceFactor(D3DVALUE v, DWORD) override { P().flDistanceFactor = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetDopplerFactor(D3DVALUE v, DWORD) override { P().flDopplerFactor = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetOrientation(D3DVALUE fx, D3DVALUE fy, D3DVALUE fz, D3DVALUE tx, D3DVALUE ty, D3DVALUE tz, DWORD) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        P().vOrientFront = {fx, fy, fz};
        P().vOrientTop = {tx, ty, tz};
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetPosition(D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        P().vPosition = {x, y, z};
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetRolloffFactor(D3DVALUE v, DWORD) override { P().flRolloffFactor = v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetVelocity(D3DVALUE x, D3DVALUE y, D3DVALUE z, DWORD) override { P().vVelocity = {x, y, z}; return DS_OK; }
    HRESULT STDMETHODCALLTYPE CommitDeferredSettings() override { return DS_OK; }
};

struct BufferObj final : ComObject<IDirectSoundBuffer8> {
    std::shared_ptr<Buffer> b;
    std::vector<uint8_t> lockScratch;  // primary buffer locks
    bool supports(REFIID iid) const override { return iidIn(iid, {&IID_IDirectSoundBuffer, &IID_IDirectSoundBuffer8}); }
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** out) override {
        if (iid == IID_IDirectSoundNotify && !b->primary) { *out = static_cast<IDirectSoundNotify*>(new Notify(this)); return S_OK; }
        if (iid == IID_IDirectSound3DBuffer && b->is3d) { *out = static_cast<IDirectSound3DBuffer*>(new Buffer3D(this)); return S_OK; }
        if (iid == IID_IDirectSound3DListener && b->primary) { *out = static_cast<IDirectSound3DListener*>(new ListenerObj(b->dev, this)); return S_OK; }
        return ComObject::QueryInterface(iid, out);
    }
    ~BufferObj() override {
        audioRemove(b.get());
        std::lock_guard<std::recursive_mutex> l(audioLock());
        auto& v = b->dev->buffers;
        v.erase(std::remove(v.begin(), v.end(), b.get()), v.end());
    }
    HRESULT STDMETHODCALLTYPE GetCaps(LPDSBCAPS c) override {
        c->dwFlags = b->flags;
        c->dwBufferBytes = static_cast<DWORD>(b->data.size());
        c->dwUnlockTransferRate = 0;
        c->dwPlayCpuOverhead = 0;
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE GetCurrentPosition(LPDWORD play, LPDWORD write) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        if (b->primary) { if (play) *play = 0; if (write) *write = 0; return DS_OK; }
        const DWORD p = static_cast<DWORD>(b->pos) * static_cast<DWORD>(b->bpf());
        if (play) *play = p;
        if (write) {
            // A write cursor a little ahead of the play cursor (the mixer's latency).
            const DWORD ahead = static_cast<DWORD>(b->fmt.nAvgBytesPerSec / 50) / static_cast<DWORD>(b->bpf()) * static_cast<DWORD>(b->bpf());
            *write = b->data.empty() ? 0 : static_cast<DWORD>((p + (b->playing ? ahead : 0)) % b->data.size());
        }
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE GetFormat(LPWAVEFORMATEX f, DWORD size, LPDWORD written) override {
        const WAVEFORMATEX& src = b->primary ? b->dev->primary : b->fmt;
        if (written) *written = sizeof(WAVEFORMATEX);
        if (f) std::memcpy(f, &src, std::min<DWORD>(size, sizeof(WAVEFORMATEX)));
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE GetVolume(LPLONG v) override { *v = b->volume; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetPan(LPLONG v) override { *v = b->pan; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetFrequency(LPDWORD v) override { *v = b->freq ? b->freq : b->fmt.nSamplesPerSec; return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetStatus(LPDWORD s) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        *s = b->playing ? (DSBSTATUS_PLAYING | (b->looping ? DSBSTATUS_LOOPING : 0)) : 0;
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE Initialize(LPDIRECTSOUND, LPCDSBUFFERDESC) override { return DSERR_ALREADYINITIALIZED; }
    HRESULT STDMETHODCALLTYPE Lock(DWORD off, DWORD bytes, LPVOID* p1, LPDWORD n1, LPVOID* p2, LPDWORD n2, DWORD flags) override {
        if (b->primary) {
            lockScratch.assign(bytes, 0);
            *p1 = lockScratch.data();
            *n1 = bytes;
            if (p2) *p2 = nullptr;
            if (n2) *n2 = 0;
            return DS_OK;
        }
        const DWORD size = static_cast<DWORD>(b->data.size());
        if (flags & DSBLOCK_FROMWRITECURSOR) GetCurrentPosition(nullptr, &off);
        if (flags & DSBLOCK_ENTIREBUFFER) bytes = size;
        if (!size || off >= size || bytes > size) return DSERR_INVALIDPARAM;
        *p1 = b->data.data() + off;
        if (off + bytes <= size) {
            *n1 = bytes;
            if (p2) *p2 = nullptr;
            if (n2) *n2 = 0;
        } else {
            *n1 = size - off;
            if (p2) *p2 = b->data.data();
            if (n2) *n2 = bytes - (size - off);
        }
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE Play(DWORD, DWORD, DWORD flags) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        b->playing = true;
        b->looping = flags & DSBPLAY_LOOPING;
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetCurrentPosition(DWORD p) override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        b->pos = static_cast<double>(p / static_cast<DWORD>(b->bpf()));
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetFormat(LPCWAVEFORMATEX f) override {
        if (!b->primary) return DSERR_INVALIDCALL;
        b->dev->primary = *f;
        b->dev->primary.cbSize = 0;
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetVolume(LONG v) override { std::lock_guard<std::recursive_mutex> l(audioLock()); b->volume = std::clamp<LONG>(v, DSBVOLUME_MIN, DSBVOLUME_MAX); return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetPan(LONG v) override { std::lock_guard<std::recursive_mutex> l(audioLock()); b->pan = std::clamp<LONG>(v, DSBPAN_LEFT, DSBPAN_RIGHT); return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetFrequency(DWORD v) override { std::lock_guard<std::recursive_mutex> l(audioLock()); b->freq = v == DSBFREQUENCY_ORIGINAL ? 0 : v; return DS_OK; }
    HRESULT STDMETHODCALLTYPE Stop() override {
        std::lock_guard<std::recursive_mutex> l(audioLock());
        if (b->playing) { b->playing = false; b->stopped(); }
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE Unlock(LPVOID, DWORD, LPVOID, DWORD) override { return DS_OK; }
    HRESULT STDMETHODCALLTYPE Restore() override { return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetFX(DWORD n, LPDSEFFECTDESC, LPDWORD res) override {
        for (DWORD i = 0; res && i < n; ++i) res[i] = DSFXR_FAILED;
        return n ? DSERR_FXUNAVAILABLE : DS_OK;
    }
    HRESULT STDMETHODCALLTYPE AcquireResources(DWORD, DWORD n, LPDWORD res) override {
        for (DWORD i = 0; res && i < n; ++i) res[i] = DSFXR_FAILED;
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE GetObjectInPath(REFGUID, DWORD, REFGUID, LPVOID*) override { return DSERR_OBJECTNOTFOUND; }
};

Notify::Notify(BufferObj* o) : owner(o) { owner->AddRef(); }
Notify::~Notify() { owner->Release(); }
HRESULT STDMETHODCALLTYPE Notify::SetNotificationPositions(DWORD n, LPCDSBPOSITIONNOTIFY p) {
    std::lock_guard<std::recursive_mutex> l(audioLock());
    owner->b->notify.assign(p, p + n);
    return DS_OK;
}
Buffer3D::Buffer3D(BufferObj* o) : owner(o) { owner->AddRef(); }
Buffer3D::~Buffer3D() { owner->Release(); }
DS3DBUFFER& Buffer3D::P() { return owner->b->p3; }

struct DirectSound final : ComObject<IDirectSound8> {
    Device dev;
    bool supports(REFIID iid) const override { return iidIn(iid, {&IID_IDirectSound, &IID_IDirectSound8}); }
    HRESULT STDMETHODCALLTYPE CreateSoundBuffer(LPCDSBUFFERDESC d, LPDIRECTSOUNDBUFFER* out, LPUNKNOWN) override {
        auto b = std::make_shared<Buffer>();
        b->dev = &dev;
        b->flags = d->dwFlags;
        b->primary = d->dwFlags & DSBCAPS_PRIMARYBUFFER;
        b->is3d = d->dwFlags & DSBCAPS_CTRL3D;
        b->p3.dwSize = sizeof b->p3;
        b->p3.flMinDistance = DS3D_DEFAULTMINDISTANCE;
        b->p3.flMaxDistance = DS3D_DEFAULTMAXDISTANCE;
        b->p3.dwInsideConeAngle = b->p3.dwOutsideConeAngle = DS3D_DEFAULTCONEANGLE;
        b->p3.vConeOrientation = {0, 0, 1};
        b->p3.dwMode = DS3DMODE_NORMAL;
        if (!b->primary) {
            if (!d->lpwfxFormat || d->dwBufferBytes < DSBSIZE_MIN || d->dwBufferBytes > DSBSIZE_MAX) return DSERR_INVALIDPARAM;
            b->fmt = *d->lpwfxFormat;
            b->fmt.cbSize = 0;
            if ((b->fmt.wFormatTag != WAVE_FORMAT_PCM && b->fmt.wFormatTag != WAVE_FORMAT_EXTENSIBLE) || (b->fmt.wBitsPerSample != 8 && b->fmt.wBitsPerSample != 16) ||
                b->fmt.nChannels < 1 || b->fmt.nChannels > 2)
                return DSERR_BADFORMAT;
            b->data.assign(d->dwBufferBytes, b->fmt.wBitsPerSample == 8 ? 0x80 : 0);
        }
        auto* o = new BufferObj;
        o->b = b;
        {
            std::lock_guard<std::recursive_mutex> l(audioLock());
            dev.buffers.push_back(b.get());
        }
        if (!b->primary) audioAdd(b);
        *out = reinterpret_cast<LPDIRECTSOUNDBUFFER>(static_cast<IDirectSoundBuffer8*>(o));  // same vtable prefix
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE GetCaps(LPDSCAPS c) override {
        const DWORD size = c->dwSize;
        std::memset(c, 0, size);
        c->dwSize = size;
        c->dwFlags = DSCAPS_PRIMARYMONO | DSCAPS_PRIMARYSTEREO | DSCAPS_PRIMARY8BIT | DSCAPS_PRIMARY16BIT | DSCAPS_SECONDARYMONO |
                     DSCAPS_SECONDARYSTEREO | DSCAPS_SECONDARY8BIT | DSCAPS_SECONDARY16BIT | DSCAPS_CONTINUOUSRATE;
        c->dwMinSecondarySampleRate = DSBFREQUENCY_MIN;
        c->dwMaxSecondarySampleRate = DSBFREQUENCY_MAX;
        c->dwPrimaryBuffers = 1;
        c->dwMaxHwMixingAllBuffers = c->dwMaxHwMixingStaticBuffers = c->dwMaxHwMixingStreamingBuffers = 1;
        c->dwFreeHwMixingAllBuffers = c->dwFreeHwMixingStaticBuffers = c->dwFreeHwMixingStreamingBuffers = 1;
        c->dwMaxHw3DAllBuffers = c->dwMaxHw3DStaticBuffers = c->dwMaxHw3DStreamingBuffers = 0;
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER src, LPDIRECTSOUNDBUFFER* out) override {
        auto* s = static_cast<BufferObj*>(reinterpret_cast<IDirectSoundBuffer8*>(src));
        if (s->b->primary) return DSERR_INVALIDCALL;
        auto b = std::make_shared<Buffer>(*s->b);
        b->pos = 0;
        b->playing = false;
        b->notify.clear();
        auto* o = new BufferObj;
        o->b = b;
        {
            std::lock_guard<std::recursive_mutex> l(audioLock());
            dev.buffers.push_back(b.get());
        }
        audioAdd(b);
        *out = reinterpret_cast<LPDIRECTSOUNDBUFFER>(static_cast<IDirectSoundBuffer8*>(o));
        return DS_OK;
    }
    HRESULT STDMETHODCALLTYPE SetCooperativeLevel(HWND, DWORD) override { return DS_OK; }
    HRESULT STDMETHODCALLTYPE Compact() override { return DS_OK; }
    HRESULT STDMETHODCALLTYPE GetSpeakerConfig(LPDWORD c) override { *c = DSSPEAKER_COMBINED(DSSPEAKER_STEREO, DSSPEAKER_GEOMETRY_WIDE); return DS_OK; }
    HRESULT STDMETHODCALLTYPE SetSpeakerConfig(DWORD) override { return DS_OK; }
    HRESULT STDMETHODCALLTYPE Initialize(LPCGUID) override { audioRate(); return DS_OK; }
    HRESULT STDMETHODCALLTYPE VerifyCertification(LPDWORD c) override { *c = DS_UNCERTIFIED; return DS_OK; }
};

// ---- private property set: device enumeration ------------------------------------------------
struct PropertySet final : ComObject<IKsPropertySet> {
    bool supports(REFIID iid) const override { return iid == IID_IKsPropertySet; }
    HRESULT STDMETHODCALLTYPE Get(REFGUID set, ULONG id, LPVOID, ULONG, LPVOID data, ULONG, PULONG got) override {
        if (set != DSPROPSETID_DirectSoundDevice) return E_PROP_ID_UNSUPPORTED;
        if (id == DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W) {
            auto* e = static_cast<PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA>(data);
            DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA d{};
            d.Type = DIRECTSOUNDDEVICE_TYPE_WDM;
            d.DataFlow = DIRECTSOUNDDEVICE_DATAFLOW_RENDER;
            d.DeviceId = kDevice;
            static wchar_t desc[] = L"SDL audio output", module[] = L"sdl", iface[] = L"\\\\?\\sdl";
            d.Description = desc;
            d.Module = module;
            d.Interface = iface;
            d.WaveDeviceId = 0;
            e->Callback(&d, e->Context);
            if (got) *got = sizeof *e;
            return S_OK;
        }
        return E_PROP_ID_UNSUPPORTED;
    }
    HRESULT STDMETHODCALLTYPE Set(REFGUID, ULONG, LPVOID, ULONG, LPVOID, ULONG) override { return E_PROP_ID_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE QuerySupport(REFGUID set, ULONG id, PULONG s) override {
        if (set == DSPROPSETID_DirectSoundDevice && id == DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W) { *s = KSPROPERTY_SUPPORT_GET; return S_OK; }
        return E_PROP_ID_UNSUPPORTED;
    }
};
struct PrivateFactory final : ComObject<IClassFactory> {
    bool supports(REFIID iid) const override { return iid == IID_IClassFactory; }
    HRESULT STDMETHODCALLTYPE CreateInstance(IUnknown*, REFIID iid, void** out) override {
        auto* p = new PropertySet;
        const HRESULT hr = p->QueryInterface(iid, out);
        p->Release();
        return hr;
    }
    HRESULT STDMETHODCALLTYPE LockServer(BOOL) override { return S_OK; }
};

HRESULT WINAPI dllGetClassObject(REFCLSID clsid, REFIID iid, void** out) {
    if (clsid != CLSID_DirectSoundPrivate) { *out = nullptr; return CLASS_E_CLASSNOTAVAILABLE; }
    auto* f = new PrivateFactory;
    const HRESULT hr = f->QueryInterface(iid, out);
    f->Release();
    return hr;
}

HRESULT createDirectSound(REFIID iid, void** out) {
    auto* ds = new DirectSound;
    const HRESULT hr = ds->QueryInterface(iid, out);
    ds->Release();
    return hr;
}

struct Registrar {
    Registrar() {
        registerHostModule("dsound.dll", "DllGetClassObject", reinterpret_cast<void*>(&dllGetClassObject));
        registerComClass(CLSID_DirectSound, createDirectSound);
        registerComClass(CLSID_DirectSound8, createDirectSound);
    }
} g_registrar;

}  // namespace
}  // namespace w32

using namespace w32;

extern "C" {
HRESULT WINAPI DirectSoundCreate8(LPCGUID, LPDIRECTSOUND8* out, LPUNKNOWN) { return createDirectSound(IID_IDirectSound8, reinterpret_cast<void**>(out)); }
HRESULT WINAPI DirectSoundCreate(LPCGUID, LPDIRECTSOUND* out, LPUNKNOWN) { return createDirectSound(IID_IDirectSound, reinterpret_cast<void**>(out)); }
HRESULT WINAPI DirectSoundEnumerateW(LPDSENUMCALLBACKW cb, LPVOID ctx) {
    static wchar_t primary[] = L"Primary Sound Driver", none[] = L"", desc[] = L"SDL audio output", module[] = L"sdl";
    if (!cb(nullptr, primary, none, ctx)) return DS_OK;
    GUID g = kDevice;
    cb(&g, desc, module, ctx);
    return DS_OK;
}
HRESULT WINAPI DirectSoundEnumerateA(LPDSENUMCALLBACKA cb, LPVOID ctx) {
    static char primary[] = "Primary Sound Driver", none[] = "", desc[] = "SDL audio output", module[] = "sdl";
    if (!cb(nullptr, primary, none, ctx)) return DS_OK;
    GUID g = kDevice;
    cb(&g, desc, module, ctx);
    return DS_OK;
}
HRESULT WINAPI GetDeviceID(LPCGUID src, LPGUID dst) {
    if (!dst) return DSERR_INVALIDPARAM;
    *dst = src && *src != DSDEVID_DefaultPlayback && *src != DSDEVID_DefaultVoicePlayback && *src != GUID_NULL ? *src : kDevice;
    return DS_OK;
}
}
