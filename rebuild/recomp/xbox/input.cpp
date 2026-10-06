// SDL game controllers and keyboard -> Xbox gamepad state.
//
// Controller: A/B/X/Y as labelled, RB = Black, LB = White, triggers, sticks (clicks = thumb
// buttons), d-pad, Start, Back. Keyboard: WASD left stick, arrows right stick, Space A,
// LShift B, E X, Q Y, R Black, F White, Mouse buttons triggers... (see kKeys), Enter Start,
// Escape Back. FABLE_AUTOPRESS=1 taps A, and Start for the first 50 s (unattended testing).
#include "input.hpp"
#include "xhost.hpp"

#include <SDL.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <mutex>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace xb::gpu { extern uint64_t g_frameCount; }
namespace xb::input {

namespace {
std::mutex g_lock;
Pad g_pads[4];
SDL_GameController* g_ctrl[4];
uint32_t g_packet = 1;
bool g_autopress = false;
std::mutex g_virtLock;
bool g_virtActive = false;
uint32_t g_virtButtons = 0;
float g_virt[6] = {};
float g_lookX = 0, g_lookY = 0;

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
    {  // on-screen controls (Android)
        std::lock_guard<std::mutex> l(g_virtLock);
        Pad& s = next[0];
        if (g_virtActive) {
            const uint32_t b = g_virtButtons;
            s.buttons |= static_cast<uint16_t>(b & 0xFF);  // d-pad, Start, Back, thumbs
            auto press = [&](uint32_t bit, int analog) { if (b & bit) s.analog[analog] = 255; };
            press(0x1000, 0);  // A
            press(0x2000, 1);  // B
            press(0x4000, 2);  // X
            press(0x8000, 3);  // Y
            press(0x0200, 4);  // RB -> Black
            press(0x0100, 5);  // LB -> White
            auto stick = [](float v) { return static_cast<int16_t>(std::clamp(v, -1.0f, 1.0f) * 32767.0f); };
            if (!s.lx && !s.ly) { s.lx = stick(g_virt[0]); s.ly = stick(g_virt[1]); }
            if (!s.rx && !s.ry) { s.rx = stick(g_virt[2]); s.ry = stick(g_virt[3]); }
            s.analog[6] = std::max<uint8_t>(s.analog[6], static_cast<uint8_t>(std::clamp(g_virt[4], 0.0f, 1.0f) * 255.0f));
            s.analog[7] = std::max<uint8_t>(s.analog[7], static_cast<uint8_t>(std::clamp(g_virt[5], 0.0f, 1.0f) * 255.0f));
        }
        // Touchpad drags: a right-stick deflection that decays over a few frames.
        if (g_lookX != 0.0f || g_lookY != 0.0f) {
            if (!s.rx) s.rx = static_cast<int16_t>(std::clamp(g_lookX, -1.0f, 1.0f) * 32767.0f);
            if (!s.ry) s.ry = static_cast<int16_t>(std::clamp(-g_lookY, -1.0f, 1.0f) * 32767.0f);
            g_lookX *= 0.6f;
            g_lookY *= 0.6f;
            if (std::fabs(g_lookX) < 0.02f) g_lookX = 0;
            if (std::fabs(g_lookY) < 0.02f) g_lookY = 0;
        }
    }
    // FABLE_INPUT_SCRIPT="200:ly=32767;215:rx=-20000;220:" (seconds since start: stick values or
    // buttons a/b/x/y/start/lt/rt=1, held until the next entry) drives port 1 for unattended testing.
    // FABLE_INPUT_FRAMES=1: the times are flip counts instead (repeatable on slow renderers).
    static const char* script = getenv("FABLE_INPUT_SCRIPT");
    if (script) {
        static const auto start = std::chrono::steady_clock::now();
        static const bool byFrames = getenv("FABLE_INPUT_FRAMES") != nullptr;
        const double t = byFrames ? static_cast<double>(gpu::g_frameCount)
                                  : std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        const char* cur = nullptr;
        for (const char* p = script; *p;) {
            if (std::atof(p) > t) break;
            cur = p;
            p = std::strchr(p, ';');
            if (!p) break;
            ++p;
        }
        if (cur) {
            const char* p = std::strchr(cur, ':');
            const char* end = std::strchr(cur, ';');
            while (p && (!end || p < end)) {
                ++p;
                char key[8] = {};
                int v = 0;
                if (std::sscanf(p, "%7[a-z]=%d", key, &v) == 2) {
                    Pad& d = next[0];
                    if (!std::strcmp(key, "lx")) d.lx = static_cast<int16_t>(v);
                    else if (!std::strcmp(key, "ly")) d.ly = static_cast<int16_t>(v);
                    else if (!std::strcmp(key, "rx")) d.rx = static_cast<int16_t>(v);
                    else if (!std::strcmp(key, "ry")) d.ry = static_cast<int16_t>(v);
                    else if (!std::strcmp(key, "a")) d.analog[0] = v ? 255 : 0;
                    else if (!std::strcmp(key, "b")) d.analog[1] = v ? 255 : 0;
                    else if (!std::strcmp(key, "x")) d.analog[2] = v ? 255 : 0;
                    else if (!std::strcmp(key, "y")) d.analog[3] = v ? 255 : 0;
                    else if (!std::strcmp(key, "start")) d.buttons |= v ? 0x10 : 0;
                    else if (!std::strcmp(key, "lt")) d.analog[6] = v ? 255 : 0;
                    else if (!std::strcmp(key, "rt")) d.analog[7] = v ? 255 : 0;
                }
                p = std::strchr(p, ',');
            }
        }
    }
    static const double autoUntil = getenv("FABLE_AUTOPRESS_UNTIL") ? atof(getenv("FABLE_AUTOPRESS_UNTIL")) : 1e18;  // seconds
    static const auto autoStart = std::chrono::steady_clock::now();
    const double autoClock = getenv("FABLE_INPUT_FRAMES") ? static_cast<double>(gpu::g_frameCount)
                                                          : std::chrono::duration<double>(std::chrono::steady_clock::now() - autoStart).count();
    if (g_autopress && autoClock > autoUntil) g_autopress = false;
    if (g_autopress) {
        const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
        const int phase = static_cast<int>((ms / 250) % 16);
        if (phase == 0) next[0].analog[0] = 255;  // A
        static const auto t0 = ms;
        if (phase == 8 && ms - t0 < 50000) next[0].buttons |= 0x10;  // Start (title and menus only)
    }
    std::lock_guard<std::mutex> l(g_lock);
    for (int p = 0; p < 4; ++p) {
        const Pad &a = next[p], &b = g_pads[p];
        const bool changed = a.buttons != b.buttons || std::memcmp(a.analog, b.analog, sizeof a.analog) != 0 || a.lx != b.lx ||
                             a.ly != b.ly || a.rx != b.rx || a.ry != b.ry;
        if (changed) next[p].packet = ++g_packet;
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

void setVirtualPad(bool active, uint32_t buttons, float lx, float ly, float rx, float ry, float lt, float rt) {
    std::lock_guard<std::mutex> l(g_virtLock);
    g_virtActive = active;
    g_virtButtons = buttons;
    const float v[6] = {lx, ly, rx, ry, lt, rt};
    std::copy(v, v + 6, g_virt);
}

void touchLook(int dx, int dy) {
    std::lock_guard<std::mutex> l(g_virtLock);
    g_lookX = std::clamp(g_lookX + dx / 40.0f, -1.0f, 1.0f);
    g_lookY = std::clamp(g_lookY + dy / 40.0f, -1.0f, 1.0f);
}

void rumble(int port, uint16_t left, uint16_t right) {
    if (port >= 0 && port < 4 && g_ctrl[port]) SDL_GameControllerRumble(g_ctrl[port], left, right, 250);
}

} // namespace xb::input
