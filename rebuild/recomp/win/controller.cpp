// Game controller support, two ways:
//
// Native (the default): the game's own Xbox-pad input, dormant in the PC build. The PC
// engine still has the Xbox pad input types (analog left/right stick, the 16 pad buttons)
// and ships the original FABLE_XBOX_CONTROL_SCHEME bindings in game.bin; only the device
// side (CJoystickDX, a DirectInput joystick with 12 buttons and no d-pad) and the
// scheme selection were PC-specific. Two lifter hooks bring it back:
//   - 0xAB6E40 CJoystickDX::UpdateEvents -> host_joystick_update: reads XInput and feeds
//     button and stick events to the game exactly as the DirectInput code did, for all
//     16 buttons (d-pad included);
//   - 0x4088E0 GetPrimaryInputVector -> host_primary_inputs: when SetControlScheme builds
//     the active controls, the Xbox scheme's pad bindings are added to the profile's
//     keyboard/mouse bindings, so pad, keyboard and mouse all work at once.
//
// Legacy (NATIVE_PAD = 0): the controller is translated into the PC key and mouse input:
// buttons press the keys they are mapped to, the left stick drives the movement keys and
// the right stick moves the mouse. The events are added to what the real devices report
// (dinput.cpp).
//
// Settings live in <game>\FableRecomp_controller.ini (written with the defaults on first run).
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
    "; Fable: The Lost Chapters (recompiled) - controller settings\n"
    ";\n"
    "; NATIVE_PAD = 1 uses the game's own Xbox controls: analog movement and camera and the\n"
    "; original Xbox button layout. Xbox 360/One pads: LB = White button, RB = Black button.\n"
    "NATIVE_PAD = 1\n"
    "SWAP_BUMPERS = 0       ; 1: LB = Black, RB = White\n"
    "LEFT_DEADZONE = 0.24\n"
    "RIGHT_DEADZONE = 0.24\n"
    ";\n"
    "; With NATIVE_PAD = 0 the pad is translated into keyboard and mouse input instead.\n"
    "; Each line below binds an Xbox/XInput control to one of the game's keys (as set in\n"
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
    bool native = true, swapBumpers = false;
    double leftDeadzone = 0.24, rightDeadzone = 0.24;
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
        else if (key == "NATIVE_PAD") cfg.native = std::atoi(val.c_str()) != 0;
        else if (key == "SWAP_BUMPERS") cfg.swapBumpers = std::atoi(val.c_str()) != 0;
        else if (key == "LEFT_DEADZONE") cfg.leftDeadzone = std::atof(val.c_str());
        else if (key == "RIGHT_DEADZONE") cfg.rightDeadzone = std::atof(val.c_str());
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
    log("controller: XInput %s, %s, settings %s", g.getState ? "available" : "not found",
        g.cfg.native ? "native Xbox controls" : "keyboard/mouse translation", narrow(path.c_str()).c_str());
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

// The connected pad's state (the first XInput slot with a controller, rechecked once a second).
bool readPad(XINPUT_STATE& st) {
    if (!g.init) load();
    if (!g.getState) return false;
    bool ok = g.pad >= 0 && g.getState(static_cast<DWORD>(g.pad), &st) == ERROR_SUCCESS;
    if (!ok && GetTickCount() - g.lastConnectCheck > 1000) {
        g.lastConnectCheck = GetTickCount();
        for (DWORD i = 0; i < 4 && !ok; ++i)
            if (g.getState(i, &st) == ERROR_SUCCESS) {
                ok = true;
                if (g.pad != static_cast<int>(i)) log("controller: using XInput pad %lu", i);
                g.pad = static_cast<int>(i);
            }
    }
    return ok;
}

void poll() {
    if (!g.init) load();
    if (!g.getState || g.cfg.native) return;
    LARGE_INTEGER now, f;
    QueryPerformanceCounter(&now);
    QueryPerformanceFrequency(&f);
    const double dt = std::min(0.1, double(now.QuadPart - g.last.QuadPart) / double(f.QuadPart));
    if (dt < 0.002) return;
    g.last = now;

    XINPUT_STATE st{};
    const bool ok = readPad(st);
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
void touchMotion(int dx, int dy) {
    std::lock_guard<std::mutex> l(g.m);
    if (dx) event(g.mouseEvents, DIMOFS_X, static_cast<DWORD>(dx));
    if (dy) event(g.mouseEvents, DIMOFS_Y, static_cast<DWORD>(dy));
    g.stateDx += dx, g.stateDy += dy;
}
void touchButton(int button, bool down) {
    std::lock_guard<std::mutex> l(g.m);
    Target t;
    t.kind = Target::MouseButton;
    t.code = button & 7;
    press(t, down);
}
void flush() {
    std::lock_guard<std::mutex> l(g.m);
    g.kbEvents.clear();
    g.mouseEvents.clear();
}

}  // namespace host::pad

// ============================================================================
// Native pad: the game's own Xbox controls
// ============================================================================
namespace host::pad {
namespace {

// Retail addresses (Steam Fable.exe).
constexpr uint32_t kClear = 0x9E41D0;                     // CJoystick::Clear (event store)
constexpr uint32_t kAddEventToStore = 0x9E41E0;           // CJoystick::AddEventToStore(const CInputEvent*)
constexpr uint32_t kProcessMaintainedEvents = 0x9E4470;   // tracks held buttons (press/release)
constexpr uint32_t kUpdateMaintainedPositions = 0x9E42E0; // held-button events follow the sticks
constexpr uint32_t kAddMaintainedEvents = 0x9E4360;       // ... and are re-sent every frame
constexpr uint32_t kStickEventTime = 0x122ED70;           // double the DirectInput code stamps stick events with
constexpr uint32_t kCallerSetControlScheme = 0x447060;    // return address of SetControlScheme's GetPrimaryInputVector call
constexpr uint32_t kResetAssignedInputs = 0x4085F0;       // CUserProfileManager::ResetAssignedInputs
// def lookup by name, as ResetAssignedInputs does it
constexpr uint32_t kDefManagerA = 0x13B86A0, kDefManagerB = 0x13B8760;
constexpr uint32_t kGetDefManagerA = 0x44C6B0, kGetDefManagerB = 0x43368D, kLookupDef = 0x410820;
constexpr uint32_t kWideStringCtor = 0x99EBF0, kWideStringDtor = 0x99EAE0;
constexpr uint32_t kRecordSize = 28;  // CActionInputControl

// CJoystick member offsets
constexpr uint32_t kAxes = 0xD18;  // float X, Y (left stick), X2, Y2 (right stick); -1..1, Y down

// CInputEvent (0x34 bytes)
enum : uint32_t { EV_BUTTON_PRESSED = 0x13, EV_BUTTON_RELEASED = 0x15, EV_LEFT_STICK = 0x11, EV_RIGHT_STICK = 0x12 };

// EXboxControllerButton (FableWin.pdb)
enum : uint8_t {
    XB_X = 1, XB_Y = 2, XB_BLACK = 3, XB_A = 4, XB_B = 5, XB_WHITE = 6, XB_LT = 7, XB_RT = 8, XB_LS = 9, XB_RS = 10,
    XB_START = 11, XB_BACK = 12, XB_DUP = 13, XB_DDOWN = 14, XB_DLEFT = 15, XB_DRIGHT = 16, XB_COUNT = 17
};

bool g_held[XB_COUNT];
uint32_t rd32le(const uint8_t* p) { uint32_t v; std::memcpy(&v, p, 4); return v; }
std::vector<uint8_t> g_xboxRecords;  // FABLE_XBOX_CONTROL_SCHEME's CActionInputControl records
bool g_xboxLoaded = false;
uint32_t g_merged = 0, g_mergedCap = 0;  // guest vector {begin, end, capacity} + records

// Radial dead zone, rescaled so the stick still reaches 1 at full deflection.
void deadzone(float& x, float& y, double dz) {
    const double m = std::sqrt(double(x) * x + double(y) * y);
    if (m <= dz || dz >= 1) {
        x = y = 0;
        return;
    }
    const double k = std::min(1.0, (m - dz) / (1 - dz)) / m;
    x = static_cast<float>(x * k);
    y = static_cast<float>(y * k);
}

void writeEvent(uint32_t ev, uint32_t type, float x, float y, uint8_t button, float time) {
    std::memset(gp(ev), 0, 0x34);
    wrf32(ev, x);
    wrf32(ev + 4, y);
    wr8(ev + 8, button);
    wr32(ev + 0x20, 1);  // device class: joystick
    wr32(ev + 0x28, type);
    wrf32(ev + 0x2C, time);
    wrf32(ev + 0x30, time);
}

void loadXboxScheme() {
    g_xboxLoaded = true;
    static const char kName[] = "FABLE_XBOX_CONTROL_SCHEME";
    const uint32_t mem = gcalloc(1, 64 + sizeof kName);
    const uint32_t str = mem, out = mem + 16, name = mem + 32;
    std::memcpy(gp(name), kName, sizeof kName);
    guestCallThis(kWideStringCtor, str, {name, 0xFFFFFFFFu});
    uint32_t mgr = 0;
    if (rd32(kDefManagerA)) mgr = guestCall(kGetDefManagerA, {});
    else if (rd32(kDefManagerB)) mgr = guestCall(kGetDefManagerB, {});
    if (mgr) guestCallThis(kLookupDef, mgr, {str, out});
    guestCallThis(kWideStringDtor, str, {});
    const uint32_t def = rd32(out);
    if (!def) {
        log("controller: FABLE_XBOX_CONTROL_SCHEME not found; pad buttons are not bound");
        return;
    }
    const uint32_t begin = rd32(def + 0x3C), end = rd32(def + 0x40);
    if (end > begin && (end - begin) % kRecordSize == 0 && end - begin < 0x10000)
        g_xboxRecords.assign(gp<uint8_t>(begin), gp<uint8_t>(end));
    log("controller: %u pad bindings from FABLE_XBOX_CONTROL_SCHEME", static_cast<unsigned>(g_xboxRecords.size() / kRecordSize));
    // The reference the lookup took is kept: the def stays loaded for the whole session.
}

// The profile's bindings plus the Xbox scheme's pad bindings (ControllerType 1), as a
// guest std::vector the game reads begin/end from.
uint32_t mergedBindings(uint32_t vec) {
    if (!g_xboxLoaded) loadXboxScheme();
    const uint32_t begin = rd32(vec), end = rd32(vec + 4);
    if (g_xboxRecords.empty() || end < begin) return vec;
    std::vector<uint8_t> all(gp<uint8_t>(begin), gp<uint8_t>(end));
    // Profiles that already carry pad bindings keep their own.
    bool hasPad = false;
    for (size_t i = 0; i + kRecordSize <= all.size(); i += kRecordSize)
        if (rd32le(&all[i + 4]) == 1) hasPad = true;
    if (!hasPad) {
        // The Xbox scheme opens the Xbox pause menu with Start (actions 3 and 5); that screen
        // is not laid out for the PC build. Start opens the PC in-game menu instead (action
        // 72, Return/Escape on the keyboard).
        for (size_t i = 0; i + kRecordSize <= g_xboxRecords.size(); i += kRecordSize) {
            const uint32_t action = rd32le(&g_xboxRecords[i]);
            if ((action == 3 || action == 5) && !std::getenv("FABLE_XBOX_MENUS")) continue;
            all.insert(all.end(), g_xboxRecords.begin() + i, g_xboxRecords.begin() + i + kRecordSize);
        }
        auto add = [&](uint32_t action, uint32_t button) {
            uint8_t rec[kRecordSize] = {};
            const uint32_t fields[] = {action, 1, 0, button, 0};
            std::memcpy(rec, fields, sizeof fields);
            all.insert(all.end(), rec, rec + kRecordSize);
        };
        if (!std::getenv("FABLE_XBOX_MENUS")) add(72, XB_START);
    }
    const uint32_t need = static_cast<uint32_t>(all.size());
    if (need + 12 > g_mergedCap) {
        g_mergedCap = need + 12 + 64 * kRecordSize;
        g_merged = gcalloc(1, g_mergedCap);  // the previous block stays valid for anyone still reading it
    }
    std::memcpy(gp(g_merged + 12), all.data(), need);
    wr32(g_merged, g_merged + 12);
    wr32(g_merged + 4, g_merged + 12 + need);
    wr32(g_merged + 8, g_merged + 12 + need);
    return g_merged;
}

}  // namespace

bool nativeEnabled() {
    std::lock_guard<std::mutex> l(g.m);
    if (!g.init) load();
    return g.cfg.native;
}

}  // namespace host::pad

using host::guestCallThis;
using host::retCdecl;

// 0x4088E0: CActionInputControlVector* __thiscall CUserProfileManager::GetPrimaryInputVector()
extern "C" void host_primary_inputs(Ctx* c) {
    const uint32_t self = c->ecx, ret = rd32(c->esp);
    HLOG(1, "controller: GetPrimaryInputVector from 0x%08X", ret);
    if (rd32(self + 0x54) == rd32(self + 0x58)) guestCallThis(host::pad::kResetAssignedInputs, self, {});
    uint32_t vec = self + 0x54;
    if (ret == host::pad::kCallerSetControlScheme && host::pad::nativeEnabled()) vec = host::pad::mergedBindings(vec);
    retCdecl(c, vec);
}

// 0xAB6E40: bool __thiscall CJoystickDX::UpdateEvents()
extern "C" void host_joystick_update(Ctx* c) {
    using namespace host::pad;
    const uint32_t self = c->ecx, savedEsp = c->esp;
    static bool once;
    if (!once) once = true, host::log("controller: joystick device 0x%08X polled by the game", self);
    c->esp = (c->esp - 0x80) & ~0xFu;  // scratch below the guest stack for the event
    const uint32_t ev = c->esp + 0x20;
    guestCallThis(kClear, self, {});

    XINPUT_STATE st{};
    bool ok = false, native = false;
    double ldz = 0.24, rdz = 0.24;
    bool swap = false;
    {
        std::lock_guard<std::mutex> l(g.m);
        if (!g.init) load();
        native = g.cfg.native;
        ldz = g.cfg.leftDeadzone, rdz = g.cfg.rightDeadzone, swap = g.cfg.swapBumpers;
        if (native) ok = readPad(st);
    }
    if (native) {
        const XINPUT_GAMEPAD& p = st.Gamepad;
        float lx = 0, ly = 0, rx = 0, ry = 0;
        if (ok) {
            lx = p.sThumbLX / 32767.0f, ly = -p.sThumbLY / 32767.0f;  // the game's Y points down (DirectInput)
            rx = p.sThumbRX / 32767.0f, ry = -p.sThumbRY / 32767.0f;
            deadzone(lx, ly, ldz);
            deadzone(rx, ry, rdz);
        }
        wrf32(self + kAxes, lx);
        wrf32(self + kAxes + 4, ly);
        wrf32(self + kAxes + 8, rx);
        wrf32(self + kAxes + 12, ry);

        bool now[XB_COUNT] = {};
        if (ok) {
            const WORD b = p.wButtons;
            now[XB_A] = b & XINPUT_GAMEPAD_A, now[XB_B] = b & XINPUT_GAMEPAD_B;
            now[XB_X] = b & XINPUT_GAMEPAD_X, now[XB_Y] = b & XINPUT_GAMEPAD_Y;
            now[swap ? XB_BLACK : XB_WHITE] = b & XINPUT_GAMEPAD_LEFT_SHOULDER;
            now[swap ? XB_WHITE : XB_BLACK] = b & XINPUT_GAMEPAD_RIGHT_SHOULDER;
            now[XB_LT] = p.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
            now[XB_RT] = p.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
            now[XB_LS] = b & XINPUT_GAMEPAD_LEFT_THUMB, now[XB_RS] = b & XINPUT_GAMEPAD_RIGHT_THUMB;
            now[XB_START] = b & XINPUT_GAMEPAD_START, now[XB_BACK] = b & XINPUT_GAMEPAD_BACK;
            now[XB_DUP] = b & XINPUT_GAMEPAD_DPAD_UP, now[XB_DDOWN] = b & XINPUT_GAMEPAD_DPAD_DOWN;
            now[XB_DLEFT] = b & XINPUT_GAMEPAD_DPAD_LEFT, now[XB_DRIGHT] = b & XINPUT_GAMEPAD_DPAD_RIGHT;
        }
        const float t = static_cast<float>(GetTickCount());
        for (uint8_t i = 1; i < XB_COUNT; ++i)
            if (now[i] != g_held[i]) {
                g_held[i] = now[i];
                writeEvent(ev, now[i] ? EV_BUTTON_PRESSED : EV_BUTTON_RELEASED, lx, ly, i, t);
                guestCallThis(kAddEventToStore, self, {ev});
                guestCallThis(kProcessMaintainedEvents, self, {ev});
            }
        if (ok) {
            const float stickTime = static_cast<float>(rdf64(kStickEventTime));
            writeEvent(ev, EV_LEFT_STICK, lx, ly, 0, stickTime);
            guestCallThis(kAddEventToStore, self, {ev});
            writeEvent(ev, EV_RIGHT_STICK, rx, ry, 0, stickTime);
            guestCallThis(kAddEventToStore, self, {ev});
        }
        guestCallThis(kUpdateMaintainedPositions, self, {});
        guestCallThis(kAddMaintainedEvents, self, {});
        if (ok) host::menu::poll(ev + 0x34);
    }
    c->esp = savedEsp;
    retCdecl(c, 1);
}
