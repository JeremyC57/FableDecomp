// DirectDraw 7: only what ConfigDetect.dll needs to size video memory
// (enumerate devices, create IDirectDraw7, GetAvailableVidMem). The real
// 64-bit ddraw sits behind a guest proxy like Direct3D 9.
#include "com.hpp"

#include <ddraw.h>

#include <string>
#include <vector>

using namespace host;

namespace {

struct DdDevice {
    bool hasGuid;
    GUID guid;
    std::string desc, name;
    HMONITOR monitor;
};

IMPORT("ddraw.dll", DirectDrawEnumerateExA) {
    const uint32_t cb = arg(c, 0), ctx = arg(c, 1), flags = arg(c, 2);
    std::vector<DdDevice> found;
    const HRESULT hr = DirectDrawEnumerateExA(
        [](GUID* g, LPSTR desc, LPSTR name, LPVOID p, HMONITOR m) -> BOOL {
            static_cast<std::vector<DdDevice>*>(p)->push_back({g != nullptr, g ? *g : GUID{}, desc ? desc : "", name ? name : "", m});
            return TRUE;
        },
        &found, flags);
    for (const DdDevice& d : found) {
        const uint32_t g = d.hasGuid ? gmemdup(&d.guid, sizeof d.guid) : 0;
        const uint32_t desc = gstrdup(d.desc.c_str()), name = gstrdup(d.name.c_str());
        HLOG(1, "DirectDrawEnumerateExA: %s (%s)", d.desc.c_str(), d.name.c_str());
        const uint32_t more = guestCall(cb, {g, desc, name, ctx, gh(d.monitor)});
        gfree(name);
        gfree(desc);
        if (g) gfree(g);
        if (!more) break;
    }
    retStd(c, static_cast<uint32_t>(hr), 3);
}

IMPORT("ddraw.dll", DirectDrawCreateEx) {
    // lpGUID may also be DDCREATE_HARDWAREONLY (1) or DDCREATE_EMULATIONONLY (2).
    const uint32_t g = arg(c, 0);
    GUID* guid = g > 2 ? gp<GUID>(g) : reinterpret_cast<GUID*>(static_cast<uintptr_t>(g));
    const GUID& iid = *gp<GUID>(arg(c, 2));
    void* p = nullptr;
    const HRESULT hr = DirectDrawCreateEx(guid, &p, iid, nullptr);
    com::Class* cls = com::classForIid(iid);
    if (SUCCEEDED(hr) && !cls) {
        static_cast<IUnknown*>(p)->Release();
        if (arg(c, 1)) wr32(arg(c, 1), 0);
        log("DirectDrawCreateEx: interface {%08lX-...} is not proxied", iid.Data1);
        retStd(c, static_cast<uint32_t>(E_NOINTERFACE), 4);
        return;
    }
    if (arg(c, 1)) wr32(arg(c, 1), SUCCEEDED(hr) ? com::wrapRaw(static_cast<IUnknown*>(p), *cls) : 0);
    log("DirectDrawCreateEx -> 0x%08lX", static_cast<unsigned long>(hr));
    retStd(c, static_cast<uint32_t>(hr), 4);
}

}  // namespace

void ovr_IDirectDraw7_GetAvailableVidMem(Ctx* c) {
    IDirectDraw7* self = com::unwrap<IDirectDraw7>(arg(c, 0));
    DWORD total = 0, avail = 0;
    const HRESULT hr = self->GetAvailableVidMem(arg(c, 1) ? gp<DDSCAPS2>(arg(c, 1)) : nullptr, &total, &avail);
    // Modern drivers (and Wine) report ~4 GB; ConfigDetect rounds up to 64 MB in 32 bits,
    // which wraps to 0 and fails the minimum-video-memory check. 1 GiB also stays positive
    // if anything treats it as signed.
    constexpr DWORD kMax = 0x40000000;
    if (arg(c, 2)) wr32(arg(c, 2), total < kMax ? total : kMax);
    if (arg(c, 3)) wr32(arg(c, 3), avail < kMax ? avail : kMax);
    HLOG(1, "IDirectDraw7::GetAvailableVidMem(caps 0x%X) -> 0x%08lX total %lu free %lu", arg(c, 1) ? rd32(arg(c, 1)) : 0,
         static_cast<unsigned long>(hr), static_cast<unsigned long>(total), static_cast<unsigned long>(avail));
    retStd(c, static_cast<uint32_t>(hr), 4);
}
