// XInput on SDL game controllers: "xinput1_4.dll" (and the older names) as a host module,
// so the host's controller mapping (controller.cpp) works unchanged.
#include <SDL.h>

#include "w32sdl.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <xinput.h>

namespace w32 {
void registerHostModule(const char* dll, const char* fn, void* addr);

// On-screen touch controls (Android): a virtual pad merged into controller 0.
namespace {
std::mutex g_virtLock;
bool g_virtActive;
XINPUT_GAMEPAD g_virt;
}  // namespace
void setVirtualPad(bool active, const XINPUT_GAMEPAD& g) {
    std::lock_guard<std::mutex> l(g_virtLock);
    g_virtActive = active;
    g_virt = g;
}

namespace {
std::mutex g_lock;
SDL_GameController* g_pads[XUSER_MAX_COUNT];
DWORD g_packet[XUSER_MAX_COUNT];
bool g_init;

void ensure() {
    if (g_init) return;
    g_init = true;
    SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");
    if (SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER) != 0) std::fprintf(stderr, "SDL game controller init failed: %s\n", SDL_GetError());
    addSdlEventHook([](const SDL_Event& e) {
        if (e.type != SDL_CONTROLLERDEVICEADDED && e.type != SDL_CONTROLLERDEVICEREMOVED) return;
        std::lock_guard<std::mutex> l(g_lock);
        for (auto& p : g_pads)
            if (p && !SDL_GameControllerGetAttached(p)) SDL_GameControllerClose(p), p = nullptr;
        if (e.type == SDL_CONTROLLERDEVICEADDED) {
            for (auto& p : g_pads)
                if (p && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(p)) == SDL_JoystickGetDeviceInstanceID(e.cdevice.which)) return;
            for (auto& p : g_pads)
                if (!p) { p = SDL_GameControllerOpen(e.cdevice.which); break; }
        }
    });
    std::lock_guard<std::mutex> l(g_lock);
    for (int i = 0, slot = 0; i < SDL_NumJoysticks() && slot < XUSER_MAX_COUNT; ++i)
        if (SDL_IsGameController(i)) g_pads[slot++] = SDL_GameControllerOpen(i);
}

SHORT axis(SDL_GameController* p, SDL_GameControllerAxis a, bool invert) {
    const int v = SDL_GameControllerGetAxis(p, a);
    return static_cast<SHORT>(invert ? std::clamp(-v - 1, -32768, 32767) : v);
}

// Testing without a controller: FABLE_TEST_PAD names a file holding
// "<buttons hex> <lx> <ly> <rx> <ry> <lt> <rt>" (sticks -1..1, triggers 0..1); it acts as the
// on-screen pad does.
void testPad() {
    static const char* path = std::getenv("FABLE_TEST_PAD");
    if (!path) return;
    if (FILE* f = std::fopen(path, "r")) {
        unsigned b = 0;
        float v[6] = {};
        const int n = std::fscanf(f, "%x %f %f %f %f %f %f", &b, &v[0], &v[1], &v[2], &v[3], &v[4], &v[5]);
        std::fclose(f);
        XINPUT_GAMEPAD g{};
        auto ax = [](float x) { return static_cast<SHORT>(std::clamp(x, -1.0f, 1.0f) * 32767); };
        g.wButtons = static_cast<WORD>(b);
        g.sThumbLX = ax(v[0]), g.sThumbLY = ax(v[1]), g.sThumbRX = ax(v[2]), g.sThumbRY = ax(v[3]);
        g.bLeftTrigger = static_cast<BYTE>(std::clamp(v[4], 0.0f, 1.0f) * 255);
        g.bRightTrigger = static_cast<BYTE>(std::clamp(v[5], 0.0f, 1.0f) * 255);
        setVirtualPad(n >= 1, g);
    }
}

DWORD WINAPI getState(DWORD user, XINPUT_STATE* s) {
    ensure();
    testPad();
    pumpSdlEvents();
    SDL_GameControllerUpdate();
    std::lock_guard<std::mutex> l(g_lock);
    SDL_GameController* p = user < XUSER_MAX_COUNT ? g_pads[user] : nullptr;
    XINPUT_GAMEPAD& g = s->Gamepad;
    std::memset(&g, 0, sizeof g);
    bool virt;
    {
        std::lock_guard<std::mutex> vl(g_virtLock);
        virt = user == 0 && g_virtActive;
        if (virt) g = g_virt;
    }
    if (!p && !virt) return ERROR_DEVICE_NOT_CONNECTED;
    if (!p) {
        s->dwPacketNumber = ++g_packet[user];
        return ERROR_SUCCESS;
    }
    const XINPUT_GAMEPAD touch = g;
    const struct { SDL_GameControllerButton b; WORD x; } map[] = {
        {SDL_CONTROLLER_BUTTON_DPAD_UP, XINPUT_GAMEPAD_DPAD_UP}, {SDL_CONTROLLER_BUTTON_DPAD_DOWN, XINPUT_GAMEPAD_DPAD_DOWN},
        {SDL_CONTROLLER_BUTTON_DPAD_LEFT, XINPUT_GAMEPAD_DPAD_LEFT}, {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, XINPUT_GAMEPAD_DPAD_RIGHT},
        {SDL_CONTROLLER_BUTTON_START, XINPUT_GAMEPAD_START}, {SDL_CONTROLLER_BUTTON_BACK, XINPUT_GAMEPAD_BACK},
        {SDL_CONTROLLER_BUTTON_LEFTSTICK, XINPUT_GAMEPAD_LEFT_THUMB}, {SDL_CONTROLLER_BUTTON_RIGHTSTICK, XINPUT_GAMEPAD_RIGHT_THUMB},
        {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, XINPUT_GAMEPAD_LEFT_SHOULDER}, {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, XINPUT_GAMEPAD_RIGHT_SHOULDER},
        {SDL_CONTROLLER_BUTTON_A, XINPUT_GAMEPAD_A}, {SDL_CONTROLLER_BUTTON_B, XINPUT_GAMEPAD_B},
        {SDL_CONTROLLER_BUTTON_X, XINPUT_GAMEPAD_X}, {SDL_CONTROLLER_BUTTON_Y, XINPUT_GAMEPAD_Y},
    };
    for (auto& m : map)
        if (SDL_GameControllerGetButton(p, m.b)) g.wButtons |= m.x;
    g.bLeftTrigger = static_cast<BYTE>(SDL_GameControllerGetAxis(p, SDL_CONTROLLER_AXIS_TRIGGERLEFT) >> 7);
    g.bRightTrigger = static_cast<BYTE>(SDL_GameControllerGetAxis(p, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) >> 7);
    g.sThumbLX = axis(p, SDL_CONTROLLER_AXIS_LEFTX, false);
    g.sThumbLY = axis(p, SDL_CONTROLLER_AXIS_LEFTY, true);  // XInput: up is positive
    g.sThumbRX = axis(p, SDL_CONTROLLER_AXIS_RIGHTX, false);
    g.sThumbRY = axis(p, SDL_CONTROLLER_AXIS_RIGHTY, true);
    if (virt) {  // touch and a physical pad together: buttons add up, the stronger stick wins
        g.wButtons |= touch.wButtons;
        g.bLeftTrigger = std::max(g.bLeftTrigger, touch.bLeftTrigger);
        g.bRightTrigger = std::max(g.bRightTrigger, touch.bRightTrigger);
        auto pick = [](SHORT& x, SHORT& y, SHORT tx, SHORT ty) {
            if (int(tx) * tx + int(ty) * ty > int(x) * x + int(y) * y) x = tx, y = ty;
        };
        pick(g.sThumbLX, g.sThumbLY, touch.sThumbLX, touch.sThumbLY);
        pick(g.sThumbRX, g.sThumbRY, touch.sThumbRX, touch.sThumbRY);
    }
    s->dwPacketNumber = ++g_packet[user];
    return ERROR_SUCCESS;
}
DWORD WINAPI setState(DWORD user, XINPUT_VIBRATION* v) {
    ensure();
    std::lock_guard<std::mutex> l(g_lock);
    SDL_GameController* p = user < XUSER_MAX_COUNT ? g_pads[user] : nullptr;
    if (!p) return ERROR_DEVICE_NOT_CONNECTED;
    SDL_GameControllerRumble(p, v->wLeftMotorSpeed, v->wRightMotorSpeed, 200);
    return ERROR_SUCCESS;
}

struct Registrar {
    Registrar() {
        for (const char* dll : {"xinput1_4.dll", "xinput9_1_0.dll", "xinput1_3.dll"}) {
            registerHostModule(dll, "XInputGetState", reinterpret_cast<void*>(&getState));
            registerHostModule(dll, "XInputSetState", reinterpret_cast<void*>(&setState));
        }
    }
} g_registrar;
}  // namespace
}  // namespace w32
