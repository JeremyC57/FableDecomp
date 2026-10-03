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

// Key names as Windows reports them (GetKeyNameText style); the game puts them into its
// on-screen prompts ("Press 'Tab' to ...").
const char* keyName(int dik) {
    static const char* names[256] = {};
    static bool init = false;
    if (!init) {
        init = true;
        const struct { int k; const char* n; } t[] = {
            {0x01, "Esc"}, {0x02, "1"}, {0x03, "2"}, {0x04, "3"}, {0x05, "4"}, {0x06, "5"}, {0x07, "6"}, {0x08, "7"}, {0x09, "8"}, {0x0A, "9"},
            {0x0B, "0"}, {0x0C, "-"}, {0x0D, "="}, {0x0E, "Backspace"}, {0x0F, "Tab"}, {0x10, "Q"}, {0x11, "W"}, {0x12, "E"}, {0x13, "R"},
            {0x14, "T"}, {0x15, "Y"}, {0x16, "U"}, {0x17, "I"}, {0x18, "O"}, {0x19, "P"}, {0x1A, "["}, {0x1B, "]"}, {0x1C, "Enter"},
            {0x1D, "Ctrl"}, {0x1E, "A"}, {0x1F, "S"}, {0x20, "D"}, {0x21, "F"}, {0x22, "G"}, {0x23, "H"}, {0x24, "J"}, {0x25, "K"},
            {0x26, "L"}, {0x27, ";"}, {0x28, "'"}, {0x29, "`"}, {0x2A, "Shift"}, {0x2B, "\\"}, {0x2C, "Z"}, {0x2D, "X"}, {0x2E, "C"},
            {0x2F, "V"}, {0x30, "B"}, {0x31, "N"}, {0x32, "M"}, {0x33, ","}, {0x34, "."}, {0x35, "/"}, {0x36, "Right Shift"},
            {0x37, "Num *"}, {0x38, "Alt"}, {0x39, "Space"}, {0x3A, "Caps Lock"}, {0x3B, "F1"}, {0x3C, "F2"}, {0x3D, "F3"}, {0x3E, "F4"},
            {0x3F, "F5"}, {0x40, "F6"}, {0x41, "F7"}, {0x42, "F8"}, {0x43, "F9"}, {0x44, "F10"}, {0x45, "Pause"}, {0x46, "Scroll Lock"},
            {0x47, "Num 7"}, {0x48, "Num 8"}, {0x49, "Num 9"}, {0x4A, "Num -"}, {0x4B, "Num 4"}, {0x4C, "Num 5"}, {0x4D, "Num 6"},
            {0x4E, "Num +"}, {0x4F, "Num 1"}, {0x50, "Num 2"}, {0x51, "Num 3"}, {0x52, "Num 0"}, {0x53, "Num Del"}, {0x56, "\\"},
            {0x57, "F11"}, {0x58, "F12"}, {0x9C, "Num Enter"}, {0x9D, "Right Ctrl"}, {0xB5, "Num /"}, {0xB7, "Prnt Scrn"},
            {0xB8, "Right Alt"}, {0xC5, "Num Lock"}, {0xC7, "Home"}, {0xC8, "Up"}, {0xC9, "Page Up"}, {0xCB, "Left"}, {0xCD, "Right"},
            {0xCF, "End"}, {0xD0, "Down"}, {0xD1, "Page Down"}, {0xD2, "Insert"}, {0xD3, "Delete"}, {0xDB, "Left Windows"},
            {0xDC, "Right Windows"}, {0xDD, "Application"},
        };
        for (auto& e : t) names[e.k] = e.n;
    }
    return dik >= 0 && dik < 256 ? names[dik] : nullptr;
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
                const char* n = keyName(dik);
                if (!n) continue;
                std::memset(&o, 0, sizeof o);
                o.dwSize = sizeof o;
                o.guidType = GUID_Key;
                o.dwOfs = static_cast<DWORD>(dik);
                o.dwType = DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(dik);
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
        if (&prop == &DIPROP_KEYNAME && st.kind == Kind::Keyboard) {
            auto* p = reinterpret_cast<LPDIPROPSTRING>(h);
            const char* n = keyName(static_cast<int>(h->dwHow == DIPH_BYID ? DIDFT_GETINSTANCE(h->dwObj) : h->dwObj));
            if (!n) return DIERR_OBJECTNOTFOUND;
            std::memset(p->wsz, 0, sizeof p->wsz);
            for (int i = 0; n[i] && i < MAX_PATH - 1; ++i) p->wsz[i] = static_cast<unsigned char>(n[i]);
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
    // Objects by offset (DIPH_BYOFFSET) or by id (DIPH_BYID: instance in bits 8..23).
    HRESULT STDMETHODCALLTYPE GetObjectInfo(typename T::ObjInst* o, DWORD obj, DWORD how) override {
        const DWORD size = o->dwSize;
        std::memset(o, 0, size);
        o->dwSize = size;
        const DWORD ofs = how == DIPH_BYID ? DIDFT_GETINSTANCE(obj) : obj;
        if (st.kind == Kind::Keyboard) {
            const char* n = keyName(static_cast<int>(ofs));
            if (how != DIPH_BYOFFSET && how != DIPH_BYID) return DIERR_UNSUPPORTED;
            if (!n) return DIERR_OBJECTNOTFOUND;
            o->guidType = GUID_Key;
            o->dwOfs = ofs;
            o->dwType = DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(ofs);
            T::objName(*o, n);
            return DI_OK;
        }
        static const struct { DWORD ofs; const GUID* g; DWORD type; const char* n; } mouse[] = {
            {DIMOFS_X, &GUID_XAxis, DIDFT_RELAXIS | DIDFT_MAKEINSTANCE(0), "X-axis"},
            {DIMOFS_Y, &GUID_YAxis, DIDFT_RELAXIS | DIDFT_MAKEINSTANCE(1), "Y-axis"},
            {DIMOFS_Z, &GUID_ZAxis, DIDFT_RELAXIS | DIDFT_MAKEINSTANCE(2), "Wheel"},
            {DIMOFS_BUTTON0, &GUID_Button, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(3), "Left Button"},
            {DIMOFS_BUTTON1, &GUID_Button, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(4), "Right Button"},
            {DIMOFS_BUTTON2, &GUID_Button, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(5), "Middle Button"},
            {DIMOFS_BUTTON3, &GUID_Button, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(6), "Button 4"},
            {DIMOFS_BUTTON4, &GUID_Button, DIDFT_PSHBUTTON | DIDFT_MAKEINSTANCE(7), "Button 5"},
        };
        for (auto& m : mouse)
            if ((how == DIPH_BYOFFSET && m.ofs == obj) || (how == DIPH_BYID && (m.type & 0xFFFFFF) == (obj & 0xFFFFFF))) {
                o->guidType = *m.g;
                o->dwOfs = m.ofs;
                o->dwType = m.type;
                T::objName(*o, m.n);
                return DI_OK;
            }
        return DIERR_OBJECTNOTFOUND;
    }
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
