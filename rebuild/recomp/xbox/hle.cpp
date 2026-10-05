// HLE replacements for statically linked XDK library functions, found by address in
// default.xbe (XbSymbolDatabase) and swapped in by the lifter (--hook ADDR=hle_Name).
//
// Input: XAPI's USB stack (XInitDevices, the XID driver) is replaced; XInput reads the host
// controller state (input.cpp). One gamepad is reported in port 1.
#include "input.hpp"
#include "xhost.hpp"

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
