// See overlay.hpp.
#include "overlay.hpp"

#include "host.hpp"

#include <windows.h>
#include <xinput.h>

#include "../w32/w32.hpp"
#include "gfx_options.hpp"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <algorithm>

namespace host::overlay {
namespace {

std::mutex g_lock;
bool g_open = false;
int g_sel = 0;
bool g_fps = false, g_stats = false, g_shot = false;
WORD g_lastButtons = 0;
bool g_lastGuide = false;
uint32_t g_guideDown = 0;
std::string g_statsText;
double g_fpsValue = 0;
uint32_t g_fpsFrames = 0, g_fpsSince = 0;
bool g_exiting = false;
std::string g_toast;
uint32_t g_toastUntil = 0;
uint32_t g_exitAt = 0;
// What the game must not see (filterPad): buttons, triggers and sticks still held when the menu
// closes stay hidden until released (else B / A / the guide's partner buttons reach the game as
// fresh presses on its next poll), and Back + Start from the moment both are down (the menu's
// combo). Start or Back pressed alone is held back for g_soloMs: if the other joins it is the
// combo and the game sees neither; a tap shorter than that is replayed afterwards.
WORD g_hidden = 0;
bool g_hideLt = false, g_hideRt = false, g_hideLs = false, g_hideRs = false;
WORD g_solo = 0, g_pulse = 0;
uint32_t g_soloSince = 0, g_pulseUntil = 0;
bool g_soloPassed = false;

uint32_t ticks() {
    using namespace std::chrono;
    return static_cast<uint32_t>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

enum Item { RESUME, GRAPHICS, VOLUME, FPS, STATS, LOGLEVEL, SCREENSHOT, EXIT, COUNT };
// the graphics page: the game's own Video Options (resolution, refresh rate, anti-aliasing,
// aspect ratio, the detail levels, brightness, vsync) plus direct levers on the engine
enum GfxItem { G_RES, G_REFRESH, G_AA, G_ASPECT, G_LANDSCAPE, G_MODEL, G_TEXTURE, G_SHADOW, G_EFFECTS, G_BRIGHT, G_VSYNC, G_FPS, G_DRAW, G_GLOW, G_WATER, G_WEATHER, G_DISTORT, G_CHARLOD, G_BACK, G_COUNT };
int g_page = 0;  // 0: main, 1: graphics
std::string onOff(bool b) { return b ? "on" : "off"; }
std::string resolutionText(int scale) {
    int w = 0, h = 0;
    if (!w32::screenSize(w, h) || w <= 0) return scale == 100 ? "native" : std::to_string(scale) + "%";
    const int rw = std::max(320, (w * scale / 100) & ~3), rh = std::max(200, (h * scale / 100) & ~3);
    return std::to_string(rw) + "x" + std::to_string(rh) + (scale == 100 ? " (native)" : " (" + std::to_string(scale) + "%)");
}
std::string gfxLabel(int i) {
    const host::gfx::Settings& s = host::gfx::get();
    using host::gfx::qualityName;
    switch (i) {
        case G_RES: return "Resolution: " + resolutionText(s.resolutionScale) + "  (left/right)";
        case G_REFRESH: return "Refresh rate: " + (s.refreshRate ? std::to_string(s.refreshRate) + " Hz" : std::string("display default")) + "  (left/right)";
        case G_AA: return std::string("Anti-aliasing: ") + (s.antialiasing ? std::to_string(s.antialiasing) + "x MSAA" : "off") + "  (left/right)";
        case G_ASPECT: return std::string("Screen aspect ratio: ") + host::gfx::aspectName(s.aspect) + "  (left/right)";
        case G_LANDSCAPE: return std::string("Landscape detail: ") + qualityName(s.landscape) + "  (left/right)";
        case G_MODEL: return std::string("Mesh detail: ") + qualityName(s.model) + "  (left/right)";
        case G_TEXTURE: return std::string("Texture detail: ") + qualityName(s.texture) + "  (left/right)";
        case G_SHADOW: return std::string("Shadow detail: ") + (s.shadow == 0 ? "Low (off)" : qualityName(s.shadow)) + "  (left/right)";
        case G_EFFECTS: return std::string("Effects detail: ") + qualityName(s.effects) + "  (left/right)";
        case G_BRIGHT: return "Brightness: " + std::to_string(s.brightness) + "  (left/right)";
        case G_VSYNC: return std::string("Vertical sync: ") + (s.vsync == 0 ? "off" : s.vsync == 1 ? "on" : "half (30 fps lock)");
        case G_FPS: return std::string("Frame rate cap: ") + (s.fpsCap ? std::to_string(s.fpsCap) + " fps" : "off") + "  (left/right)";
        case G_DRAW: return "Draw distance: " + std::to_string(s.drawDistance) + "%  (left/right)";
        case G_GLOW: return "Glow / bloom: " + onOff(s.glow);
        case G_WATER: return "Water reflections: " + onOff(s.water);
        case G_WEATHER: return "Weather effects: " + onOff(s.weather);
        case G_DISTORT: return "Heat haze / distortion: " + onOff(s.distortion);
        case G_CHARLOD: {
            static const char* names[] = {"off (always full detail)", "1.0", "1.5", "3.0 (coarsest)"};
            return std::string("Character LOD: ") + names[std::clamp(s.characterLod, 0, 3)] + "  (left/right)";
        }
        case G_BACK: return "Back";
        default: return "";
    }
}
int cycleQ(int q, int dir) { return dir ? std::clamp(q + dir, 0, 3) : (q + 1) % 4; }  // select cycles, left/right step
template <size_t N> int stepList(const int (&list)[N], int cur, int dir) {  // left/right along a list, select cycles
    int k = 0;
    for (size_t j = 0; j < N; ++j) if (list[j] == cur) k = static_cast<int>(j);
    if (!dir) return list[(k + 1) % N];
    return list[std::clamp(k + dir, 0, static_cast<int>(N) - 1)];
}
void gfxChange(int i, int dir) {  // dir: -1 left, +1 right, 0 select
    host::gfx::Settings s = host::gfx::get();
    static const int scales[] = {50, 67, 75, 83, 100}, refreshes[] = {0, 50, 60, 75, 90, 120}, aas[] = {0, 2, 4}, aspects[] = {0, 1, 2, 3, 4, 5}, vsyncs[] = {0, 1, 2};
    switch (i) {
        case G_RES: s.resolutionScale = stepList(scales, s.resolutionScale, dir); break;
        case G_REFRESH: s.refreshRate = stepList(refreshes, s.refreshRate, dir); break;
        case G_AA: s.antialiasing = stepList(aas, s.antialiasing, dir); break;
        case G_ASPECT: s.aspect = stepList(aspects, s.aspect, dir); break;
        case G_LANDSCAPE: s.landscape = cycleQ(s.landscape, dir); break;
        case G_MODEL: s.model = cycleQ(s.model, dir); break;
        case G_TEXTURE: s.texture = cycleQ(s.texture, dir); break;
        case G_SHADOW: s.shadow = cycleQ(s.shadow, dir); break;
        case G_EFFECTS: s.effects = cycleQ(s.effects, dir); break;
        case G_BRIGHT: s.brightness = std::clamp(s.brightness + (dir ? dir : 1) * 10, 0, 100); break;
        case G_VSYNC: s.vsync = stepList(vsyncs, s.vsync, dir); break;
        case G_FPS: { static const int caps[] = {0, 20, 25, 30, 40, 60}; s.fpsCap = stepList(caps, s.fpsCap, dir); break; }
        case G_DRAW: s.drawDistance = std::clamp(s.drawDistance + (dir ? dir : 1) * 10, 30, 150); break;
        case G_GLOW: s.glow = !s.glow; break;
        case G_WATER: s.water = !s.water; break;
        case G_WEATHER: s.weather = !s.weather; break;
        case G_DISTORT: s.distortion = !s.distortion; break;
        case G_CHARLOD: s.characterLod = cycleQ(s.characterLod, dir); break;
        case G_BACK: g_page = 0; g_sel = GRAPHICS; return;
        default: return;
    }
    host::gfx::set(s);
}

std::string label(int i) {
    switch (i) {
        case RESUME: return "Resume game";
        case GRAPHICS: return "Graphics settings...";
        case VOLUME: return "Volume: " + std::to_string(static_cast<int>(w32::masterVolume() * 100 + 0.5f)) + "%  (left/right)";
        case FPS: return std::string("Show FPS: ") + (g_fps ? "on" : "off");
        case STATS: return std::string("Renderer statistics: ") + (g_stats ? "on" : "off");
        case LOGLEVEL: return "Log level: " + std::to_string(host::g_logLevel) + (host::g_logLevel == 0 ? " (errors)" : host::g_logLevel == 1 ? " (info)" : " (every call, slow)");
        case SCREENSHOT: return "Save a screenshot (Fable/save/)";
        case EXIT: return g_exiting ? "Exiting..." : "Exit the game";
        default: return "";
    }
}

void activate(int i) {
    switch (i) {
        case RESUME: g_open = false; break;
        case GRAPHICS: g_page = 1; g_sel = 0; break;
        case FPS: g_fps = !g_fps; break;
        case STATS: g_stats = !g_stats; break;
        case LOGLEVEL: host::g_logLevel = (host::g_logLevel + 1) % 3; break;
        case VOLUME: w32::setMasterVolume(w32::masterVolume() >= 0.99f ? 0.0f : w32::masterVolume() + 0.1f); break;
        case SCREENSHOT: g_shot = true; g_open = false; break;
        case EXIT:
            if (g_exiting) break;
            g_exiting = true;
            g_exitAt = ticks() + 4000;
            // the game's own way out: close its main window (WM_CLOSE -> it saves settings and
            // quits); a hard exit follows if it has not left in four seconds
            if (HWND w = w32::mainHwnd()) PostMessageW(w, WM_CLOSE, 0, 0);
            break;
        default: break;
    }
}

}  // namespace

bool enabled() {
    static const bool on = [] { const char* e = std::getenv("FABLE_OVERLAY"); return e && std::atoi(e) != 0; }();
    return on;
}
bool open() { std::lock_guard<std::mutex> l(g_lock); return g_open; }
bool showFps() { return g_fps; }
bool showStats() { return g_stats; }
int vsync() { return host::gfx::get().vsync; }
void setStats(const char* t) { std::lock_guard<std::mutex> l(g_lock); g_statsText = t; }
bool takeScreenshot() { std::lock_guard<std::mutex> l(g_lock); const bool s = g_shot; g_shot = false; return s; }
void notePresent() {
    const uint32_t now = ticks();
    ++g_fpsFrames;
    if (!g_fpsSince) g_fpsSince = now;
    else if (now - g_fpsSince >= 1000) { g_fpsValue = g_fpsFrames * 1000.0 / (now - g_fpsSince); g_fpsFrames = 0; g_fpsSince = now; }
}
int textScale(int height) { return height > 600 ? 2 : 1; }
void toast(const char* text) { std::lock_guard<std::mutex> l(g_lock); g_toast = text; g_toastUntil = ticks() + 2000; }

bool stickHeld(SHORT x, SHORT y) { return std::abs(int(x)) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || std::abs(int(y)) > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE; }

// The menu just closed: everything still held stays away from the game until released.
void hideHeld(const XINPUT_GAMEPAD& in) {
    g_hidden = in.wButtons;
    g_hideLt = in.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD, g_hideRt = in.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
    g_hideLs = stickHeld(in.sThumbLX, in.sThumbLY), g_hideRs = stickHeld(in.sThumbRX, in.sThumbRY);
    g_solo = g_pulse = 0;
    if (g_hidden || g_hideLt || g_hideRt || g_hideLs || g_hideRs)
        std::fprintf(stderr, "overlay: menu closed; held buttons 0x%04X%s%s stay hidden from the game until released\n", g_hidden,
                     g_hideLt || g_hideRt ? " + triggers" : "", g_hideLs || g_hideRs ? " + sticks" : "");
}

// Closed menu: what reaches the game (see g_hidden).
void filterClosed(XINPUT_GAMEPAD& g, bool combo, uint32_t now) {
    static const uint32_t soloMs = [] { const char* e = std::getenv("FABLE_OVERLAY_SOLO_MS"); return e ? static_cast<uint32_t>(std::atoi(e)) : 120u; }();
    constexpr WORD kCombo = XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START;
    const WORD raw = g.wButtons, solo = raw & kCombo;
    g_hidden &= raw;  // released: the game sees the button again
    if (g_hideLt && g.bLeftTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) g_hideLt = false;
    if (g_hideRt && g.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) g_hideRt = false;
    if (g_hideLs && !stickHeld(g.sThumbLX, g.sThumbLY)) g_hideLs = false;
    if (g_hideRs && !stickHeld(g.sThumbRX, g.sThumbRY)) g_hideRs = false;
    if (combo) {
        g_hidden |= kCombo, g_solo = 0;
    } else if (solo && soloMs) {
        if (solo != g_solo) g_solo = solo, g_soloSince = now, g_soloPassed = (g_hidden & solo) != 0;
        if (!g_soloPassed && now - g_soloSince < soloMs) g.wButtons &= static_cast<WORD>(~solo);
        else g_soloPassed = true;
    } else {
        if (g_solo && !g_soloPassed) g_pulse = g_solo, g_pulseUntil = now + std::max(soloMs, 100u);  // a short tap, replayed
        g_solo = 0;
    }
    g.wButtons &= static_cast<WORD>(~g_hidden);
    if (g_pulse) {
        if (now < g_pulseUntil) g.wButtons |= g_pulse;
        else g_pulse = 0;
    }
    if (g_hideLt) g.bLeftTrigger = 0;
    if (g_hideRt) g.bRightTrigger = 0;
    if (g_hideLs) g.sThumbLX = g.sThumbLY = 0;
    if (g_hideRs) g.sThumbRX = g.sThumbRY = 0;
}

void filterPad(XINPUT_GAMEPAD& g, bool guide) {
    if (!enabled()) return;
    std::lock_guard<std::mutex> l(g_lock);
    const uint32_t now = ticks();
    if (g_exiting && now > g_exitAt) w32::drainFileFlushes();  // the background fsyncs of FlushFileBuffers (files.cpp), before the hard exit
    if (g_exiting && now > g_exitAt) { std::fprintf(stderr, "overlay: exit\n"); std::fflush(nullptr); _exit(0); }
    const XINPUT_GAMEPAD in = g;
    const WORD pressed = static_cast<WORD>(g.wButtons & ~g_lastButtons);
    g_lastButtons = g.wButtons;
    const bool guidePressed = guide && !g_lastGuide;
    g_lastGuide = guide;
    // Back + Start held also opens it (pads whose guide button is not mapped)
    const bool combo = (g.wButtons & (XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START)) == (XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START);
    if (combo) { if (!g_guideDown) g_guideDown = now; } else g_guideDown = 0;
    const bool comboHeld = g_guideDown && now - g_guideDown > 1500;
    if (!g_open) {
        if (guidePressed || comboHeld) { g_open = true; g_page = 0; g_sel = 0; g_guideDown = 0; std::memset(&g, 0, sizeof g); return; }
        filterClosed(g, combo, now);
        return;
    }
    if (g_page == 1) {  // graphics page
        if (pressed & XINPUT_GAMEPAD_DPAD_UP) g_sel = (g_sel + G_COUNT - 1) % G_COUNT;
        if (pressed & XINPUT_GAMEPAD_DPAD_DOWN) g_sel = (g_sel + 1) % G_COUNT;
        if (pressed & XINPUT_GAMEPAD_DPAD_LEFT) gfxChange(g_sel, -1);
        if (pressed & XINPUT_GAMEPAD_DPAD_RIGHT) gfxChange(g_sel, 1);
        if (pressed & XINPUT_GAMEPAD_A) gfxChange(g_sel, 0);
        if ((pressed & XINPUT_GAMEPAD_B) || guidePressed) { g_page = 0; g_sel = GRAPHICS; }
        std::memset(&g, 0, sizeof g);
        return;  // the graphics page never closes the menu itself
    }
    // open: the game sees nothing; navigate
    if (pressed & XINPUT_GAMEPAD_DPAD_UP) g_sel = (g_sel + COUNT - 1) % COUNT;
    if (pressed & XINPUT_GAMEPAD_DPAD_DOWN) g_sel = (g_sel + 1) % COUNT;
    if (pressed & XINPUT_GAMEPAD_A) activate(g_sel);
    if (g_sel == VOLUME && (pressed & (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT)))
        w32::setMasterVolume(w32::masterVolume() + ((pressed & XINPUT_GAMEPAD_DPAD_RIGHT) ? 0.05f : -0.05f));
    if (g_sel == LOGLEVEL && (pressed & (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT))) host::g_logLevel = (host::g_logLevel + ((pressed & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 2)) % 3;
    if ((pressed & XINPUT_GAMEPAD_B) || guidePressed) g_open = false;
    if (!g_open) hideHeld(in);  // closed by B, the guide, Resume or Screenshot
    std::memset(&g, 0, sizeof g);
}

std::vector<Line> lines(int width, int height) {
    std::lock_guard<std::mutex> l(g_lock);
    std::vector<Line> out;
    const int s = textScale(height);
    const int ch = 16 * s, cw = 8 * s;
    char buf[128];
    if (g_fps) {
        std::snprintf(buf, sizeof buf, "%.1f fps", g_fpsValue);
        out.push_back({4, 4, buf, 0xFFFFFF40});
    }
    if (g_stats) out.push_back({4, 4 + (g_fps ? ch : 0), g_statsText, 0xFFFFFF40});
    if (!g_toast.empty() && ticks() < g_toastUntil) out.push_back({(width - static_cast<int>(g_toast.size()) * cw) / 2, height - 3 * ch, g_toast, 0xFFFFFFFF});
    if (!g_open) return out;
    const int count = g_page == 1 ? G_COUNT : COUNT;
    const int lineCount = count + 3;
    int y = (height - lineCount * ch) / 2;
    const int x = std::max(8, (width - 40 * cw) / 2);
    out.push_back({x, y, g_page == 1 ? "GRAPHICS  (applied at once and saved; the game's own Video Options are off)" : "FABLE RECOMP", 0xFFFFD040});
    y += ch;
    out.push_back({x, y, "D-pad: move / change   A: select   B: back", 0xFFA0A0A0});
    y += ch * 2;
    for (int i = 0; i < count; ++i, y += ch) {
        const bool sel = i == g_sel;
        out.push_back({x, y, (sel ? "> " : "  ") + (g_page == 1 ? gfxLabel(i) : label(i)), sel ? 0xFFFFFFFF : 0xFFC0C0C0});
    }
    (void)cw;
    return out;
}

}  // namespace host::overlay
