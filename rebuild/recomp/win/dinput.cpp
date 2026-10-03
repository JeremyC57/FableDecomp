// DirectInput 8: the real 64-bit runtime behind guest proxies. Hand-written
// methods convert DIDATAFORMAT / DIDEVICEOBJECTDATA (pointer-sized fields) and
// run enumeration callbacks in guest code.
#define DIRECTINPUT_VERSION 0x0800
#include "com.hpp"
#include "controller.hpp"

#include <dinput.h>

#include <mutex>
#include <unordered_map>
#include <vector>

using namespace host;
using host::com::unwrap;

namespace {

// Which proxied devices are the keyboard and the mouse (from their data formats), so
// controller input can be added to them.
enum class Kind { Other, Keyboard, Mouse };
std::mutex g_kindLock;
std::unordered_map<IUnknown*, Kind> g_kind;
Kind kindOf(void* dev) {
    std::lock_guard<std::mutex> l(g_kindLock);
    auto it = g_kind.find(static_cast<IUnknown*>(dev));
    return it == g_kind.end() ? Kind::Other : it->second;
}

template <class Dev> void setDataFormat(Ctx* c) {
    auto* self = unwrap<Dev>(arg(c, 0));
    const uint32_t g = arg(c, 1);
    // x86 DIDATAFORMAT: dwSize, dwObjSize, dwFlags, dwDataSize, dwNumObjs, rgodf (24 bytes)
    // x86 DIOBJECTDATAFORMAT: pguid, dwOfs, dwType, dwFlags (16 bytes)
    const uint32_t n = rd32(g + 16), objs = rd32(g + 20);
    std::vector<DIOBJECTDATAFORMAT> o(n);
    for (uint32_t i = 0; i < n; ++i) {
        const uint32_t e = objs + 16 * i;
        o[i].pguid = rd32(e) ? gp<const GUID>(rd32(e)) : nullptr;
        o[i].dwOfs = rd32(e + 4);
        o[i].dwType = rd32(e + 8);
        o[i].dwFlags = rd32(e + 12);
    }
    DIDATAFORMAT f{sizeof(DIDATAFORMAT), sizeof(DIOBJECTDATAFORMAT), rd32(g + 8), rd32(g + 12), n, o.data()};
    const HRESULT hr = self->SetDataFormat(&f);
    {
        std::lock_guard<std::mutex> l(g_kindLock);
        const uint32_t size = rd32(g + 12);
        g_kind[reinterpret_cast<IUnknown*>(self)] = size == 256 ? Kind::Keyboard : (size == 16 || size == 20) && n <= 11 ? Kind::Mouse : Kind::Other;
    }
    HLOG(1, "IDirectInputDevice8::SetDataFormat(%u objects, %u bytes) -> 0x%08lX", n, rd32(g + 12), static_cast<unsigned long>(hr));
    retStd(c, static_cast<uint32_t>(hr), 2);
}

template <class Dev> void getDeviceData(Ctx* c) {
    auto* self = unwrap<Dev>(arg(c, 0));
    const uint32_t cb = arg(c, 1), out = arg(c, 2), inout = arg(c, 3), flags = arg(c, 4);
    DWORD n = rd32(inout);
    std::vector<DIDEVICEOBJECTDATA> buf(out ? n : 0);
    const DWORD capacity = n;
    HRESULT hr = self->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), out ? buf.data() : nullptr, &n, flags);
    if (SUCCEEDED(hr) && out && !(flags & DIGDD_PEEK)) {  // add controller input
        buf.resize(n);
        const Kind k = kindOf(self);
        if (k == Kind::Keyboard) pad::injectKeyboardData(buf, capacity);
        else if (k == Kind::Mouse) pad::injectMouseData(buf, capacity);
        n = static_cast<DWORD>(buf.size());
    } else if (SUCCEEDED(hr) && !out && kindOf(self) != Kind::Other) {
        pad::flush();
    }
    if (SUCCEEDED(hr) && out) {
        for (DWORD i = 0; i < n; ++i) {
            const uint32_t e = out + cb * i;  // guest stride (20 for DIDEVICEOBJECTDATA, 16 for the DX3 variant)
            wr32(e, buf[i].dwOfs);
            wr32(e + 4, buf[i].dwData);
            wr32(e + 8, buf[i].dwTimeStamp);
            wr32(e + 12, buf[i].dwSequence);
            if (cb >= 20) wr32(e + 16, static_cast<uint32_t>(buf[i].uAppData));
        }
    }
    wr32(inout, n);
    if (n && out) HLOG(1, "IDirectInputDevice8::GetDeviceData -> %lu events (first ofs 0x%lX data 0x%lX)", n, buf[0].dwOfs, buf[0].dwData);
    static thread_local HRESULT lastHr = S_OK;
    if (hr != lastHr) { HLOG(1, "IDirectInputDevice8::GetDeviceData -> 0x%08lX", static_cast<unsigned long>(hr)); lastHr = hr; }
    retStd(c, static_cast<uint32_t>(hr), 5);
}

template <class DI, class Inst> void enumDevices(Ctx* c) {
    auto* self = unwrap<DI>(arg(c, 0));
    const uint32_t type = arg(c, 1), cb = arg(c, 2), ref = arg(c, 3), flags = arg(c, 4);
    std::vector<Inst> found;
    const HRESULT hr = self->EnumDevices(
        type,
        [](const Inst* d, void* p) -> BOOL {
            static_cast<std::vector<Inst>*>(p)->push_back(*d);
            return DIENUM_CONTINUE;
        },
        &found, flags);
    const uint32_t g = gmalloc(sizeof(Inst));  // layout has no pointers: identical on x86
    for (const Inst& d : found) {
        std::memcpy(gp(g), &d, sizeof d);
        if (guestCall(cb, {g, ref}) == DIENUM_STOP) break;
    }
    gfree(g);
    retStd(c, static_cast<uint32_t>(hr), 5);
}

template <class Dev, class Inst> void enumObjects(Ctx* c) {
    auto* self = unwrap<Dev>(arg(c, 0));
    const uint32_t cb = arg(c, 1), ref = arg(c, 2), flags = arg(c, 3);
    std::vector<Inst> found;
    const HRESULT hr = self->EnumObjects(
        [](const Inst* d, void* p) -> BOOL {
            static_cast<std::vector<Inst>*>(p)->push_back(*d);
            return DIENUM_CONTINUE;
        },
        &found, flags);
    const uint32_t g = gmalloc(sizeof(Inst));
    for (const Inst& d : found) {
        std::memcpy(gp(g), &d, sizeof d);
        if (guestCall(cb, {g, ref}) == DIENUM_STOP) break;
    }
    gfree(g);
    retStd(c, static_cast<uint32_t>(hr), 4);
}

template <class Dev> void getDeviceState(Ctx* c) {
    auto* self = unwrap<Dev>(arg(c, 0));
    const DWORD size = arg(c, 1);
    uint8_t* data = arg(c, 2) ? gp<uint8_t>(arg(c, 2)) : nullptr;
    const HRESULT hr = self->GetDeviceState(size, data);
    if (SUCCEEDED(hr) && data) {
        const Kind k = kindOf(self);
        if (k == Kind::Keyboard) pad::injectKeyboardState(data, size);
        else if (k == Kind::Mouse) pad::injectMouseState(data, size);
    }
    retStd(c, static_cast<uint32_t>(hr), 3);
}

}  // namespace

void ovr_IDirectInputDevice8A_GetDeviceState(Ctx* c) { getDeviceState<IDirectInputDevice8A>(c); }
void ovr_IDirectInputDevice8W_GetDeviceState(Ctx* c) { getDeviceState<IDirectInputDevice8W>(c); }
void ovr_IDirectInput8A_EnumDevices(Ctx* c) { enumDevices<IDirectInput8A, DIDEVICEINSTANCEA>(c); }
void ovr_IDirectInput8W_EnumDevices(Ctx* c) { enumDevices<IDirectInput8W, DIDEVICEINSTANCEW>(c); }
void ovr_IDirectInputDevice8A_SetDataFormat(Ctx* c) { setDataFormat<IDirectInputDevice8A>(c); }
void ovr_IDirectInputDevice8W_SetDataFormat(Ctx* c) { setDataFormat<IDirectInputDevice8W>(c); }
void ovr_IDirectInputDevice8A_GetDeviceData(Ctx* c) { getDeviceData<IDirectInputDevice8A>(c); }
void ovr_IDirectInputDevice8W_GetDeviceData(Ctx* c) { getDeviceData<IDirectInputDevice8W>(c); }
void ovr_IDirectInputDevice8A_EnumObjects(Ctx* c) { enumObjects<IDirectInputDevice8A, DIDEVICEOBJECTINSTANCEA>(c); }
void ovr_IDirectInputDevice8W_EnumObjects(Ctx* c) { enumObjects<IDirectInputDevice8W, DIDEVICEOBJECTINSTANCEW>(c); }

namespace {
IMPORT("dinput8.dll", DirectInput8Create) {
    const GUID& iid = *gp<GUID>(arg(c, 2));
    void* p = nullptr;
    const HRESULT hr = DirectInput8Create(g_hinst, arg(c, 1), iid, &p, nullptr);
    com::Class* cls = com::classForIid(iid);
    if (arg(c, 3)) wr32(arg(c, 3), SUCCEEDED(hr) && cls ? com::wrapRaw(static_cast<IUnknown*>(p), *cls) : 0);
    log("DirectInput8Create(0x%X) -> 0x%08lX", arg(c, 1), static_cast<unsigned long>(hr));
    retStd(c, static_cast<uint32_t>(hr), 5);
}
}  // namespace
