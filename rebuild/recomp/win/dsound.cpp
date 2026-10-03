// DirectSound.
// - Fable.exe creates CLSID_DirectSound (IDirectSound) through CoCreateInstance: the real
//   64-bit DirectSound behind the generated proxies, with hand-written CreateSoundBuffer
//   (DSBUFFERDESC holds a pointer), Lock/Unlock (host pointers go through low guest
//   staging memory) and SetNotificationPositions (DSBPOSITIONNOTIFY holds a HANDLE).
// - ConfigDetect.dll needs dsound.dll to be present and resolves GetDeviceID.
// - Fable.exe calls dsound!DllGetClassObject(CLSID_DirectSoundPrivate) and uses the
//   IKsPropertySet it yields to enumerate devices (DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A),
//   without checking any result. Both objects are host-implemented guest objects; the device
//   list comes from the real 64-bit dsound.
#include "com.hpp"

#include <windows.h>
#include <mmreg.h>
#include <dsound.h>
#include <dsconf.h>

#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

using namespace host;

namespace {

#ifndef E_PROP_ID_UNSUPPORTED
#define E_PROP_ID_UNSUPPORTED HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
#endif

static ::host::AutoReg reg_GetDeviceID("dsound.dll", "GetDeviceID", &::host::stdThunk<&::GetDeviceID>);

struct DsDevice {
    DIRECTSOUNDDEVICE_TYPE type;
    DIRECTSOUNDDEVICE_DATAFLOW flow;
    GUID id;
    std::string desc, module, iface;
    ULONG waveId;
};

std::string acp(const wchar_t* w) {
    if (!w) return {};
    const int n = WideCharToMultiByte(CP_ACP, 0, w, -1, nullptr, 0, nullptr, nullptr);
    std::string s(n > 0 ? n - 1 : 0, '\0');
    if (n > 1) WideCharToMultiByte(CP_ACP, 0, w, -1, s.data(), n, nullptr, nullptr);
    return s;
}

// The real device list, through the host dsound's private property set.
std::vector<DsDevice> enumerateHostDevices() {
    std::vector<DsDevice> out;
    HMODULE ds = LoadLibraryW(L"dsound.dll");
    auto getClass = ds ? reinterpret_cast<HRESULT(WINAPI*)(REFCLSID, REFIID, void**)>(GetProcAddress(ds, "DllGetClassObject")) : nullptr;
    IClassFactory* cf = nullptr;
    IKsPropertySet* ps = nullptr;
    if (getClass && SUCCEEDED(getClass(CLSID_DirectSoundPrivate, IID_IClassFactory, reinterpret_cast<void**>(&cf))) &&
        SUCCEEDED(cf->CreateInstance(nullptr, IID_IKsPropertySet, reinterpret_cast<void**>(&ps)))) {
        DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA data{};
        data.Callback = [](PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA d, LPVOID p) -> BOOL {
            static_cast<std::vector<DsDevice>*>(p)->push_back(
                {d->Type, d->DataFlow, d->DeviceId, acp(d->Description), acp(d->Module), acp(d->Interface), d->WaveDeviceId});
            return TRUE;
        };
        data.Context = &out;
        ULONG got = 0;
        const HRESULT hr = ps->Get(DSPROPSETID_DirectSoundDevice, DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W, nullptr, 0, &data, sizeof data, &got);
        HLOG(1, "host DirectSoundPrivate enumerate -> 0x%08lX, %zu devices", static_cast<unsigned long>(hr), out.size());
    }
    if (ps) ps->Release();
    if (cf) cf->Release();
    // Wine leaves DataFlow unset: take it from the public render/capture lists.
    std::vector<GUID> render;
    DirectSoundEnumerateW(
        [](GUID* g, LPCWSTR, LPCWSTR, LPVOID p) -> BOOL {
            if (g) static_cast<std::vector<GUID>*>(p)->push_back(*g);
            return TRUE;
        },
        &render);
    for (DsDevice& d : out) {
        if (d.flow == DIRECTSOUNDDEVICE_DATAFLOW_RENDER || d.flow == DIRECTSOUNDDEVICE_DATAFLOW_CAPTURE) continue;
        bool isRender = false;
        for (const GUID& g : render) isRender |= IsEqualGUID(g, d.id) != 0;
        d.flow = isRender ? DIRECTSOUNDDEVICE_DATAFLOW_RENDER : DIRECTSOUNDDEVICE_DATAFLOW_CAPTURE;
    }
    if (out.empty()) {  // fall back to the public enumerators (no interface path)
        DirectSoundEnumerateA(
            [](GUID* g, LPCSTR desc, LPCSTR mod, LPVOID p) -> BOOL {
                if (g) static_cast<std::vector<DsDevice>*>(p)->push_back(
                    {DIRECTSOUNDDEVICE_TYPE_WDM, DIRECTSOUNDDEVICE_DATAFLOW_RENDER, *g, desc ? desc : "", mod ? mod : "", "", 0});
                return TRUE;
            },
            &out);
    }
    return out;
}

// x86 DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA: Type, DataFlow, DeviceId,
// Description, Module, Interface (32-bit pointers), WaveDeviceId — 40 bytes.
uint32_t callGuestEnumA(uint32_t cb, uint32_t ctx) {
    const std::vector<DsDevice> devs = enumerateHostDevices();
    const uint32_t g = gcalloc(1, 40);
    for (const DsDevice& d : devs) {
        HLOG(1, "DirectSoundPrivate device: %s [%s] flow %d", d.desc.c_str(), d.iface.c_str(), d.flow);
        const uint32_t desc = gstrdup(d.desc.c_str()), mod = gstrdup(d.module.c_str()), iface = gstrdup(d.iface.c_str());
        wr32(g + 0, d.type);
        wr32(g + 4, d.flow);
        std::memcpy(gp(g + 8), &d.id, 16);
        wr32(g + 24, desc);
        wr32(g + 28, mod);
        wr32(g + 32, iface);
        wr32(g + 36, d.waveId);
        const uint32_t more = guestCall(cb, {g, ctx});
        gfree(iface);
        gfree(mod);
        gfree(desc);
        if (!more) break;
    }
    gfree(g);
    return static_cast<uint32_t>(devs.size());
}

uint32_t g_factory = 0, g_propSet = 0;

void objAddRef(Ctx* c) { retStd(c, 1, 1); }
void objRelease(Ctx* c) { retStd(c, 1, 1); }  // static singletons

void returnInterface(Ctx* c, const GUID& iid, uint32_t out, int nargs) {
    uint32_t obj = 0;
    if (IsEqualGUID(iid, IID_IClassFactory)) obj = g_factory;
    else if (IsEqualGUID(iid, IID_IKsPropertySet)) obj = g_propSet;
    else if (IsEqualGUID(iid, IID_IUnknown)) obj = arg(c, 0) ? arg(c, 0) : g_factory;
    if (out) wr32(out, obj);
    retStd(c, static_cast<uint32_t>(obj ? S_OK : E_NOINTERFACE), nargs);
}

void objQueryInterface(Ctx* c) { returnInterface(c, *gp<GUID>(arg(c, 1)), arg(c, 2), 3); }
void cfCreateInstance(Ctx* c) {
    if (arg(c, 1)) { if (arg(c, 3)) wr32(arg(c, 3), 0); retStd(c, static_cast<uint32_t>(CLASS_E_NOAGGREGATION), 4); return; }
    returnInterface(c, *gp<GUID>(arg(c, 2)), arg(c, 3), 4);
}
void cfLockServer(Ctx* c) { retStd(c, S_OK, 2); }

// Get(this, set, id, instData, instLen, propData, dataLen, bytesReturned)
void psGet(Ctx* c) {
    const GUID& set = *gp<GUID>(arg(c, 1));
    const uint32_t id = arg(c, 2), data = arg(c, 5), len = arg(c, 6), ret = arg(c, 7);
    if (IsEqualGUID(set, DSPROPSETID_DirectSoundDevice) && id == DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A && data && len >= 8) {
        callGuestEnumA(rd32(data), rd32(data + 4));
        if (ret) wr32(ret, 8);
        retStd(c, S_OK, 8);
        return;
    }
    log("IKsPropertySet::Get({%08lX-...}, %u) not supported", set.Data1, id);
    if (ret) wr32(ret, 0);
    retStd(c, static_cast<uint32_t>(E_PROP_ID_UNSUPPORTED), 8);
}
void psSet(Ctx* c) { retStd(c, static_cast<uint32_t>(E_PROP_ID_UNSUPPORTED), 6); }
// QuerySupport(this, set, id, pTypeSupport)
void psQuerySupport(Ctx* c) {
    const bool ok = IsEqualGUID(*gp<GUID>(arg(c, 1)), DSPROPSETID_DirectSoundDevice) && arg(c, 2) == DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A;
    if (arg(c, 3)) wr32(arg(c, 3), ok ? KSPROPERTY_SUPPORT_GET : 0);
    retStd(c, static_cast<uint32_t>(ok ? S_OK : E_PROP_ID_UNSUPPORTED), 4);
}

void makeObjects() {
    if (g_factory) return;
    static com::Class cf = com::makeClass("DirectSoundPrivate IClassFactory", {
        {"IClassFactory::QueryInterface", objQueryInterface},
        {"IClassFactory::AddRef", objAddRef},
        {"IClassFactory::Release", objRelease},
        {"IClassFactory::CreateInstance", cfCreateInstance},
        {"IClassFactory::LockServer", cfLockServer},
    });
    static com::Class ps = com::makeClass("DirectSoundPrivate IKsPropertySet", {
        {"IKsPropertySet::QueryInterface", objQueryInterface},
        {"IKsPropertySet::AddRef", objAddRef},
        {"IKsPropertySet::Release", objRelease},
        {"IKsPropertySet::Get", psGet},
        {"IKsPropertySet::Set", psSet},
        {"IKsPropertySet::QuerySupport", psQuerySupport},
    });
    g_factory = gcalloc(1, 4);
    wr32(g_factory, cf.vtbl);
    g_propSet = gcalloc(1, 4);
    wr32(g_propSet, ps.vtbl);
}

IMPORT("dsound.dll", DllGetClassObject) {
    const GUID& clsid = *gp<GUID>(arg(c, 0));
    const uint32_t out = arg(c, 2);
    if (!IsEqualGUID(clsid, CLSID_DirectSoundPrivate)) {
        log("dsound!DllGetClassObject({%08lX-...}) -> class not available", clsid.Data1);
        if (out) wr32(out, 0);
        retStd(c, static_cast<uint32_t>(CLASS_E_CLASSNOTAVAILABLE), 3);
        return;
    }
    makeObjects();
    returnInterface(c, *gp<GUID>(arg(c, 1)), out, 3);
}

}  // namespace

// ---------------------------------------------------------------------------
// Generated-proxy overrides
// ---------------------------------------------------------------------------
namespace {

// x86 DSBUFFERDESC: dwSize, dwFlags, dwBufferBytes, dwReserved, lpwfxFormat (32-bit),
// guid3DAlgorithm (DX7+ only: dwSize 36; the DX3 DSBUFFERDESC1 is 20 bytes).
template <class DS> void createSoundBuffer(Ctx* c) {
    DS* self = com::unwrap<DS>(arg(c, 0));
    const uint32_t g = arg(c, 1), out = arg(c, 2);
    DSBUFFERDESC d{};
    d.dwSize = sizeof d;
    d.dwFlags = rd32(g + 4);
    d.dwBufferBytes = rd32(g + 8);
    d.dwReserved = rd32(g + 12);
    d.lpwfxFormat = rd32(g + 16) ? gp<WAVEFORMATEX>(rd32(g + 16)) : nullptr;
    if (rd32(g) >= 36) std::memcpy(&d.guid3DAlgorithm, gp(g + 20), 16);
    IDirectSoundBuffer* b = nullptr;
    const HRESULT hr = self->CreateSoundBuffer(&d, &b, nullptr);
    if (out) wr32(out, SUCCEEDED(hr) ? com::wrap(b) : 0);
    HLOG(2, "IDirectSound::CreateSoundBuffer(flags 0x%lX, %lu bytes, %u Hz) -> 0x%08lX", d.dwFlags, d.dwBufferBytes,
         d.lpwfxFormat ? static_cast<unsigned>(d.lpwfxFormat->nSamplesPerSec) : 0u, static_cast<unsigned long>(hr));
    retStd(c, static_cast<uint32_t>(hr), 4);
}

// Lock staging: one low guest block per buffer, reused across locks.
struct Staging {
    uint32_t mem = 0, cap = 0;
    void* host1 = nullptr;
    void* host2 = nullptr;
    DWORD n1 = 0, n2 = 0;
};
std::mutex g_stageLock;
std::unordered_map<IUnknown*, Staging> g_stage;

void freeStaging(IUnknown* p) {
    std::lock_guard<std::mutex> l(g_stageLock);
    auto it = g_stage.find(p);
    if (it == g_stage.end()) return;
    if (it->second.mem) gfree(it->second.mem);
    g_stage.erase(it);
}

// Lock(this, offset, bytes, ppv1, pn1, ppv2, pn2, flags)
template <class B> void lockBuffer(Ctx* c) {
    B* self = com::unwrap<B>(arg(c, 0));
    void* p1 = nullptr;
    void* p2 = nullptr;
    DWORD n1 = 0, n2 = 0;
    const HRESULT hr = self->Lock(arg(c, 1), arg(c, 2), &p1, &n1, arg(c, 5) ? &p2 : nullptr, arg(c, 5) ? &n2 : nullptr, arg(c, 7));
    uint32_t g1 = 0, g2 = 0;
    if (SUCCEEDED(hr)) {
        com::onRelease(reinterpret_cast<IUnknown*>(self), freeStaging);
        std::lock_guard<std::mutex> l(g_stageLock);
        Staging& s = g_stage[reinterpret_cast<IUnknown*>(self)];
        if (s.cap < n1 + n2) {
            if (s.mem) gfree(s.mem);
            s.cap = (n1 + n2 + 0xFFF) & ~0xFFFu;
            s.mem = gmalloc(s.cap);
        }
        s.host1 = p1, s.host2 = p2, s.n1 = n1, s.n2 = n2;
        g1 = s.mem;
        g2 = p2 ? s.mem + n1 : 0;
        if (n1) std::memcpy(gp(g1), p1, n1);  // the guest may read back (and Unlock may write less)
        if (n2) std::memcpy(gp(g2), p2, n2);
    }
    if (arg(c, 3)) wr32(arg(c, 3), g1);
    if (arg(c, 4)) wr32(arg(c, 4), n1);
    if (arg(c, 5)) wr32(arg(c, 5), g2);
    if (arg(c, 6)) wr32(arg(c, 6), n2);
    retStd(c, static_cast<uint32_t>(hr), 8);
}

// Unlock(this, pv1, n1, pv2, n2)
template <class B> void unlockBuffer(Ctx* c) {
    B* self = com::unwrap<B>(arg(c, 0));
    void* h1 = nullptr;
    void* h2 = nullptr;
    DWORD w1 = arg(c, 2), w2 = arg(c, 4);
    {
        std::lock_guard<std::mutex> l(g_stageLock);
        auto it = g_stage.find(reinterpret_cast<IUnknown*>(self));
        if (it != g_stage.end()) {
            Staging& s = it->second;
            h1 = s.host1, h2 = s.host2;
            if (w1 > s.n1) w1 = s.n1;
            if (w2 > s.n2) w2 = s.n2;
            if (h1 && w1 && arg(c, 1)) std::memcpy(h1, gp(arg(c, 1)), w1);
            if (h2 && w2 && arg(c, 3)) std::memcpy(h2, gp(arg(c, 3)), w2);
            s.host1 = s.host2 = nullptr;
        }
    }
    const HRESULT hr = self->Unlock(h1, w1, h2, w2);
    retStd(c, static_cast<uint32_t>(hr), 5);
}

}  // namespace

void ovr_IDirectSound_CreateSoundBuffer(Ctx* c) { createSoundBuffer<IDirectSound>(c); }
void ovr_IDirectSound8_CreateSoundBuffer(Ctx* c) { createSoundBuffer<IDirectSound8>(c); }
void ovr_IDirectSoundBuffer_Lock(Ctx* c) { lockBuffer<IDirectSoundBuffer>(c); }
void ovr_IDirectSoundBuffer_Unlock(Ctx* c) { unlockBuffer<IDirectSoundBuffer>(c); }
void ovr_IDirectSoundBuffer8_Lock(Ctx* c) { lockBuffer<IDirectSoundBuffer8>(c); }
void ovr_IDirectSoundBuffer8_Unlock(Ctx* c) { unlockBuffer<IDirectSoundBuffer8>(c); }

// SetNotificationPositions(this, count, positions): x86 DSBPOSITIONNOTIFY is {offset, HANDLE} (8 bytes).
void ovr_IDirectSoundNotify_SetNotificationPositions(Ctx* c) {
    IDirectSoundNotify* self = com::unwrap<IDirectSoundNotify>(arg(c, 0));
    const uint32_t n = arg(c, 1), g = arg(c, 2);
    std::vector<DSBPOSITIONNOTIFY> v(n);
    for (uint32_t i = 0; i < n; ++i) {
        v[i].dwOffset = rd32(g + 8 * i);
        v[i].hEventNotify = hh(rd32(g + 8 * i + 4));
    }
    retStd(c, static_cast<uint32_t>(self->SetNotificationPositions(n, n ? v.data() : nullptr)), 3);
}

