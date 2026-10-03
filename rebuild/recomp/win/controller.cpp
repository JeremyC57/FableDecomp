// Game controller support. The PC game only reads a DirectInput keyboard and mouse, so an
// XInput controller is translated into the game's own key and mouse input: buttons
// press the keys they are mapped to, the left stick drives the movement keys and the
// right stick moves the mouse (camera, and the menu cursor). The events are added to
// what the real devices report (dinput.cpp), so keyboard and mouse keep working.
//
// The mapping lives in <game>\FableRecomp_controller.ini (written with the defaults on
// first run). The defaults follow the Xbox version's layout on top of the PC game's
// default key bindings (Redefine Keys, "Reset to WASD").
#include "controller.hpp"

#include <xinput.h>

#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <map>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace host::pad {
namespace {

// A target: a DirectInput key (DIK_*), or a mouse button / wheel step.
struct Target {
    enum Kind { None, Key, MouseButton, WheelUp, WheelDown } kind = None;
    int code = 0;
};

const std::map<std::string, int>& keyNames() {
    static const std::map<std::string, int> m = [] {
        std::map<std::string, int> k = {
            {"TAB", DIK_TAB}, {"SPACE", DIK_SPACE}, {"RETURN", DIK_RETURN}, {"ENTER", DIK_RETURN}, {"ESCAPE", DIK_ESCAPE},
            {"ESC", DIK_ESCAPE}, {"LSHIFT", DIK_LSHIFT}, {"RSHIFT", DIK_RSHIFT}, {"LCTRL", DIK_LCONTROL}, {"RCTRL", DIK_RCONTROL},
            {"LALT", DIK_LMENU}, {"CAPSLOCK", DIK_CAPITAL}, {"CAPS_LOCK", DIK_CAPITAL}, {"BACKSPACE", DIK_BACK},
            {"UP", DIK_UP}, {"DOWN", DIK_DOWN}, {"LEFT", DIK_LEFT}, {"RIGHT", DIK_RIGHT}, {"PRINT", DIK_SYSRQ},
            {"HOME", DIK_HOME}, {"END", DIK_END}, {"PGUP", DIK_PRIOR}, {"PGDN", DIK_NEXT}, {"INSERT", DIK_INSERT},
            {"DELETE", DIK_DELETE}, {"MINUS", DIK_MINUS}, {"EQUALS", DIK_EQUALS},
        };
        const char* letters = "QWERTYUIOP";
        for (int i = 0; letters[i]; ++i) k[std::string(1, letters[i])] = DIK_Q + i;
        letters = "ASDFGHJKL";
        for (int i = 0; letters[i]; ++i) k[std::string(1, letters[i])] = DIK_A + i;
        letters = "ZXCVBNM";
        for (int i = 0; letters[i]; ++i) k[std::string(1, letters[i])] = DIK_Z + i;
        for (int i = 1; i <= 9; ++i) k[std::to_string(i)] = DIK_1 + i - 1;
        k["0"] = DIK_0;
        for (int i = 1; i <= 10; ++i) k["F" + std::to_string(i)] = DIK_F1 + i - 1;
        k["F11"] = DIK_F11;
        k["F12"] = DIK_F12;
        return k;
    }();
    return m;
}

Target parseTarget(std::string s) {
    for (auto& ch : s) ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
    if (s == "LMB") return {Target::MouseButton, 0};
    if (s == "RMB") return {Target::MouseButton, 1};
    if (s == "MMB") return {Target::MouseButton, 2};
    if (s == "WHEELUP") return {Target::WheelUp, 0};
    if (s == "WHEELDOWN") return {Target::WheelDown, 0};
    auto it = keyNames().find(s);
    return it == keyNames().end() ? Target{} : Target{Target::Key, it->second};
}

// Controller inputs that can be bound.
enum Input { A, B, X, Y, LB, RB, LT, RT, BACK, START, LS, RS, DUP, DDOWN, DLEFT, DRIGHT, LUP, LDOWN, LLEFT, LRIGHT, InputCount };
const char* const kInputNames[InputCount] = {"A", "B", "X", "Y", "LB", "RB", "LT", "RT", "BACK", "START", "LS", "RS",
                                             "DPAD_UP", "DPAD_DOWN", "DPAD_LEFT", "DPAD_RIGHT", "LSTICK_UP", "LSTICK_DOWN",
                                             "LSTICK_LEFT", "LSTICK_RIGHT"};

const char* const kDefaultConfig =
    "; Fable: The Lost Chapters (recompiled) - controller mapping\n"
    "; Each line binds an Xbox/XInput control to one of the game's keys (as set in\n"
    "; Options > Redefine Keys). Values: a key name (A-Z, 0-9, F1-F12, TAB, SPACE, RETURN,\n"
    "; ESCAPE, LSHIFT, RSHIFT, LCTRL, CAPSLOCK, UP, DOWN, LEFT, RIGHT, PRINT, ...),\n"
    "; LMB / RMB / MMB (mouse buttons), WHEELUP / WHEELDOWN, or NONE.\n"
    "; The right stick moves the mouse (camera, and the cursor in menus).\n"
    ";\n"
    "; Defaults follow the original Xbox layout on top of the PC default keys:\n"
    "A = TAB            ; interact / talk / pick up\n"
    "X = LMB            ; attack (hold for a charged attack); also clicks in menus\n"
    "B = MMB            ; block\n"
    "Y = RMB            ; flourish / run / first-person targeting\n"
    "LT = SPACE         ; target lock\n"
    "RT = LSHIFT        ; Will (spell mode) - hold RT and press X to cast; also attracts orbs\n"
    "LB = E             ; draw ranged weapon\n"
    "RB = Q             ; draw melee weapon\n"
    "DPAD_UP = 1        ; quick-access items 1-4\n"
    "DPAD_RIGHT = 2\n"
    "DPAD_DOWN = 3\n"
    "DPAD_LEFT = 4\n"
    "START = RETURN     ; in-game menu\n"
    "BACK = F12         ; map\n"
    "LS = CAPSLOCK      ; sneak (click the left stick)\n"
    "RS = R             ; reset camera (click the right stick)\n"
    "LSTICK_UP = W      ; movement\n"
    "LSTICK_DOWN = S\n"
    "LSTICK_LEFT = A\n"
    "LSTICK_RIGHT = D\n"
    "\n"
    "CAMERA_SPEED = 900     ; mouse pixels per second at full right-stick deflection\n"
    "INVERT_Y = 0\n"
    "MOVE_THRESHOLD = 0.35  ; how far the left stick must be pushed to move\n";

struct Config {
    Target bind[InputCount];
    double cameraSpeed = 900, moveThreshold = 0.35;
    bool invertY = false;
};

void parseConfig(Config& cfg, const std::string& text) {
    std::istringstream in(text);
    std::string line;
    while (std::getline(in, line)) {
        if (auto p = line.find(';'); p != std::string::npos) line.resize(p);
        const auto eq = line.find('=');
        if (eq == std::string::npos) continue;
        auto trim = [](std::string s) {
            const auto a = s.find_first_not_of(" \t\r"), b = s.find_last_not_of(" \t\r");
            return a == std::string::npos ? std::string() : s.substr(a, b - a + 1);
        };
        std::string key = trim(line.substr(0, eq)), val = trim(line.substr(eq + 1));
        for (auto& ch : key) ch = static_cast<char>(toupper(static_cast<unsigned char>(ch)));
        if (key == "CAMERA_SPEED") cfg.cameraSpeed = std::atof(val.c_str());
        else if (key == "MOVE_THRESHOLD") cfg.moveThreshold = std::atof(val.c_str());
        else if (key == "INVERT_Y") cfg.invertY = std::atoi(val.c_str()) != 0;
        else
            for (int i = 0; i < InputCount; ++i)
                if (key == kInputNames[i]) cfg.bind[i] = parseTarget(val);
    }
}

using GetStateFn = DWORD(WINAPI*)(DWORD, XINPUT_STATE*);

struct State {
    std::mutex m;
    bool init = false;
    GetStateFn getState = nullptr;
    Config cfg;
    int pad = -1;
    LARGE_INTEGER last{};
    bool held[InputCount] = {};
    int keyHeld[256] = {};   // reference counts (several inputs may share a key)
    int mouseHeld[8] = {};
    double fx = 0, fy = 0;   // fractional mouse motion
    std::vector<DIDEVICEOBJECTDATA> kbEvents, mouseEvents;
    LONG stateDx = 0, stateDy = 0, stateDz = 0;
    DWORD sequence = 0x40000000;
    DWORD lastConnectCheck = 0;
};
State g;

void load() {
    g.init = true;
    for (const wchar_t* dll : {L"xinput1_4.dll", L"xinput9_1_0.dll", L"xinput1_3.dll"})
        if (HMODULE h = LoadLibraryW(dll)) {
            g.getState = reinterpret_cast<GetStateFn>(GetProcAddress(h, "XInputGetState"));
            if (g.getState) break;
        }
    const wstring path = g_gameDir + L"FableRecomp_controller.ini";
    std::string text;
    if (FILE* f = _wfopen(path.c_str(), L"rb")) {
        char buf[4096];
        for (size_t n; (n = std::fread(buf, 1, sizeof buf, f)) > 0;) text.append(buf, n);
        std::fclose(f);
    }
    if (text.empty()) {
        text = kDefaultConfig;
        if (FILE* f = _wfopen(path.c_str(), L"wb")) {
            std::fwrite(text.data(), 1, text.size(), f);
            std::fclose(f);
        }
    }
    parseConfig(g.cfg, text);
    QueryPerformanceCounter(&g.last);
    log("controller: XInput %s, mapping %s", g.getState ? "available" : "not found", narrow(path.c_str()).c_str());
}

void event(std::vector<DIDEVICEOBJECTDATA>& q, DWORD ofs, DWORD data) {
    DIDEVICEOBJECTDATA e{};
    e.dwOfs = ofs;
    e.dwData = data;
    e.dwTimeStamp = GetTickCount();
    e.dwSequence = g.sequence++;
    if (q.size() < 512) q.push_back(e);
}

void press(const Target& t, bool down) {
    switch (t.kind) {
    case Target::Key: {
        int& n = g.keyHeld[t.code & 0xFF];
        const bool was = n > 0;
        n += down ? 1 : -1;
        if (n < 0) n = 0;
        if (was != (n > 0)) event(g.kbEvents, static_cast<DWORD>(t.code), n > 0 ? 0x80 : 0);
        break;
    }
    case Target::MouseButton: {
        int& n = g.mouseHeld[t.code & 7];
        const bool was = n > 0;
        n += down ? 1 : -1;
        if (n < 0) n = 0;
        if (was != (n > 0)) event(g.mouseEvents, static_cast<DWORD>(DIMOFS_BUTTON0 + t.code), n > 0 ? 0x80 : 0);
        break;
    }
    case Target::WheelUp:
    case Target::WheelDown:
        if (down) {
            const LONG z = t.kind == Target::WheelUp ? 120 : -120;
            event(g.mouseEvents, DIMOFS_Z, static_cast<DWORD>(z));
            g.stateDz += z;
        }
        break;
    default:
        break;
    }
}

double stick(SHORT v, SHORT dead) {
    if (std::abs(v) <= dead) return 0;
    const double s = (std::abs(static_cast<double>(v)) - dead) / (32767.0 - dead);
    return v < 0 ? -std::min(1.0, s) : std::min(1.0, s);
}

void poll() {
    if (!g.init) load();
    if (!g.getState) return;
    LARGE_INTEGER now, f;
    QueryPerformanceCounter(&now);
    QueryPerformanceFrequency(&f);
    const double dt = std::min(0.1, double(now.QuadPart - g.last.QuadPart) / double(f.QuadPart));
    if (dt < 0.002) return;
    g.last = now;

    XINPUT_STATE st{};
    bool ok = false;
    if (g.pad >= 0) ok = g.getState(static_cast<DWORD>(g.pad), &st) == ERROR_SUCCESS;
    if (!ok && GetTickCount() - g.lastConnectCheck > 1000) {  // look for a controller once a second
        g.lastConnectCheck = GetTickCount();
        for (DWORD i = 0; i < 4 && !ok; ++i)
            if (g.getState(i, &st) == ERROR_SUCCESS) {
                ok = true;
                if (g.pad != static_cast<int>(i)) log("controller: using XInput pad %lu", i);
                g.pad = static_cast<int>(i);
            }
    }
    bool now_[InputCount] = {};
    if (ok) {
        const XINPUT_GAMEPAD& p = st.Gamepad;
        const WORD b = p.wButtons;
        now_[A] = b & XINPUT_GAMEPAD_A, now_[B] = b & XINPUT_GAMEPAD_B, now_[X] = b & XINPUT_GAMEPAD_X, now_[Y] = b & XINPUT_GAMEPAD_Y;
        now_[LB] = b & XINPUT_GAMEPAD_LEFT_SHOULDER, now_[RB] = b & XINPUT_GAMEPAD_RIGHT_SHOULDER;
        now_[LT] = p.bLeftTrigger > 60, now_[RT] = p.bRightTrigger > 60;
        now_[BACK] = b & XINPUT_GAMEPAD_BACK, now_[START] = b & XINPUT_GAMEPAD_START;
        now_[LS] = b & XINPUT_GAMEPAD_LEFT_THUMB, now_[RS] = b & XINPUT_GAMEPAD_RIGHT_THUMB;
        now_[DUP] = b & XINPUT_GAMEPAD_DPAD_UP, now_[DDOWN] = b & XINPUT_GAMEPAD_DPAD_DOWN;
        now_[DLEFT] = b & XINPUT_GAMEPAD_DPAD_LEFT, now_[DRIGHT] = b & XINPUT_GAMEPAD_DPAD_RIGHT;
        const double lx = stick(p.sThumbLX, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE), ly = stick(p.sThumbLY, XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
        const double th = g.cfg.moveThreshold;
        now_[LUP] = ly > th, now_[LDOWN] = ly < -th, now_[LLEFT] = lx < -th, now_[LRIGHT] = lx > th;
        // right stick -> mouse motion (quadratic response for fine aiming)
        const double rx = stick(p.sThumbRX, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE), ry = stick(p.sThumbRY, XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
        g.fx += rx * std::abs(rx) * g.cfg.cameraSpeed * dt;
        g.fy += (g.cfg.invertY ? ry : -ry) * std::abs(ry) * g.cfg.cameraSpeed * dt;
        const LONG dx = static_cast<LONG>(g.fx), dy = static_cast<LONG>(g.fy);
        g.fx -= dx, g.fy -= dy;
        if (dx) event(g.mouseEvents, DIMOFS_X, static_cast<DWORD>(dx));
        if (dy) event(g.mouseEvents, DIMOFS_Y, static_cast<DWORD>(dy));
        g.stateDx += dx, g.stateDy += dy;
    }
    for (int i = 0; i < InputCount; ++i)
        if (now_[i] != g.held[i]) {
            g.held[i] = now_[i];
            press(g.cfg.bind[i], now_[i]);
        }
}

}  // namespace

void injectKeyboardData(std::vector<DIDEVICEOBJECTDATA>& out, DWORD capacity) {
    std::lock_guard<std::mutex> l(g.m);
    poll();
    size_t n = 0;
    while (n < g.kbEvents.size() && out.size() < capacity) out.push_back(g.kbEvents[n++]);
    g.kbEvents.erase(g.kbEvents.begin(), g.kbEvents.begin() + static_cast<ptrdiff_t>(n));
}
void injectMouseData(std::vector<DIDEVICEOBJECTDATA>& out, DWORD capacity) {
    std::lock_guard<std::mutex> l(g.m);
    poll();
    size_t n = 0;
    while (n < g.mouseEvents.size() && out.size() < capacity) out.push_back(g.mouseEvents[n++]);
    g.mouseEvents.erase(g.mouseEvents.begin(), g.mouseEvents.begin() + static_cast<ptrdiff_t>(n));
}
void injectKeyboardState(uint8_t* keys, DWORD size) {
    std::lock_guard<std::mutex> l(g.m);
    poll();
    for (DWORD i = 0; i < size && i < 256; ++i)
        if (g.keyHeld[i] > 0) keys[i] |= 0x80;
}
void injectMouseState(uint8_t* state, DWORD size) {
    std::lock_guard<std::mutex> l(g.m);
    poll();
    if (size < 16) return;
    LONG v[3];
    std::memcpy(v, state, 12);
    v[0] += g.stateDx, v[1] += g.stateDy, v[2] += g.stateDz;
    std::memcpy(state, v, 12);
    g.stateDx = g.stateDy = g.stateDz = 0;
    for (DWORD i = 0; i < 8 && 12 + i < size; ++i)
        if (g.mouseHeld[i] > 0) state[12 + i] |= 0x80;
}
void flush() {
    std::lock_guard<std::mutex> l(g.m);
    g.kbEvents.clear();
    g.mouseEvents.clear();
}

}  // namespace host::pad
