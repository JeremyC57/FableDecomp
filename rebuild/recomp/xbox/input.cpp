// SDL game controllers and keyboard -> Xbox gamepad state.
//
// Controller: A/B/X/Y as labelled, RB = Black, LB = White, triggers, sticks (clicks = thumb
// buttons), d-pad, Start, Back. Keyboard: WASD left stick, arrows right stick, Space A,
// LShift B, E X, Q Y, R Black, F White, Mouse buttons triggers... (see kKeys), Enter Start,
// Escape Back. FABLE_AUTOPRESS=1 taps A, and Start for the first 50 s (unattended testing).
#include "input.hpp"
#include "settings.hpp"
#include "xhost.hpp"

#include <SDL.h>
#ifdef __ANDROID__
#include <SDL_system.h>
#include <jni.h>
#endif

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>
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

// The phone's own vibrator (Android, touch controls): GameActivity.vibrate(left, right).
void phoneVibrate(uint16_t left, uint16_t right) {
#ifdef __ANDROID__
    JNIEnv* env = static_cast<JNIEnv*>(SDL_AndroidGetJNIEnv());
    jobject activity = static_cast<jobject>(SDL_AndroidGetActivity());
    if (!env || !activity) return;
    jclass cls = env->GetObjectClass(activity);
    if (jmethodID m = env->GetMethodID(cls, "vibrate", "(II)V")) env->CallVoidMethod(activity, m, static_cast<jint>(left), static_cast<jint>(right));
    if (env->ExceptionCheck()) env->ExceptionClear();
    env->DeleteLocalRef(cls);
    env->DeleteLocalRef(activity);
#else
    (void)left;
    (void)right;
#endif
}

// Pads without SDL rumble support (Bluetooth pads on Android go through SDL's ANDROID joystick
// backend, whose rumble is unsupported) vibrate through the haptic API instead.
SDL_Haptic* g_haptic[4];
std::atomic<bool> g_background{false};  // app in the background: motors off

void openHaptic(int p) {
    if (!g_ctrl[p] || SDL_GameControllerHasRumble(g_ctrl[p])) return;
    SDL_Haptic* h = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(g_ctrl[p]));
    if (h && SDL_HapticRumbleInit(h) == 0) {
        g_haptic[p] = h;
        XLOG(1, "input: port %d rumbles through the haptic API", p + 1);
    } else if (h) {
        SDL_HapticClose(h);
    }
}

}  // namespace

void applyRumble();

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
                openHaptic(p);
                break;
            }
    } else if (e.type == SDL_APP_WILLENTERBACKGROUND || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_LOST)) {
        g_background = true;
    } else if (e.type == SDL_APP_DIDENTERFOREGROUND || (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)) {
        g_background = false;
    } else if (e.type == SDL_CONTROLLERDEVICEREMOVED) {
        for (int p = 0; p < 4; ++p)
            if (g_ctrl[p] && SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(g_ctrl[p])) == e.cdevice.which) {
                if (g_haptic[p]) SDL_HapticClose(g_haptic[p]);
                g_haptic[p] = nullptr;
                SDL_GameControllerClose(g_ctrl[p]);
                g_ctrl[p] = nullptr;
            }
    }
}

namespace {
std::string g_recordPath;
FILE* g_record = nullptr;
size_t g_recordBytes = 0;
const auto g_recordStart = std::chrono::steady_clock::now();

void record(const Pad& d) {
    if (g_recordPath.empty() || g_recordBytes > (64u << 20)) return;
    if (!g_record && !(g_record = std::fopen(g_recordPath.c_str(), "w"))) {
        g_recordPath.clear();
        return;
    }
    const long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - g_recordStart).count();
    const int n = std::fprintf(g_record, "%llu %lld %u %u %u %u %u %u %u %u %u %d %d %d %d\n", static_cast<unsigned long long>(gpu::g_frameCount), ms,
                               d.buttons, d.analog[0], d.analog[1], d.analog[2], d.analog[3], d.analog[4], d.analog[5], d.analog[6], d.analog[7], d.lx,
                               d.ly, d.rx, d.ry);
    std::fflush(g_record);  // the app can be closed at any time
    if (n > 0) g_recordBytes += static_cast<size_t>(n);
}
}  // namespace

void setRecordFile(const char* path) { g_recordPath = path ? path : ""; }

void update() {
    applyRumble();
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
    // FABLE_INPUT_REPLAY=<recording>,<ms>,<flip> (testing): port 1 plays a FableXbox_input.txt
    // recording from its time <ms> on, in real time, starting when the run reaches <flip>.
    static const auto replay = [] {
        struct Entry { long long ms; Pad pad; };
        std::vector<Entry> v;
        long long fromMs = 0;
        uint64_t atFlip = 0;
        if (const char* e = getenv("FABLE_INPUT_REPLAY")) {
            std::string path(e);
            const size_t c1 = path.find(','), c2 = c1 == std::string::npos ? c1 : path.find(',', c1 + 1);
            if (c2 != std::string::npos) {
                fromMs = std::atoll(path.c_str() + c1 + 1);
                atFlip = std::strtoull(path.c_str() + c2 + 1, nullptr, 10);
                path.resize(c1);
            }
            if (FILE* f = std::fopen(path.c_str(), "r")) {
                unsigned long long flip;
                Entry x{};
                unsigned b, a[8];
                int lx, ly, rx, ry;
                while (std::fscanf(f, "%llu %lld %u %u %u %u %u %u %u %u %u %d %d %d %d", &flip, &x.ms, &b, &a[0], &a[1], &a[2], &a[3], &a[4], &a[5],
                                   &a[6], &a[7], &lx, &ly, &rx, &ry) == 15) {
                    x.pad.buttons = static_cast<uint16_t>(b);
                    for (int i = 0; i < 8; ++i) x.pad.analog[i] = static_cast<uint8_t>(a[i]);
                    x.pad.lx = static_cast<int16_t>(lx), x.pad.ly = static_cast<int16_t>(ly), x.pad.rx = static_cast<int16_t>(rx), x.pad.ry = static_cast<int16_t>(ry);
                    v.push_back(x);
                }
                std::fclose(f);
            }
            XLOG(0, "input: replaying %zu recorded changes from %lld ms at flip %llu", v.size(), fromMs, static_cast<unsigned long long>(atFlip));
        }
        return std::make_tuple(v, fromMs, atFlip);
    }();
    if (!std::get<0>(replay).empty() && gpu::g_frameCount >= std::get<2>(replay)) {
        static const auto t0 = std::chrono::steady_clock::now();
        const long long now = std::get<1>(replay) + std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - t0).count();
        const auto& v = std::get<0>(replay);
        static size_t i = 0;
        while (i + 1 < v.size() && v[i + 1].ms <= now) ++i;
        if (v[i].ms <= now) {
            const uint32_t packet = next[0].packet;
            next[0] = v[i].pad;
            next[0].packet = packet;
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
        if (changed && p == 0) record(next[p]);
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

// XInputSetState sets motor speeds that hold until the next call; SDL rumble has a duration,
// so the video thread (applyRumble, from update) renews it while the motors run.
std::atomic<uint32_t> g_rumble[4];  // left << 16 | right

void rumble(int port, uint16_t left, uint16_t right) {
    if (port >= 0 && port < 4) g_rumble[port] = static_cast<uint32_t>(left) << 16 | right;
}

void applyRumble() {
    static uint32_t applied[4];
    static uint32_t renewedAt[4];
    const uint32_t now = SDL_GetTicks();
    for (int p = 0; p < 4; ++p) {
        const uint32_t v = settings().vibration && !g_background ? g_rumble[p].load() : 0;
        if (v == applied[p] && (v == 0 || now - renewedAt[p] < 500)) continue;
        applied[p] = v;
        renewedAt[p] = now;
        const uint16_t left = static_cast<uint16_t>(v >> 16), right = static_cast<uint16_t>(v);
        const bool padRumble = g_ctrl[p] && SDL_GameControllerHasRumble(g_ctrl[p]);
        if (padRumble) {
            SDL_GameControllerRumble(g_ctrl[p], left, right, v ? 1000 : 0);
        } else if (g_haptic[p]) {
            if (v) SDL_HapticRumblePlay(g_haptic[p], std::max(left, right) / 65535.0f, 1000);
            else SDL_HapticRumbleStop(g_haptic[p]);
        } else if (p == 0) {
            phoneVibrate(left, right);  // touch controls, or a pad that cannot rumble
        }
    }
}

} // namespace xb::input
