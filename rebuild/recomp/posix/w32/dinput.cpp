// DirectInput 8 on SDL: the system keyboard and mouse (controllers reach the game through
// the host's XInput mapping, controller.cpp). Both devices use the standard data formats
// (c_dfDIKeyboard: 256 bytes indexed by DIK code; c_dfDIMouse/2: lX, lY, lZ, buttons).
#define DIRECTINPUT_VERSION 0x0800
#include <SDL.h>

#include <deque>
#include <set>

#include "comobj.hpp"
#include "keymap.hpp"
#include "w32sdl.hpp"
#include <dinput.h>

namespace w32 {
namespace {

enum class Kind { Keyboard, Mouse };

struct DeviceState {
    Kind kind;
    std::mutex lock;
    bool acquired = false, exclusive = false;
    DWORD bufferSize = 0;
    std::deque<DIDEVICEOBJECTDATA> buffer;
    bool overflow = false;
    uint8_t keys[256] = {};
    LONG dx = 0, dy = 0, dz = 0;  // since the last GetDeviceState
    uint8_t buttons[8] = {};
    DWORD sequence = 0;
    HANDLE event = nullptr;
    void push(DWORD ofs, DWORD data) {
        if (!acquired || !bufferSize) return;
        if (buffer.size() >= bufferSize) { buffer.pop_front(); overflow = true; }
        DIDEVICEOBJECTDATA d{};
        d.dwOfs = ofs;
        d.dwData = data;
        d.dwTimeStamp = GetTickCount();
        d.dwSequence = ++sequence;
        buffer.push_back(d);
    }
};

std::mutex g_devLock;
std::set<DeviceState*> g_devices;
bool g_hooked = false;

void onEvent(const SDL_Event& e) {
    std::lock_guard<std::mutex> l(g_devLock);
    for (DeviceState* d : g_devices) {
        std::lock_guard<std::mutex> dl(d->lock);
        if (d->kind == Kind::Keyboard && (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)) {
            if (e.key.repeat) continue;
            const KeyInfo k = keyInfo(e.key.keysym.scancode);
            if (!k.scan) continue;
            const uint8_t dik = static_cast<uint8_t>(k.scan | (k.extended ? 0x80 : 0));
            const bool down = e.type == SDL_KEYDOWN;
            if (d->acquired) d->keys[dik] = down ? 0x80 : 0;
            d->push(dik, down ? 0x80 : 0);
        } else if (d->kind == Kind::Mouse) {
            if (e.type == SDL_MOUSEMOTION) {
                if (!d->acquired) continue;
                d->dx += e.motion.xrel;
                d->dy += e.motion.yrel;
                if (e.motion.xrel) d->push(DIMOFS_X, static_cast<DWORD>(e.motion.xrel));
                if (e.motion.yrel) d->push(DIMOFS_Y, static_cast<DWORD>(e.motion.yrel));
            } else if (e.type == SDL_MOUSEWHEEL) {
                if (!d->acquired) continue;
                const LONG z = e.wheel.y * WHEEL_DELTA;
                d->dz += z;
                d->push(DIMOFS_Z, static_cast<DWORD>(z));
            } else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
                int b = -1;
                switch (e.button.button) {
                    case SDL_BUTTON_LEFT: b = 0; break;
                    case SDL_BUTTON_RIGHT: b = 1; break;
                    case SDL_BUTTON_MIDDLE: b = 2; break;
                    case SDL_BUTTON_X1: b = 3; break;
                    case SDL_BUTTON_X2: b = 4; break;
                    default: break;
                }
                if (b < 0) continue;
                const bool down = e.type == SDL_MOUSEBUTTONDOWN;
                if (d->acquired) d->buttons[b] = down ? 0x80 : 0;
                d->push(static_cast<DWORD>(DIMOFS_BUTTON0 + b), down ? 0x80 : 0);
            }
        } else {
            continue;
        }
        if (d->event) SetEvent(d->event);
    }
}

template <bool W> struct Traits;
template <> struct Traits<true> {
    using Dev = IDirectInputDevice8W;
    using DI = IDirectInput8W;
    using Inst = DIDEVICEINSTANCEW;
    using ObjInst = DIDEVICEOBJECTINSTANCEW;
    using EnumDevCb = LPDIENUMDEVICESCALLBACKW;
    using EnumObjCb = LPDIENUMDEVICEOBJECTSCALLBACKW;
    using EnumEffCb = LPDIENUMEFFECTSCALLBACKW;
    using EffInfo = LPDIEFFECTINFOW;
    using ActionFmt = LPDIACTIONFORMATW;
    using ImageHdr = LPDIDEVICEIMAGEINFOHEADERW;
    using Str = LPCWSTR;
    using SemCb = LPDIENUMDEVICESBYSEMANTICSCBW;
    using CfgParams = LPDICONFIGUREDEVICESPARAMSW;
    static const IID& devIid() { return IID_IDirectInputDevice8W; }
    static const IID& diIid() { return IID_IDirectInput8W; }
    static void name(Inst& i, const char* inst, const char* prod) {
        for (int k = 0; inst[k] && k < MAX_PATH - 1; ++k) i.tszInstanceName[k] = inst[k];
        for (int k = 0; prod[k] && k < MAX_PATH - 1; ++k) i.tszProductName[k] = prod[k];
    }
    static void objName(ObjInst& o, const char* n) { for (int k = 0; n[k] && k < MAX_PATH - 1; ++k) o.tszName[k] = n[k]; }
};
template <> struct Traits<false> {
    using Dev = IDirectInputDevice8A;
    using DI = IDirectInput8A;
    using Inst = DIDEVICEINSTANCEA;
    using ObjInst = DIDEVICEOBJECTINSTANCEA;
    using EnumDevCb = LPDIENUMDEVICESCALLBACKA;
    using EnumObjCb = LPDIENUMDEVICEOBJECTSCALLBACKA;
    using EnumEffCb = LPDIENUMEFFECTSCALLBACKA;
    using EffInfo = LPDIEFFECTINFOA;
    using ActionFmt = LPDIACTIONFORMATA;
    using ImageHdr = LPDIDEVICEIMAGEINFOHEADERA;
    using Str = LPCSTR;
    using SemCb = LPDIENUMDEVICESBYSEMANTICSCBA;
    using CfgParams = LPDICONFIGUREDEVICESPARAMSA;
    static const IID& devIid() { return IID_IDirectInputDevice8A; }
    static const IID& diIid() { return IID_IDirectInput8A; }
    static void name(Inst& i, const char* inst, const char* prod) {
        std::strncpy(i.tszInstanceName, inst, MAX_PATH - 1);
        std::strncpy(i.tszProductName, prod, MAX_PATH - 1);
    }
    static void objName(ObjInst& o, const char* n) { std::strncpy(o.tszName, n, MAX_PATH - 1); }
};

template <bool W> void describe(Kind k, typename Traits<W>::Inst& i) {
    const DWORD size = i.dwSize;
    std::memset(&i, 0, size);
    i.dwSize = size;
    i.guidInstance = i.guidProduct = k == Kind::Keyboard ? GUID_SysKeyboard : GUID_SysMouse;
    i.dwDevType = k == Kind::Keyboard ? (DI8DEVTYPE_KEYBOARD | (DI8DEVTYPEKEYBOARD_PCENH << 8)) : (DI8DEVTYPE_MOUSE | (DI8DEVTYPEMOUSE_UNKNOWN << 8));
    Traits<W>::name(i, k == Kind::Keyboard ? "Keyboard" : "Mouse", k == Kind::Keyboard ? "Keyboard" : "Mouse");
}

template <bool W> struct Device final : ComObject<typename Traits<W>::Dev> {
    using T = Traits<W>;
    DeviceState st;
    explicit Device(Kind k) {
        st.kind = k;
        std::lock_guard<std::mutex> l(g_devLock);
        g_devices.insert(&st);
        if (!g_hooked) { g_hooked = true; addSdlEventHook(onEvent); }
    }
    ~Device() override {
        Unacquire();
        std::lock_guard<std::mutex> l(g_devLock);
        g_devices.erase(&st);
    }
    bool supports(REFIID iid) const override { return iid == T::devIid(); }

    HRESULT STDMETHODCALLTYPE GetCapabilities(LPDIDEVCAPS caps) override {
        const DWORD size = caps->dwSize;
        std::memset(caps, 0, size);
        caps->dwSize = size;
        caps->dwFlags = DIDC_ATTACHED;
        if (st.kind == Kind::Keyboard) caps->dwDevType = DI8DEVTYPE_KEYBOARD | (DI8DEVTYPEKEYBOARD_PCENH << 8), caps->dwButtons = 128;
        else caps->dwDevType = DI8DEVTYPE_MOUSE, caps->dwAxes = 3, caps->dwButtons = 5;
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE EnumObjects(typename T::EnumObjCb cb, LPVOID ref, DWORD) override {
        typename T::ObjInst o{};
        o.dwSize = sizeof o;
        if (st.kind == Kind::Mouse) {
            const struct { const GUID* g; DWORD ofs, type; const char* n; } objs[] = {
                {&GUID_XAxis, DIMOFS_X, DIDFT_RELAXIS | DIDFT_MAKEINSTANCE(0), "X Axis"},
                {&GUID_YAxis, DIMOFS_Y, DIDFT_RELAXIS | DIDFT_MAKEINSTANCE(1), "Y Axis"},
                {&GUID_ZAxis, DIMOFS_Z, DIDFT_RELAXIS | DIDFT_MAKEINSTANCE(2), "Wheel"},
                {&GUID_Button, DIMOFS_BUTTON0, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(3), "Button 0"},
                {&GUID_Button, DIMOFS_BUTTON1, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(4), "Button 1"},
                {&GUID_Button, DIMOFS_BUTTON2, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(5), "Button 2"},
            };
            for (auto& x : objs) {
                std::memset(&o, 0, sizeof o);
                o.dwSize = sizeof o;
                o.guidType = *x.g;
                o.dwOfs = x.ofs;
                o.dwType = x.type;
                T::objName(o, x.n);
                if (cb(&o, ref) == DIENUM_STOP) break;
            }
        } else {
            for (int dik = 1; dik < 256; ++dik) {
                std::memset(&o, 0, sizeof o);
                o.dwSize = sizeof o;
                o.guidType = GUID_Key;
                o.dwOfs = static_cast<DWORD>(dik);
                o.dwType = DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(dik);
                char n[16];
                std::snprintf(n, sizeof n, "Key %d", dik);
                T::objName(o, n);
                if (cb(&o, ref) == DIENUM_STOP) break;
            }
        }
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE GetProperty(REFGUID prop, LPDIPROPHEADER h) override {
        if (&prop == &DIPROP_BUFFERSIZE) {
            reinterpret_cast<LPDIPROPDWORD>(h)->dwData = st.bufferSize;
            return DI_OK;
        }
        if (&prop == &DIPROP_AXISMODE) {
            reinterpret_cast<LPDIPROPDWORD>(h)->dwData = DIPROPAXISMODE_REL;
            return DI_OK;
        }
        return DIERR_UNSUPPORTED;
    }
    HRESULT STDMETHODCALLTYPE SetProperty(REFGUID prop, LPCDIPROPHEADER h) override {
        // DIPROP_* are MAKEDIPROP(n) pseudo-GUID pointers: compare addresses.
        if (&prop == &DIPROP_BUFFERSIZE) {
            std::lock_guard<std::mutex> l(st.lock);
            st.bufferSize = reinterpret_cast<LPCDIPROPDWORD>(h)->dwData;
        }
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE Acquire() override {
        std::lock_guard<std::mutex> l(st.lock);
        if (st.acquired) return S_FALSE;
        st.acquired = true;
        if (st.kind == Kind::Mouse && st.exclusive && SDL_WasInit(SDL_INIT_VIDEO)) SDL_SetRelativeMouseMode(SDL_TRUE);
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE Unacquire() override {
        std::lock_guard<std::mutex> l(st.lock);
        if (!st.acquired) return DI_NOEFFECT;
        st.acquired = false;
        std::memset(st.keys, 0, sizeof st.keys);
        std::memset(st.buttons, 0, sizeof st.buttons);
        st.buffer.clear();
        if (st.kind == Kind::Mouse && st.exclusive && SDL_WasInit(SDL_INIT_VIDEO)) SDL_SetRelativeMouseMode(SDL_FALSE);
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE GetDeviceState(DWORD size, LPVOID data) override {
        pumpSdlEvents();
        std::lock_guard<std::mutex> l(st.lock);
        if (!st.acquired) return DIERR_NOTACQUIRED;
        std::memset(data, 0, size);
        if (st.kind == Kind::Keyboard) {
            std::memcpy(data, st.keys, std::min<DWORD>(size, 256));
        } else {
            auto* m = static_cast<uint8_t*>(data);
            const LONG v[3] = {st.dx, st.dy, st.dz};
            std::memcpy(m, v, std::min<DWORD>(size, 12));
            if (size > 12) std::memcpy(m + 12, st.buttons, std::min<DWORD>(size - 12, 8));
            st.dx = st.dy = st.dz = 0;
        }
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE GetDeviceData(DWORD cb, LPDIDEVICEOBJECTDATA out, LPDWORD inout, DWORD flags) override {
        pumpSdlEvents();
        std::lock_guard<std::mutex> l(st.lock);
        if (!st.acquired) { *inout = 0; return DIERR_NOTACQUIRED; }
        if (!st.bufferSize) { *inout = 0; return DIERR_NOTBUFFERED; }
        const HRESULT hr = st.overflow ? DI_BUFFEROVERFLOW : DI_OK;
        if (!(flags & DIGDD_PEEK)) st.overflow = false;
        DWORD n = 0;
        if (!out) {
            n = static_cast<DWORD>(std::min<size_t>(st.buffer.size(), *inout));
            if (!(flags & DIGDD_PEEK)) st.buffer.erase(st.buffer.begin(), st.buffer.begin() + n);
            *inout = n;
            return hr;
        }
        for (; n < *inout && n < st.buffer.size(); ++n) std::memcpy(reinterpret_cast<uint8_t*>(out) + n * cb, &st.buffer[n], std::min<DWORD>(cb, sizeof(DIDEVICEOBJECTDATA)));
        if (!(flags & DIGDD_PEEK)) st.buffer.erase(st.buffer.begin(), st.buffer.begin() + n);
        *inout = n;
        return hr;
    }
    HRESULT STDMETHODCALLTYPE SetDataFormat(LPCDIDATAFORMAT) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE SetEventNotification(HANDLE h) override { st.event = h; return DI_OK; }
    HRESULT STDMETHODCALLTYPE SetCooperativeLevel(HWND, DWORD flags) override {
        st.exclusive = (flags & DISCL_EXCLUSIVE) && st.kind == Kind::Mouse;
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE GetObjectInfo(typename T::ObjInst*, DWORD, DWORD) override { return DIERR_OBJECTNOTFOUND; }
    HRESULT STDMETHODCALLTYPE GetDeviceInfo(typename T::Inst* i) override { describe<W>(st.kind, *i); return DI_OK; }
    HRESULT STDMETHODCALLTYPE RunControlPanel(HWND, DWORD) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE, DWORD, REFGUID) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE CreateEffect(REFGUID, LPCDIEFFECT, LPDIRECTINPUTEFFECT*, LPUNKNOWN) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumEffects(typename T::EnumEffCb, LPVOID, DWORD) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE GetEffectInfo(typename T::EffInfo, REFGUID) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE GetForceFeedbackState(LPDWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE SendForceFeedbackCommand(DWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumCreatedEffectObjects(LPDIENUMCREATEDEFFECTOBJECTSCALLBACK, LPVOID, DWORD) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE Escape(LPDIEFFESCAPE) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE Poll() override { pumpSdlEvents(); return DI_NOEFFECT; }
    HRESULT STDMETHODCALLTYPE SendDeviceData(DWORD, LPCDIDEVICEOBJECTDATA, LPDWORD, DWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE EnumEffectsInFile(typename T::Str, LPDIENUMEFFECTSINFILECALLBACK, LPVOID, DWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE WriteEffectToFile(typename T::Str, DWORD, LPDIFILEEFFECT, DWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE BuildActionMap(typename T::ActionFmt, typename T::Str, DWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE SetActionMap(typename T::ActionFmt, typename T::Str, DWORD) override { return DIERR_UNSUPPORTED; }
    HRESULT STDMETHODCALLTYPE GetImageInfo(typename T::ImageHdr) override { return DIERR_UNSUPPORTED; }
};

template <bool W> struct DirectInput final : ComObject<typename Traits<W>::DI> {
    using T = Traits<W>;
    bool supports(REFIID iid) const override { return iid == T::diIid(); }
    HRESULT STDMETHODCALLTYPE CreateDevice(REFGUID g, typename T::Dev** out, LPUNKNOWN) override {
        if (g == GUID_SysKeyboard) *out = new Device<W>(Kind::Keyboard);
        else if (g == GUID_SysMouse) *out = new Device<W>(Kind::Mouse);
        else { *out = nullptr; return DIERR_DEVICENOTREG; }
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE EnumDevices(DWORD type, typename T::EnumDevCb cb, LPVOID ref, DWORD) override {
        for (Kind k : {Kind::Keyboard, Kind::Mouse}) {
            const DWORD cls = type & 0xFF;
            if (cls != DI8DEVCLASS_ALL && !(k == Kind::Keyboard && (cls == DI8DEVCLASS_KEYBOARD || cls == DI8DEVTYPE_KEYBOARD)) &&
                !(k == Kind::Mouse && (cls == DI8DEVCLASS_POINTER || cls == DI8DEVTYPE_MOUSE)))
                continue;
            typename T::Inst i{};
            i.dwSize = sizeof i;
            describe<W>(k, i);
            if (cb(&i, ref) == DIENUM_STOP) break;
        }
        return DI_OK;
    }
    HRESULT STDMETHODCALLTYPE GetDeviceStatus(REFGUID g) override { return g == GUID_SysKeyboard || g == GUID_SysMouse ? DI_OK : DI_NOTATTACHED; }
    HRESULT STDMETHODCALLTYPE RunControlPanel(HWND, DWORD) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE Initialize(HINSTANCE, DWORD) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE FindDevice(REFGUID, typename T::Str, LPGUID) override { return DIERR_DEVICENOTREG; }
    HRESULT STDMETHODCALLTYPE EnumDevicesBySemantics(typename T::Str, typename T::ActionFmt, typename T::SemCb, LPVOID, DWORD) override { return DI_OK; }
    HRESULT STDMETHODCALLTYPE ConfigureDevices(LPDICONFIGUREDEVICESCALLBACK, typename T::CfgParams, DWORD, LPVOID) override { return DIERR_UNSUPPORTED; }
};

}  // namespace
}  // namespace w32

using namespace w32;

extern "C" HRESULT WINAPI DirectInput8Create(HINSTANCE, DWORD, REFIID iid, LPVOID* out, LPUNKNOWN) {
    *out = nullptr;
    if (iid == IID_IDirectInput8W) *out = static_cast<IDirectInput8W*>(new DirectInput<true>);
    else if (iid == IID_IDirectInput8A) *out = static_cast<IDirectInput8A*>(new DirectInput<false>);
    else return E_NOINTERFACE;
    return DI_OK;
}
