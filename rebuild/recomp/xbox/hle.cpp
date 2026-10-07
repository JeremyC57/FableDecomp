// HLE replacements for statically linked XDK library functions, found by address in
// default.xbe (XbSymbolDatabase) and swapped in by the lifter (--hook ADDR=hle_Name).
//
// Input: XAPI's USB stack (XInitDevices, the XID driver) is replaced; XInput reads the host
// controller state (input.cpp). One gamepad is reported in port 1.
#include "input.hpp"
#include "settings.hpp"
#include "xhost.hpp"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include <mutex>

namespace xb {

void hleInstall() {}

namespace {
constexpr uint32_t kTypeGamepad = 0x0086E558;  // XDEVICE_TYPE_GAMEPAD (g_DeviceType_Gamepad)
constexpr uint32_t kHandleBase = 0x4E500000;   // XInputOpen handles: base + port
bool g_reported[4];

// Lifted-function contract: [esp] is the return address; stdcall pops `args` words.
void ret(Ctx* c, uint32_t value, int args) {
    c->eax = value;
    c->esp += 4 + 4u * static_cast<uint32_t>(args);
}
uint32_t arg(Ctx* c, int i) { return rd32(c->esp + 4 + 4 * i); }
}  // namespace

} // namespace xb

using namespace xb;

extern "C" {

// HRESULT Direct3D_CreateDevice(UINT, D3DDEVTYPE, HWND, DWORD, D3DPRESENT_PARAMETERS*, IDirect3DDevice8**)
// fps = 60: FullScreen_PresentationInterval (+0x30) becomes D3DPRESENT_INTERVAL_ONE, so the
// game flips every vblank instead of every other one.
void F_00851360_orig(Ctx* c);
// _CIfmod (x in ST(1), y in ST(0); result fmod(x, y) in ST(0), one pop) through the CRT's
// _trandisp2 path. Debug check: log when it returns NaN where the C library does not.
extern "C" void F_005806FE_orig(Ctx* c);
void hle_CIfmod(Ctx* c) {
    const double y = c->fpu.st[c->fpu.top & 7], x = c->fpu.st[(c->fpu.top + 1) & 7];
    F_005806FE_orig(c);
    const double r = c->fpu.st[c->fpu.top & 7];
    static int logged = 0;
    if (std::isnan(r) && !std::isnan(std::fmod(x, y)) && logged < 20) {
        ++logged;
        XLOG(0, "_CIfmod(%g, %g) = NaN (libc %g), cw %04X sw %04X", x, y, std::fmod(x, y), c->fpu.cw, c->fpu.sw);
    }
}

void hle_Direct3D_CreateDevice(Ctx* c) {
    const uint32_t pp = arg(c, 4);
    if (pp) {
        const uint32_t interval = rd32(pp + 0x30);
        XLOG(1, "Direct3D_CreateDevice: %ux%u, presentation interval 0x%X%s", rd32(pp), rd32(pp + 4), interval,
             settings().fps >= 60 ? " -> 1 (60 fps)" : "");
        if (settings().fps >= 60) wr32(pp + 0x30, 1);
    }
    F_00851360_orig(c);
}

// The game picks its present interval itself: CGraphics::SetTargetFrameRate (0x3F700, this in
// ecx; float fps, bool orImmediate; ret 8) turns the region's LockToFrameRate (30) into
// n = ceil(refresh / fps) vblanks per frame, stores the D3D present interval
// (D3D__RenderState[PRESENTATIONINTERVAL], 0x862494) and the frame period n / refresh (+0x1C4) that
// the render interpolation predicts present times with. 0x3F820 (bool, int n, bool) sets n
// directly (some modes force 2). The simulation runs on real time (15 server turns per second,
// rendering interpolates), so fps = 60 asks for 60 here and turns forced 2s into 1s.
void F_0003F700_orig(Ctx* c);
void F_0003F820_orig(Ctx* c);
void hle_SetTargetFrameRate(Ctx* c) {
    float fps;
    std::memcpy(&fps, gp(c->esp + 4), 4);
    if (settings().fps >= 60 && fps > 0.0f && fps < 60.0f) {
        static int logged = 0;
        if (logged++ < 4) XLOG(1, "frame rate: the game asks for %.0f fps, using 60", fps);
        const float sixty = 60.0f;
        std::memcpy(gp(c->esp + 4), &sixty, 4);
    }
    const uint32_t self = c->ecx;
    F_0003F700_orig(c);
    XLOG(2, "frame rate: present interval 0x%X, frame period %g s", rd32(0x862494), static_cast<double>(*reinterpret_cast<const float*>(gp(self + 0x1C4))));
}
void hle_SetPresentInterval(Ctx* c) {
    if (settings().fps >= 60 && rd32(c->esp + 8) == 2) wr32(c->esp + 8, 1);
    F_0003F820_orig(c);
}

// CEngineCamera::SetupGamut (0x132D00; this in ebx, CEngineCameraDesc* on the stack). It sets
// AspectRatio (+0xD0) = window width / height and the homogeneous view scales (+0xD4/+0xD8)
// from the description's FOV (+0x4C), or from separate X/Y FOVs (+0x4C/+0x50) when the
// description's flag at +0x54 is set (nothing is computed while the flag at +0x68 is clear);
// the frustum planes and projection follow from them.
// aspect = 16:9: a 4:3 screen-sized window (some cameras use tiny normalised ones) gets explicit FOVs for the same vertical view and a 16:9-wide
// horizontal one, so the game projects and culls a widescreen view into its 640x480 buffer and
// the presenter stretches that to 16:9 (anamorphic, as Xbox widescreen games do).
void F_00132D00_orig(Ctx* c);
// Widened cameras: camera object -> (horizontal, vertical) FOV given to SetupGamut.
std::mutex g_wideLock;
std::unordered_map<uint32_t, std::pair<float, float>> g_wideFov;
void hle_SetupGamut(Ctx* c) {
    const uint32_t desc = arg(c, 0);
    float x0, y0, x1, y1, fov;
    std::memcpy(&x0, gp(desc + 0x0), 4);
    std::memcpy(&y0, gp(desc + 0x4), 4);
    std::memcpy(&x1, gp(desc + 0x8), 4);
    std::memcpy(&y1, gp(desc + 0xC), 4);
    std::memcpy(&fov, gp(desc + 0x4C), 4);
    const float w = x1 - x0, h = y1 - y0;
    static const bool off = featureOff("hor+");
    const bool widen = settings().widescreen && !off && rd8(desc + 0x68) && !rd8(desc + 0x54) && w >= 320 && h > 0 &&
                       std::fabs(h / w - 0.75f) < 0.01f && fov > 0.0f && fov < 3.1f && !rd32(0x9A1188);
    if (getenv("FABLE_GAMUT_LOG")) {
        static int n = 0;
        float fy;
        std::memcpy(&fy, gp(desc + 0x50), 4);
        if (n++ < 40)
            XLOG(0, "SetupGamut in: window %g,%g-%g,%g fov %g/%g flags %u/%u override %08X -> %s", x0, y0, x1, y1, fov, fy, rd8(desc + 0x54),
                 rd8(desc + 0x68), rd32(0x9A1188), widen ? "widen" : "as is");
    }
    if (!widen) {
        F_00132D00_orig(c);
        return;
    }
    // Normal path: scaleX = 1 / tan(fov / 2), scaleY = scaleX * 4/3. Widescreen: scaleY the same,
    // scaleX = scaleY / (16/9) = 0.75 * the 4:3 value.
    const float t = std::tan(0.5f * fov), fovX = 2.0f * std::atan(t / 0.75f), fovY = 2.0f * std::atan(t * 0.75f);
    uint32_t saved[3];
    std::memcpy(saved, gp(desc + 0x4C), 12);
    std::memcpy(gp(desc + 0x4C), &fovX, 4);
    std::memcpy(gp(desc + 0x50), &fovY, 4);
    wr8(desc + 0x54, 1);
    F_00132D00_orig(c);
    {
        std::lock_guard<std::mutex> l(g_wideLock);
        g_wideFov[c->ebx] = {fovX, fovY};
    }
    std::memcpy(gp(desc + 0x4C), saved, 12);
    std::memcpy(gp(c->ebx + 0x14 + 0x4C), saved, 12);  // the camera's copy of the description
    static float logged[8];  // each distinct FOV once (cutscenes alternate cameras every frame)
    static int nLogged = 0;
    if (nLogged < 8 && std::find(logged, logged + nLogged, fov) == logged + nLogged) {
        logged[nLogged++] = fov;
        XLOG(1, "SetupGamut: %gx%g window, fov %g -> widescreen %g x %g", w, h, fov, fovX, fovY);
    }
}

// The sky/horizon band (0x161830) maps the screen's width onto the panorama from the main camera's
// stored FOV (camera +0x60, or +0x64 for the vertical with the explicit-FOV flag +0x68). The camera
// keeps the 4:3 description (other code copies it), so for a widened camera the band would be
// 4:3-narrow and slide against the terrain; it is shown the widened FOVs for the call.
void F_00161830_orig(Ctx* c);
void hle_SkyBand(Ctx* c) {
    const uint32_t cam = rd32(0x994BC4);
    std::pair<float, float> fov{0, 0};
    bool wide = false;
    if (cam && !rd8(cam + 0x68)) {
        std::lock_guard<std::mutex> l(g_wideLock);
        const auto it = g_wideFov.find(cam);
        if (it != g_wideFov.end()) { fov = it->second; wide = true; }
    }
    if (!wide) {
        F_00161830_orig(c);
        return;
    }
    uint8_t saved[12];
    std::memcpy(saved, gp(cam + 0x60), 12);
    std::memcpy(gp(cam + 0x60), &fov.first, 4);
    std::memcpy(gp(cam + 0x64), &fov.second, 4);
    wr8(cam + 0x68, 1);
    F_00161830_orig(c);
    std::memcpy(gp(cam + 0x60), saved, 12);
}

// The game's operator new (0x1FBF0, cdecl: size) zero-fills: Fable relies on fresh blocks
// reading as zero in places (a HUD element's graphic pointer at +0x28 is only ever tested
// for null), which held on the console's heap layout but not when a block is reused here.
void F_0001FBF0_orig(Ctx* c);
void hle_operator_new(Ctx* c) {
    const uint32_t size = arg(c, 0);
    F_0001FBF0_orig(c);
    static const bool off = featureOff("zerofill");
    if (c->eax && size && !off) std::memset(gp(c->eax), 0, size);
}

// VOID XInitDevices(DWORD dwPreallocTypeCount, PXDEVICE_PREALLOC_TYPE PreallocTypes)
void hle_XInitDevices(Ctx* c) {
    XLOG(1, "HLE XInitDevices: host input replaces the USB stack");
    ret(c, 0, 2);
}

// DWORD XGetDevices(PXPP_DEVICE_TYPE DeviceType): bit per connected port
void hle_XGetDevices(Ctx* c) {
    const uint32_t type = arg(c, 0);
    uint32_t mask = 0;
    if (type == kTypeGamepad) {
        mask = input::connectedMask();
        for (int p = 0; p < 4; ++p) g_reported[p] = (mask >> p) & 1;
    }
    ret(c, mask, 1);
}

// BOOL XGetDeviceChanges(PXPP_DEVICE_TYPE DeviceType, PDWORD pdwInsertions, PDWORD pdwRemovals)
void hle_XGetDeviceChanges(Ctx* c) {
    const uint32_t type = arg(c, 0), pIns = arg(c, 1), pRem = arg(c, 2);
    uint32_t ins = 0, rem = 0;
    if (type == kTypeGamepad) {
        const uint32_t now = input::connectedMask();
        for (int p = 0; p < 4; ++p) {
            const bool on = (now >> p) & 1;
            if (on && !g_reported[p]) ins |= 1u << p;
            if (!on && g_reported[p]) rem |= 1u << p;
            g_reported[p] = on;
        }
    }
    if (pIns) wr32(pIns, ins);
    if (pRem) wr32(pRem, rem);
    ret(c, (ins | rem) ? 1 : 0, 3);
}

// HANDLE XInputOpen(PXPP_DEVICE_TYPE DeviceType, DWORD dwPort, DWORD dwSlot, PXINPUT_POLLING_PARAMETERS)
void hle_XInputOpen(Ctx* c) {
    const uint32_t type = arg(c, 0), port = arg(c, 1);
    const bool ok = type == kTypeGamepad && port < 4;
    XLOG(1, "HLE XInputOpen(type 0x%08X, port %u) -> %s", type, port, ok ? "gamepad" : "none");
    ret(c, ok ? kHandleBase + port : 0, 4);
}

// VOID XInputClose(HANDLE hDevice)
void hle_XInputClose(Ctx* c) { ret(c, 0, 1); }

// DWORD XInputGetState(HANDLE hDevice, PXINPUT_STATE pState)
// XINPUT_STATE: DWORD dwPacketNumber; XINPUT_GAMEPAD { WORD wButtons; BYTE bAnalogButtons[8];
// SHORT sThumbLX, sThumbLY, sThumbRX, sThumbRY; }
void hle_XInputGetState(Ctx* c) {
    const uint32_t h = arg(c, 0), out = arg(c, 1);
    const uint32_t port = h - kHandleBase;
    if (port >= 4 || !((input::connectedMask() >> port) & 1)) {
        ret(c, 1167, 2);  // ERROR_DEVICE_NOT_CONNECTED
        return;
    }
    const input::Pad p = input::pad(static_cast<int>(port));
    static uint32_t calls = 0, lastPacket = 0;
    if (++calls % 300 == 1 || (p.packet != lastPacket && (p.buttons || p.analog[0])))
        XLOG(1, "HLE XInputGetState(port %u): call %u, packet %u, buttons 0x%04X, A %u", port, calls, p.packet, p.buttons, p.analog[0]);
    lastPacket = p.packet;
    wr32(out, p.packet);
    wr16(out + 4, p.buttons);
    for (int i = 0; i < 8; ++i) wr8(out + 6 + i, p.analog[i]);
    wr16(out + 14, static_cast<uint16_t>(p.lx));
    wr16(out + 16, static_cast<uint16_t>(p.ly));
    wr16(out + 18, static_cast<uint16_t>(p.rx));
    wr16(out + 20, static_cast<uint16_t>(p.ry));
    ret(c, 0, 2);
}

// DWORD XInputSetState(HANDLE hDevice, PXINPUT_FEEDBACK pFeedback)
// XINPUT_FEEDBACK_HEADER { DWORD dwStatus; HANDLE hEvent; BYTE Reserved[58]; } then
// XINPUT_RUMBLE { WORD wLeftMotorSpeed; WORD wRightMotorSpeed; }
void hle_XInputSetState(Ctx* c) {
    const uint32_t h = arg(c, 0), fb = arg(c, 1);
    const uint32_t port = h - kHandleBase;
    if (port < 4) input::rumble(static_cast<int>(port), rd16(fb + 66), rd16(fb + 68));
    wr32(fb, 0);  // dwStatus = ERROR_SUCCESS (completed at once)
    if (const uint32_t ev = rd32(fb + 4)) {
        Handle* hh = handleGet(ev);
        if (hh && hh->kind == Handle::Dispatcher) keSetEvent(hh->object);
    }
    ret(c, 997, 2);  // ERROR_IO_PENDING, as on the console
}

}  // extern "C"
