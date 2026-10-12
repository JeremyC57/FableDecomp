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
bool g_fps = false, g_stats = false, g_vsync = true, g_shot = false;
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

uint32_t ticks() {
    using namespace std::chrono;
    return static_cast<uint32_t>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

enum Item { RESUME, GRAPHICS, VOLUME, FPS, STATS, LOGLEVEL, VSYNC, SCREENSHOT, EXIT, COUNT };
// the graphics page
enum GfxItem { G_DRAW, G_LOD, G_SHADOWSIZE, G_SHADOWS, G_GLOW, G_BACK, G_COUNT };
int g_page = 0;  // 0: main, 1: graphics
std::string gfxLabel(int i) {
    const host::gfx::Settings& s = host::gfx::get();
    switch (i) {
        case G_DRAW: return "Draw distance: " + std::to_string(s.drawDistance) + "%  (left/right)";
        case G_LOD: return "Model detail: " + std::to_string(s.modelDetail) + "%  (left/right)";
        case G_SHADOWSIZE: return std::string("Shadow map: ") + (s.shadowSize ? std::to_string(s.shadowSize) : "game default") + "  (left/right)";
        case G_SHADOWS: return std::string("Shadows: ") + (s.shadows ? "on" : "off");
        case G_GLOW: return std::string("Glow / bloom: ") + (s.glow ? "on" : "off");
        case G_BACK: return "Back";
        default: return "";
    }
}
void gfxChange(int i, int dir) {  // dir: -1 left, +1 right, 0 select
    host::gfx::Settings s = host::gfx::get();
    static const int sizes[] = {0, 128, 256, 512, 1024};
    switch (i) {
        case G_DRAW: s.drawDistance = std::clamp(s.drawDistance + (dir ? dir : 1) * 10, 30, 150); break;
        case G_LOD: s.modelDetail = std::clamp(s.modelDetail + (dir ? dir : -1) * 25, 25, 100); break;
        case G_SHADOWSIZE: {
            int k = 0;
            for (int j = 0; j < 5; ++j) if (sizes[j] == s.shadowSize) k = j;
            k = (k + (dir < 0 ? 4 : 1)) % 5;
            s.shadowSize = sizes[k];
            break;
        }
        case G_SHADOWS: s.shadows = !s.shadows; break;
        case G_GLOW: s.glow = !s.glow; break;
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
        case VSYNC: return std::string("VSync: ") + (g_vsync ? "on" : "off");
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
        case VSYNC: g_vsync = !g_vsync; break;
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
bool vsync() { return g_vsync; }
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

void filterPad(XINPUT_GAMEPAD& g, bool guide) {
    if (!enabled()) return;
    std::lock_guard<std::mutex> l(g_lock);
    const uint32_t now = ticks();
    if (g_exiting && now > g_exitAt) { std::fprintf(stderr, "overlay: exit\n"); std::fflush(nullptr); _exit(0); }
    const WORD pressed = static_cast<WORD>(g.wButtons & ~g_lastButtons);
    g_lastButtons = g.wButtons;
    const bool guidePressed = guide && !g_lastGuide;
    g_lastGuide = guide;
    // Back + Start held also opens it (pads whose guide button is not mapped)
    const bool combo = (g.wButtons & (XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START)) == (XINPUT_GAMEPAD_BACK | XINPUT_GAMEPAD_START);
    if (combo) { if (!g_guideDown) g_guideDown = now; } else g_guideDown = 0;
    const bool comboHeld = g_guideDown && now - g_guideDown > 1500;
    if (!g_open) {
        if (guidePressed || comboHeld) { g_open = true; g_page = 0; g_sel = 0; g_guideDown = 0; std::memset(&g, 0, sizeof g); }
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
        return;
    }
    // open: the game sees nothing; navigate
    if (pressed & XINPUT_GAMEPAD_DPAD_UP) g_sel = (g_sel + COUNT - 1) % COUNT;
    if (pressed & XINPUT_GAMEPAD_DPAD_DOWN) g_sel = (g_sel + 1) % COUNT;
    if (pressed & XINPUT_GAMEPAD_A) activate(g_sel);
    if (g_sel == VOLUME && (pressed & (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT)))
        w32::setMasterVolume(w32::masterVolume() + ((pressed & XINPUT_GAMEPAD_DPAD_RIGHT) ? 0.05f : -0.05f));
    if (g_sel == LOGLEVEL && (pressed & (XINPUT_GAMEPAD_DPAD_LEFT | XINPUT_GAMEPAD_DPAD_RIGHT))) host::g_logLevel = (host::g_logLevel + ((pressed & XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 2)) % 3;
    if ((pressed & XINPUT_GAMEPAD_B) || guidePressed) g_open = false;
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
    out.push_back({x, y, g_page == 1 ? "GRAPHICS  (changes apply at once and are saved)" : "FABLE RECOMP", 0xFFFFD040});
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
