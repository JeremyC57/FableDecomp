// DirectSound.
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

#include <string>
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
