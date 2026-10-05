// SDL game controllers and keyboard -> Xbox gamepad state.
//
// Controller: A/B/X/Y as labelled, RB = Black, LB = White, triggers, sticks (clicks = thumb
// buttons), d-pad, Start, Back. Keyboard: WASD left stick, arrows right stick, Space A,
// LShift B, E X, Q Y, R Black, F White, Mouse buttons triggers... (see kKeys), Enter Start,
// Escape Back. FABLE_AUTOPRESS=1 taps A and Start in turn (unattended testing).
#include "input.hpp"
#include "xhost.hpp"

#include <SDL.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cstdlib>

namespace xb::input {

namespace {
std::mutex g_lock;
Pad g_pads[4];
SDL_GameController* g_ctrl[4];
uint32_t g_packet = 1;
bool g_autopress = false;

int16_t axis(SDL_GameController* c, SDL_GameControllerAxis a, bool invert) {
    int v = SDL_GameControllerGetAxis(c, a);
    if (invert) v = -v - 1;
    return static_cast<int16_t>(std::clamp(v, -32768, 32767));
}
uint8_t trigger(SDL_GameController* c, SDL_GameControllerAxis a) {
    return static_cast<uint8_t>(std::clamp(SDL_GameControllerGetAxis(c, a) / 128, 0, 255));
}
}  // namespace

void init() {
    SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER | SDL_INIT_HAPTIC);
    g_autopress = getenv("FABLE_AUTOPRESS") != nullptr;
}

void handleEvent(const SDL_Event& e) {
    if (e.type == SDL_CONTROLLERDEVICEADDED) {
        for (int p = 0; p < 4; ++p)
            if (!g_ctrl[p]) {
                g_ctrl[p] = SDL_GameControllerOpen(e.cdevice.which);
                XLOG(1, "input: controller \"%s\" on port %d", SDL_GameControllerName(g_ctrl[p]), p + 1);
                break;
            }
    } else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
        for (int p = 0; p < 4; ++p)
            if (g_ctrl[p] && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(g_ctrl[p])) == e.cdevice.which) {
                SDL_GameControllerClose(g_ctrl[p]);
                g_ctrl[p] = nullptr;
            }
    }
}

void update() {
    Pad next[4];
    for (int p = 0; p < 4; ++p) {
        SDL_GameController* c = g_ctrl[p];
        if (!c) continue;
        Pad& s = next[p];
        auto btn = [&](SDL_GameControllerButton b) { return SDL_GameControllerGetButton(c, b) != 0; };
        if (btn(SDL_CONTROLLER_BUTTON_DPAD_UP)) s.buttons |= 0x01;
        if (btn(SDL_CONTROLLER_BUTTON_DPAD_DOWN)) s.buttons |= 0x02;
        if (btn(SDL_CONTROLLER_BUTTON_DPAD_LEFT)) s.buttons |= 0x04;
        if (btn(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) s.buttons |= 0x08;
        if (btn(SDL_CONTROLLER_BUTTON_START)) s.buttons |= 0x10;
        if (btn(SDL_CONTROLLER_BUTTON_BACK)) s.buttons |= 0x20;
        if (btn(SDL_CONTROLLER_BUTTON_LEFTSTICK)) s.buttons |= 0x40;
        if (btn(SDL_CONTROLLER_BUTTON_RIGHTSTICK)) s.buttons |= 0x80;
        s.analog[0] = btn(SDL_CONTROLLER_BUTTON_A) ? 255 : 0;
        s.analog[1] = btn(SDL_CONTROLLER_BUTTON_B) ? 255 : 0;
        s.analog[2] = btn(SDL_CONTROLLER_BUTTON_X) ? 255 : 0;
        s.analog[3] = btn(SDL_CONTROLLER_BUTTON_Y) ? 255 : 0;
        s.analog[4] = btn(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) ? 255 : 0;  // Black
        s.analog[5] = btn(SDL_CONTROLLER_BUTTON_LEFTSHOULDER) ? 255 : 0;   // White
        s.analog[6] = trigger(c, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
        s.analog[7] = trigger(c, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
        s.lx = axis(c, SDL_CONTROLLER_AXIS_LEFTX, false);
        s.ly = axis(c, SDL_CONTROLLER_AXIS_LEFTY, true);  // Xbox: up is positive
        s.rx = axis(c, SDL_CONTROLLER_AXIS_RIGHTX, false);
        s.ry = axis(c, SDL_CONTROLLER_AXIS_RIGHTY, true);
    }
    // Keyboard drives port 1 (merged with a controller there).
    const Uint8* k = SDL_GetKeyboardState(nullptr);
    if (k) {
        Pad& s = next[0];
        auto stick = [&](SDL_Scancode neg, SDL_Scancode pos) -> int16_t { return k[pos] ? 32767 : k[neg] ? -32768 : 0; };
        if (!s.lx) s.lx = stick(SDL_SCANCODE_A, SDL_SCANCODE_D);
        if (!s.ly) s.ly = stick(SDL_SCANCODE_S, SDL_SCANCODE_W);
        if (!s.rx) s.rx = stick(SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT);
        if (!s.ry) s.ry = stick(SDL_SCANCODE_DOWN, SDL_SCANCODE_UP);
        auto key = [&](SDL_Scancode sc, int analog) { if (k[sc]) s.analog[analog] = 255; };
        key(SDL_SCANCODE_SPACE, 0);
        key(SDL_SCANCODE_LSHIFT, 1);
        key(SDL_SCANCODE_E, 2);
        key(SDL_SCANCODE_Q, 3);
        key(SDL_SCANCODE_R, 4);
        key(SDL_SCANCODE_F, 5);
        key(SDL_SCANCODE_Z, 6);
        key(SDL_SCANCODE_C, 7);
        if (k[SDL_SCANCODE_RETURN]) s.buttons |= 0x10;
        if (k[SDL_SCANCODE_ESCAPE] || k[SDL_SCANCODE_TAB]) s.buttons |= 0x20;
        if (k[SDL_SCANCODE_I]) s.buttons |= 0x01;
        if (k[SDL_SCANCODE_K]) s.buttons |= 0x02;
        if (k[SDL_SCANCODE_J]) s.buttons |= 0x04;
        if (k[SDL_SCANCODE_L]) s.buttons |= 0x08;
    }
    if (g_autopress) {
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        const int phase = static_cast<int>((ms / 250) % 16);
        if (phase == 0) next[0].analog[0] = 255;  // A
        if (phase == 8) next[0].buttons |= 0x10;  // Start
    }
    std::lock_guard<std::mutex> l(g_lock);
    for (int p = 0; p < 4; ++p) {
        if (std::memcmp(&next[p].buttons, &g_pads[p].buttons, sizeof(Pad) - sizeof(uint32_t)) != 0) next[p].packet = ++g_packet;
        else next[p].packet = g_pads[p].packet;
        g_pads[p] = next[p];
    }
}

uint32_t connectedMask() {
    uint32_t m = 1;  // port 1: a controller or the keyboard
    for (int p = 1; p < 4; ++p)
        if (g_ctrl[p]) m |= 1u << p;
    return m;
}

Pad pad(int port) {
    std::lock_guard<std::mutex> l(g_lock);
    return g_pads[port];
}

void rumble(int port, uint16_t left, uint16_t right) {
    if (port >= 0 && port < 4 && g_ctrl[port]) SDL_GameControllerRumble(g_ctrl[port], left, right, 250);
}

} // namespace xb::input
