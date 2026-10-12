// XInput on SDL game controllers: "xinput1_4.dll" (and the older names) as a host module,
// so the host's controller mapping (controller.cpp) works unchanged.
#include <SDL.h>

#include "w32sdl.hpp"
#include "../overlay/overlay.hpp"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <vector>
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
    // Extra mappings (SDL gamecontrollerdb.txt lines): FABLE_PAD_MAPPINGS names a file; the
    // stock handheld pads need one (the launcher ships gamecontrollerdb.txt).
    if (const char* f = std::getenv("FABLE_PAD_MAPPINGS")) {
        const int n = SDL_GameControllerAddMappingsFromFile(f);
        std::fprintf(stderr, "controller: %d mappings from %s\n", n, f);
    }
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        char guid[64];
        SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof guid);
        char* m = SDL_GameControllerMappingForDeviceIndex(i);
        std::fprintf(stderr, "controller: joystick %d \"%s\" guid %s: %s\n", i, SDL_JoystickNameForIndex(i), guid, m ? m : "no game controller mapping");
        SDL_free(m);
    }
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
        if (user == 0) host::overlay::filterPad(g, false);
        s->dwPacketNumber = ++g_packet[user];
        return ERROR_SUCCESS;
    }
    const XINPUT_GAMEPAD touch = g;
    // FABLE_PAD_QUIT=1 (handhelds without a desktop): Back + Start held for two seconds exits
    static const bool quitCombo = std::getenv("FABLE_PAD_QUIT") && std::atoi(std::getenv("FABLE_PAD_QUIT")) != 0;
    if (quitCombo) {
        static Uint32 since = 0;
        if (SDL_GameControllerGetButton(p, SDL_CONTROLLER_BUTTON_BACK) && SDL_GameControllerGetButton(p, SDL_CONTROLLER_BUTTON_START)) {
            if (!since) since = SDL_GetTicks();
            else if (SDL_GetTicks() - since > 2000) { std::fprintf(stderr, "controller: Back+Start held: exiting\n"); std::fflush(nullptr); _exit(0); }
        } else {
            since = 0;
        }
    }
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
    if (user == 0) host::overlay::filterPad(g, SDL_GameControllerGetButton(p, SDL_CONTROLLER_BUTTON_GUIDE) != 0);
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

// --pad-probe: lists the joysticks and prints raw button/axis/hat events for 30 seconds, to
// write a game controller mapping for an unknown pad.
int w32::padProbe() {
    if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER | SDL_INIT_EVENTS) != 0) { std::fprintf(stderr, "pad probe: SDL_Init: %s\n", SDL_GetError()); return 1; }
    if (const char* f = std::getenv("FABLE_PAD_MAPPINGS")) std::fprintf(stderr, "pad probe: %d mappings from %s\n", SDL_GameControllerAddMappingsFromFile(f), f);
    std::fprintf(stderr, "pad probe: %d joysticks\n", SDL_NumJoysticks());
    std::vector<SDL_Joystick*> opened;
    for (int i = 0; i < SDL_NumJoysticks(); ++i) {
        char guid[64];
        SDL_JoystickGetGUIDString(SDL_JoystickGetDeviceGUID(i), guid, sizeof guid);
        SDL_Joystick* j = SDL_JoystickOpen(i);
        if (!j) continue;
        opened.push_back(j);
        char* m = SDL_GameControllerMappingForDeviceIndex(i);
        std::fprintf(stderr, "pad probe: joystick %d \"%s\" guid %s: %d axes, %d buttons, %d hats; path %s; mapping: %s\n", i, SDL_JoystickName(j), guid,
                     SDL_JoystickNumAxes(j), SDL_JoystickNumButtons(j), SDL_JoystickNumHats(j), SDL_JoystickPath(j) ? SDL_JoystickPath(j) : "?", m ? m : "none");
        SDL_free(m);
    }
    std::fprintf(stderr, "pad probe: press each button and move each stick (30 seconds)...\n");
    const Uint32 end = SDL_GetTicks() + 30000;
    std::vector<Sint16> last(64, 0);
    while (SDL_GetTicks() < end) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_JOYBUTTONDOWN || e.type == SDL_JOYBUTTONUP) std::fprintf(stderr, "pad probe: joy %d button %d %s\n", e.jbutton.which, e.jbutton.button, e.type == SDL_JOYBUTTONDOWN ? "down" : "up");
            else if (e.type == SDL_JOYHATMOTION) std::fprintf(stderr, "pad probe: joy %d hat %d value %d\n", e.jhat.which, e.jhat.hat, e.jhat.value);
            else if (e.type == SDL_JOYAXISMOTION) {
                const int a = e.jaxis.axis;
                if (a < 64 && std::abs(e.jaxis.value - last[a]) > 8000) { last[a] = e.jaxis.value; std::fprintf(stderr, "pad probe: joy %d axis %d value %d\n", e.jaxis.which, a, e.jaxis.value); }
            } else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) std::fprintf(stderr, "pad probe: key %s %s\n", SDL_GetKeyName(e.key.keysym.sym), e.type == SDL_KEYDOWN ? "down" : "up");
        }
        SDL_Delay(5);
    }
    for (SDL_Joystick* j : opened) SDL_JoystickClose(j);
    std::fprintf(stderr, "pad probe: done\n");
    return 0;
}
