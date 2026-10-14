// Controller navigation for the PC menus (Xbox-style focus movement).
//
// The PC frontend still turns pad input into the Xbox build's menu events: the stick and
// d-pad become 0..3 (up/down/left/right), A and Start 4 (select), B and Back 5 (back),
// sent to the UI manager (CNewFrontendGameComponent::Input 0x42E3EE). On PC nothing
// consumes 0..4: the menus are made of NUISystem clickables (CNavButton, CFrontEndButton)
// that react only to the mouse. The lifter wraps the manager's event dispatch
// (CManager::ProcessEvent 0x55CB10, --wrap 0x55CB10=host_ui_event) and this file gives
// those events their Xbox meaning:
//   - a direction moves the focus to the nearest active button that way. Focus is the
//     game's own hover state (hover enter/leave, the hovered flag, the state refresh), so
//     the highlight and sounds are those of the mouse;
//   - select (A) presses and releases the focused button (events 0x1A/0x1C), which runs
//     its action through the button's own code;
//   - back (B) presses the screen's Back / Cancel / No button (def Action 86).
// While a pad is connected, poll() (from the joystick update) focuses a screen's default
// button when it opens and returns to the remembered one when going back to a screen.
// Events the navigator does not take (no button in that direction, mouse events) go to the
// original dispatcher, so scrolling lists and the mouse work as before.
//
// In game, XBOX_MENUS (controller.cpp) brings back the Xbox in-game menus, which the PC build
// still contains: the inventory opens the Xbox screens instead of the PC menu
// (host_open_inventory), they are scaled from their 640x480 layout (host_ui_scale), and A
// reaches them as the Xbox select event. The navigator stays out of the in-game menus.
#include "controller.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <set>
#include <vector>

using namespace host;
static bool safeRead(uint32_t addr, void* out, size_t n);  // below: a guest read that fails instead of faulting

extern "C" void F_0055CB10_orig(Ctx* c);

namespace {

constexpr uint32_t kGetUiManager = 0x41E5F2;      // CFrontEndManager::GetInstance
constexpr uint32_t kManagerDispatch = 0x55CB10;   // CManager::ProcessEvent (wrapped)
constexpr uint32_t kClickableProcess = 0x55AD60;  // CClickable::ProcessEvent (CNavButton)
constexpr uint32_t kFrontEndButtonProcess = 0x54DBC0;

constexpr uint32_t kUiManager = 0x13B8710;      // its instance (0 before the first GetInstance)
constexpr uint32_t kDefAction = 228;             // CUIDef::Action (the Ego_r PDB has it at 224)
constexpr uint32_t kActionBack = 86;             // the action of Back / Cancel / No buttons
constexpr uint32_t kActionVideoOptions = 13;     // the Options screen's Video Options button
// FABLE_HOST_GFX=1: the host (overlay / launcher) owns the graphics settings and the game's
// Video Options page is kept out of reach (the pad skips it, a click on it does nothing).
bool hostGfx() {
    static const bool on = [] { const char* e = std::getenv("FABLE_HOST_GFX"); return e && std::atoi(e) != 0; }();
    return on;
}

enum : uint32_t { EV_UP = 0, EV_DOWN = 1, EV_LEFT = 2, EV_RIGHT = 3, EV_SELECT = 4, EV_BACK = 5, EV_MOUSE_MOVE = 0x19,
                  EV_LEFT_PRESS = 0x1A, EV_LEFT_RELEASE = 0x1C };

struct Button {
    uint32_t widget;
    uint32_t action;  // the def's Action
    float l, t, r, b;
    float cx() const { return (l + r) * 0.5f; }
    float cy() const { return (t + b) * 0.5f; }
};

uint32_t g_focus = 0;  // widget the pad last focused
}  // namespace
extern bool g_pcScreen;  // host_create_component: a PC_ screen was the last created
namespace {

bool xboxMenus() {
    static const bool on = host::pad::xboxMenusEnabled();
    return on;
}
// The in-game menu (CPlayerGui's live GUI flag) is open.
bool liveGui() {
    const uint32_t gui = rd32(0x13B8790);
    return gui && rd8(gui + 0x2BE);
}

// The button's def (vtable +0x1B0 returns a counted pointer through an out parameter, as
// CFrontEndButton::ProcessEvent 0x54DBC0 uses it): its Action, then the reference is dropped
// the same way. In retail, CUIDef's fields from Action on sit 4 bytes later than in the
// Ego_r PDB (seen in memory: the main menu's buttons hold their actions at +228).
uint32_t defAction(uint32_t widget, uint32_t out) {
    wr32(out, 0);
    guestCallThis(rd32(rd32(widget) + 0x1B0), widget, {out});
    const uint32_t def = rd32(out);
    if (!def) return 0;
    const uint32_t action = rd32(def + kDefAction);
    const uint32_t refs = rd32(def + 4) - 1;
    wr32(def + 4, refs);
    if (!refs) guestCallThis(rd32(rd32(def) + 4), def, {});
    return action;
}

// The active clickable buttons registered with the manager, with their screen rectangles
// (computed as CClickable::IsInside 0x55B8F0 does).
std::vector<Button> activeButtons(uint32_t mgr, uint32_t scratch) {
    std::vector<Button> out;
    const uint32_t head = rd32(mgr + 4);
    uint32_t n = rd32(head);
    for (int guard = 0; n != head && guard < 1024; ++guard, n = rd32(n)) {
        const uint32_t comp = rd32(n + 8);
        if (!comp) continue;
        const uint32_t cvt = rd32(comp);
        const uint32_t process = rd32(cvt + 4);
        if (process != kClickableProcess && process != kFrontEndButtonProcess) continue;
        if (!(guestCallThis(rd32(cvt + 8), comp, {EV_MOUSE_MOVE}) & 0xFF)) continue;  // inactive / hidden
        const uint32_t widget = comp - 4, wvt = rd32(widget);
        // Fully transparent (colour +0x84, ARGB): e.g. a list's scroll arrows while the list
        // fits. They still answer the mouse, but nothing shows where they are.
        if (!(rd32(widget + 0x84) >> 24)) continue;
        const uint32_t pos = scratch, size = scratch + 8, off = scratch + 16;
        std::memset(gp(scratch), 0, 32);
        guestCallThis(rd32(wvt + 0x1E8), widget, {pos});
        guestCallThis(rd32(wvt + 0x1EC), widget, {size});
        guestCallThis(rd32(wvt + 0x60), widget, {off});
        Button bt;
        bt.widget = widget;
        bt.action = defAction(widget, scratch + 32);
        // Same arithmetic as CClickable::IsInside (0x55B8F0): +0x1E8 position, +0x1EC scale,
        // +0x60 offset and size. Only the relative layout matters here.
        bt.l = rdf32(off) + rdf32(pos);
        bt.t = rdf32(off + 4) + rdf32(pos + 4);
        bt.r = rdf32(size) * rdf32(off + 8) + rdf32(pos);
        bt.b = rdf32(size + 4) * rdf32(off + 12) + rdf32(pos + 4);
        if (!(bt.r > bt.l && bt.b > bt.t) || !std::isfinite(bt.l + bt.r + bt.t + bt.b)) continue;
        if (hostGfx() && bt.action == kActionVideoOptions) continue;
        out.push_back(bt);
    }
    return out;
}

const Button* find(const std::vector<Button>& v, uint32_t widget) {
    for (const Button& b : v)
        if (b.widget == widget) return &b;
    return nullptr;
}

// Nearest button in a direction: the distance along it between centres, plus the sideways
// gap between the rectangles (0 when they overlap), within a cone. Rectangles rather than
// centres sideways, because buttons of one column differ in width (the main menu's are as
// wide as their text).
const Button* step(const std::vector<Button>& v, const Button& from, uint32_t dir) {
    const Button* best = nullptr;
    float bestScore = 0;
    auto gap = [](float a0, float a1, float b0, float b1) { return std::max(0.0f, std::max(b0 - a1, a0 - b1)); };
    for (const Button& b : v) {
        if (b.widget == from.widget) continue;
        const float dx = b.cx() - from.cx(), dy = b.cy() - from.cy();
        float along, across;
        switch (dir) {
            case EV_UP: along = -dy, across = gap(from.l, from.r, b.l, b.r); break;
            case EV_DOWN: along = dy, across = gap(from.l, from.r, b.l, b.r); break;
            case EV_LEFT: along = -dx, across = gap(from.t, from.b, b.t, b.b); break;
            default: along = dx, across = gap(from.t, from.b, b.t, b.b); break;
        }
        if (along <= 1.0f || across > along) continue;  // not that way (outside 45 degrees)
        const float centres = std::fabs(dir == EV_UP || dir == EV_DOWN ? dx : dy);  // tie-break
        const float score = along + 2.5f * across + 0.1f * centres;
        if (!best || score < bestScore) best = &b, bestScore = score;
    }
    return best;
}

// Hover state, set the way CClickable's own hover pass (0x55BF10) does it: OnHoverEnter
// (+0x23C) / OnHoverLeave (+0x240), the hovered flag, then the state refresh (+0x214).
void setHovered(uint32_t widget, bool on) {
    if ((rd8(widget + 0x160) != 0) == on) return;
    const uint32_t vt = rd32(widget);
    guestCallThis(rd32(vt + (on ? 0x23C : 0x240)), widget, {});
    wr8(widget + 0x160, on ? 1 : 0);
    guestCallThis(rd32(vt + 0x214), widget, {});
}

void focusOn(const std::vector<Button>& all, const Button& b) {
    for (const Button& o : all)
        if (o.widget != b.widget) setHovered(o.widget, false);
    setHovered(b.widget, true);
    g_focus = b.widget;
}

void press(uint32_t mgr, const std::vector<Button>& all, const Button& b) {
    focusOn(all, b);
    guestCallThis(kManagerDispatch, mgr, {EV_LEFT_PRESS});
    guestCallThis(kManagerDispatch, mgr, {EV_LEFT_RELEASE});
}

// The button a screen starts on: the top-left one.
const Button& defaultButton(const std::vector<Button>& buttons) {
    const Button* first = &buttons[0];
    for (const Button& b : buttons)
        if (b.t < first->t - 1 || (std::fabs(b.t - first->t) <= 1 && b.l < first->l)) first = &b;
    return *first;
}

const Button* hovered(const std::vector<Button>& buttons) {
    for (const Button& b : buttons)
        if (rd8(b.widget + 0x160)) return &b;  // by the pad or the mouse
    return nullptr;
}

// true when the navigator handled the event
bool navigate(uint32_t mgr, uint32_t ev, uint32_t scratch) {
    if (ev > EV_BACK) return false;
    const std::vector<Button> buttons = activeButtons(mgr, scratch);
    if (buttons.empty()) {
        if (ev != EV_SELECT) return false;
        // Screens without buttons ("Press Left Mouse Button To Continue") wait for a click.
        guestCallThis(kManagerDispatch, mgr, {EV_LEFT_PRESS});
        guestCallThis(kManagerDispatch, mgr, {EV_LEFT_RELEASE});
        return true;
    }
    if (ev == EV_BACK) {  // B: the screen's Back / Cancel / No button, as on Xbox
        for (const Button& b : buttons)
            if (b.action == kActionBack) {
                HLOG(1, "menu: back 0x%08X", b.widget);
                press(mgr, buttons, b);
                return true;
            }
        return false;
    }
    const Button* cur = hovered(buttons);
    if (!cur) cur = find(buttons, g_focus);
    if (ev == EV_SELECT) {
        if (!cur) return false;
        HLOG(1, "menu: select 0x%08X", cur->widget);
        press(mgr, buttons, *cur);
        return true;
    }
    if (!cur) {  // first press on a screen: focus its default button
        focusOn(buttons, defaultButton(buttons));
        return true;
    }
    // Left/right on an options row work its arrow buttons, the way the Xbox sliders and
    // choices change with the pad, and the focus stays where it is. A row is the focused
    // button and the small (arrow) buttons level with it.
    if (ev == EV_LEFT || ev == EV_RIGHT) {
        auto arrow = [](const Button& b) { return b.r - b.l < 2.0f * (b.b - b.t); };
        const Button* target = nullptr;
        for (const Button& b : buttons) {
            if (!arrow(b) || !(b.t < cur->b && cur->t < b.b)) continue;
            if (!arrow(*cur) && (ev == EV_LEFT ? b.cx() > cur->cx() : b.cx() < cur->cx())) continue;
            if (!target || (ev == EV_LEFT ? b.cx() < target->cx() : b.cx() > target->cx())) target = &b;
        }
        if (target && (arrow(*cur) || std::fabs(target->cx() - cur->cx()) > 1)) {
            HLOG(1, "menu: event %u presses 0x%08X on the row of 0x%08X", ev, target->widget, cur->widget);
            const uint32_t keep = cur->widget;
            press(mgr, buttons, *target);
            if (const Button* row = find(buttons, keep)) focusOn(buttons, *row);
            return true;
        }
    }
    const Button* next = step(buttons, *cur, ev);
    HLOG(1, "menu: event %u from 0x%08X (%.0f,%.0f) to 0x%08X", ev, cur->widget, cur->cx(), cur->cy(), next ? next->widget : 0);
    if (!next) return false;  // nothing that way: let lists scroll
    focusOn(buttons, *next);
    return true;
}

}  // namespace

namespace host::menu {

// Once per input update while a controller is connected: when a new set of buttons comes
// up (a screen or dialog opens) and nothing is hovered, focus the default button, as the
// Xbox menus show their selection from the start.
void poll(uint32_t scratch) {
    static std::vector<uint32_t> last;
    static float mouseX, mouseY;
    static bool padOwns;  // the pad holds the focus until the mouse moves
    const uint32_t mgr = rd32(kUiManager);
    if (!mgr) return;
    const float mx = rdf32(mgr + 0xB0), my = rdf32(mgr + 0xB4);
    if (mx != mouseX || my != mouseY) mouseX = mx, mouseY = my, padOwns = false;
    const std::vector<Button> buttons = activeButtons(mgr, scratch);
    std::vector<uint32_t> ids;
    for (const Button& b : buttons) ids.push_back(b.widget);
    static int settle;                                      // input updates since the buttons changed
    static std::map<std::vector<uint32_t>, uint32_t> remembered;  // focus per screen, for returning to it
    if (ids != last) {
        if (!last.empty() && g_focus) remembered[last] = g_focus;
        last = ids, padOwns = true, settle = 0;
        const auto it = remembered.find(ids);
        g_focus = it != remembered.end() ? it->second : 0;
    }
    // A dialog opening resets its buttons' look, so the first focus waits for it to settle.
    if (++settle < 20 || !padOwns || buttons.empty() || hovered(buttons)) return;
    // Screens reset their buttons' hover state while they open, so this holds the focus.
    const Button* f = find(buttons, g_focus);
    if (!f) f = &defaultButton(buttons);
    HLOG(1, "menu: %zu buttons, focus 0x%08X", buttons.size(), f->widget);
    for (const Button& b : buttons)
        HLOG(2, "menu:   0x%08X vt 0x%08X action %u rect %.0f,%.0f-%.0f,%.0f", b.widget, rd32(b.widget), b.action, b.l, b.t, b.r, b.b);
    focusOn(buttons, *f);
}

// The PC screens the game still builds in play (the quest table, quest start and completion,
// trading, the experience screen) are mouse screens: their clickable buttons answer the
// manager's mouse events only, so the pad did nothing there. While one is up (a PC_ screen
// was the last created, the Xbox menus are closed, and clickable buttons are registered) the
// pad drives them through the front end's navigator: A selects, B presses the Back/close
// button, the d-pad and left stick move the focus, the right stick presses the scroll arrows
// of the description pane (the lowest column of arrow buttons). The caller keeps those
// inputs from the game while this returns true.
bool pcScreenPad(uint32_t scratch, int dirEv, bool a, bool b, float ry) {
    const uint32_t mgr = rd32(kUiManager);
    if (!mgr || !rd32(0x13B8790) || liveGui() || !g_pcScreen) return false;
    if (dirEv < 0 && !a && !b && std::fabs(ry) < 0.5f) {
        // nothing pressed: report whether the screen is up, cheaply (buttons cached per frame)
        static uint32_t lastCheck;
        static bool lastUp;
        const uint32_t now = GetTickCount();
        if (now - lastCheck > 250) lastCheck = now, lastUp = !activeButtons(mgr, scratch).empty();
        return lastUp;
    }
    const std::vector<Button> buttons = activeButtons(mgr, scratch);
    if (buttons.empty()) return false;
    if (b) { HLOG(1, "menu: pc screen: back"); if (!navigate(mgr, EV_BACK, scratch)) {
        // no Back-action button: the close button is the small one furthest top-right
        const Button* close = nullptr;
        for (const Button& x : buttons)
            if (x.r - x.l < 2.0f * (x.b - x.t) && (!close || x.t < close->t - 1 || (std::fabs(x.t - close->t) <= 1 && x.r > close->r))) close = &x;
        if (close) press(mgr, buttons, *close);
    } }
    // Up/down and A go to the screen's list as the manager's own UI events (the PC lists'
    // entries are not clickable buttons, so the button navigator walked the scroll arrows);
    // left/right still move between buttons.
    if (a) { HLOG(1, "menu: pc screen: select"); guestCallThis(kManagerDispatch, mgr, {EV_SELECT}); }
    if (dirEv == EV_UP || dirEv == EV_DOWN) { HLOG(1, "menu: pc screen: list %s", dirEv == EV_UP ? "up" : "down"); guestCallThis(kManagerDispatch, mgr, {static_cast<uint32_t>(dirEv)}); }
    else if (dirEv >= 0) navigate(mgr, static_cast<uint32_t>(dirEv), scratch);
    if (std::fabs(ry) >= 0.5f) {
        static uint32_t lastScroll;
        const uint32_t now = GetTickCount();
        if (now - lastScroll >= 120) {
            lastScroll = now;
            auto arrow = [](const Button& x) { return x.r - x.l < 2.0f * (x.b - x.t) && x.b - x.t < 60.0f; };
            const Button* down = nullptr;
            for (const Button& x : buttons) if (arrow(x) && (!down || x.cy() > down->cy())) down = &x;
            const Button* up = nullptr;
            if (down)
                for (const Button& x : buttons)
                    if (arrow(x) && &x != down && std::fabs(x.cx() - down->cx()) < 8.0f && x.cy() < down->cy() && (!up || x.cy() > up->cy())) up = &x;
            const Button* target = ry > 0 ? down : up;  // the game's stick Y points down
            if (target) {
                const uint32_t keep = g_focus;
                press(mgr, buttons, *target);
                if (const Button* f = find(buttons, keep)) focusOn(buttons, *f);
            }
        }
    }
    return true;
}

}  // namespace host::menu

// FABLE_UI_DUMP: on a pad direction in the in-game menus, log every component of the manager
// (class vtable, def, position, colour, state), to see how a screen is built.
static void dumpComponents(uint32_t mgr, uint32_t scratch) {
    const uint32_t head = rd32(mgr + 4);
    uint32_t n = rd32(head);
    int i = 0;
    for (int guard = 0; n != head && guard < 2048; ++guard, n = rd32(n)) {
        const uint32_t comp = rd32(n + 8);
        if (!comp) continue;
        const uint32_t widget = comp - 4, wvt = rd32(widget);
        const uint32_t pos = scratch;
        std::memset(gp(scratch), 0, 32);
        guestCallThis(rd32(wvt + 0x1E8), widget, {pos});
        wr32(scratch + 16, 0);
        guestCallThis(rd32(wvt + 0x1B0), widget, {scratch + 16});
        const uint32_t def = rd32(scratch + 16);
        std::string name;
        if (def) {
            const uint32_t refs = rd32(def + 4) - 1;
            wr32(def + 4, refs);
            if (!refs) guestCallThis(rd32(rd32(def) + 4), def, {});
        }
        if (wvt == 0x012462E4) {  // CList: its def's ViewArea (retail +0x14C..+0x158) and its position
            uint32_t d = 0;
            wr32(scratch + 16, 0);
            guestCallThis(rd32(wvt + 0x1B0), widget, {scratch + 16});
            d = rd32(scratch + 16);
            std::memset(gp(scratch), 0, 32);
            guestCallThis(rd32(wvt + 0x1E8), widget, {scratch});
            if (d) {
                const uint32_t refs = rd32(d + 4) - 1;
                wr32(d + 4, refs);
                if (!refs) guestCallThis(rd32(rd32(d) + 4), d, {});
            }
            host::log("ui: #%d LIST comp 0x%08X def 0x%08X viewarea %d,%d %d,%d pos %.0f,%.0f size %.0f,%.0f colour 0x%08X", i, comp, d,
                      d ? (int)rd32(d + 0x14C) : 0, d ? (int)rd32(d + 0x150) : 0, d ? (int)rd32(d + 0x154) : 0, d ? (int)rd32(d + 0x158) : 0,
                      rdf32(scratch), rdf32(scratch + 4), rdf32(widget + 0x5C), rdf32(widget + 0x60), rd32(widget + 0x84));
        }
        if (wvt != 0x0124662C) { ++i; continue; }  // CMenuEntry only
        const uint32_t stateIdx = rd32(widget + 0x148);
        const uint32_t state = guestCallThis(rd32(wvt + 0x21C), widget, {stateIdx});
        if (def && i < 14) {  // the def's CUIStateDef vector (retail +64; CUIState::CUIState 0x52F440 reads the
            // def at +0x40 flags, +0x44 type, +0x48 pos, +0x50 zoom, +0x58 colour floats, +0x68 time, +0x78 linear)
            const uint32_t b = rd32(def + 64), e = rd32(def + 68);
            for (uint32_t size = 120; size <= 136 && b && e > b; size += 4) {
                if ((e - b) % size || (e - b) / size > 12) continue;
                std::string st;
                for (uint32_t p = b; p < e; p += size) {
                    char buf[200];
                    std::snprintf(buf, sizeof buf, " [fl %u ty %u pos %.1f,%.1f col %.2f,%.2f,%.2f,%.2f t %.2f lin %u]", rd32(p + 0x40), rd32(p + 0x44),
                                  rdf32(p + 0x48), rdf32(p + 0x4C), rdf32(p + 0x58), rdf32(p + 0x5C), rdf32(p + 0x60), rdf32(p + 0x64), rdf32(p + 0x68),
                                  rd8(p + 0x78));
                    st += buf;
                }
                host::log("ui:   def 0x%08X states (size %u):%s", def, size, st.c_str());
            }
        }
        if (def && i < 14) {
            std::string st;
            for (uint32_t k = 0; k < 6; ++k) {
                const uint32_t sp = guestCallThis(rd32(wvt + 0x21C), widget, {k});
                char buf[64];
                std::snprintf(buf, sizeof buf, " %u:0x%08X", k, sp ? rd32(sp + 24) : 0xDEADBEEF);
                st += buf;
            }
            host::log("ui:   states:%s  cur 0x%08X target 0x%08X start 0x%08X c90 0x%08X t %.2f/%.2f", st.c_str(), rd32(widget + 0x84), rd32(widget + 0x88), rd32(widget + 0x8C), rd32(widget + 0x90), rdf32(widget + 0xA8), rdf32(widget + 0xAC));
        }
        std::memset(gp(scratch), 0, 32);
        guestCallThis(rd32(wvt + 0x5C), widget, {scratch});
        host::log("ui:   entry size %.1f,%.1f zoom %.2f,%.2f", rdf32(scratch), rdf32(scratch + 4), rdf32(widget + 0x74), rdf32(widget + 0x78));
        if (i < 14) {  // the entry's children: vector of 8-byte {component, ?} at +0xB0..+0xB4
            for (uint32_t p = rd32(widget + 0xB0); p < rd32(widget + 0xB4) && p; p += 8) {
                const uint32_t ch = rd32(p), chvt = ch ? rd32(ch) : 0;
                if (!ch) continue;
                std::memset(gp(scratch), 0, 64);
                guestCallThis(rd32(chvt + 0x1E8), ch, {scratch});
                guestCallThis(rd32(chvt + 0x5C), ch, {scratch + 8});
                wr32(scratch + 16, 0);
                guestCallThis(rd32(chvt + 0x1B0), ch, {scratch + 16});
                const uint32_t cd = rd32(scratch + 16);
                if (cd) {
                    const uint32_t refs = rd32(cd + 4) - 1;
                    wr32(cd + 4, refs);
                    if (!refs) guestCallThis(rd32(rd32(cd) + 4), cd, {});
                }
                host::log("ui:     child 0x%08X vt 0x%08X def 0x%08X type %u pos %.0f,%.0f size %.0f,%.0f colour 0x%08X flag %u", ch, chvt, cd,
                          cd ? rd32(cd + 0x40) : 0, rdf32(scratch), rdf32(scratch + 4), rdf32(scratch + 8), rdf32(scratch + 12), rd32(ch + 0x84), rd32(p + 4));
            }
        }
        std::memset(gp(scratch), 0, 32);
        guestCallThis(rd32(wvt + 0x1E8), widget, {pos});
        host::log("ui: #%d comp 0x%08X def 0x%08X(+221=%u) pos %.0f,%.0f colour 0x%08X c90 0x%08X state %u/%u trans %u sflags 0x%08X s+4 %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f", i++,
                  comp, def, def ? rd8(def + 0x221) : 0, rdf32(pos), rdf32(pos + 4), rd32(widget + 0x84), rd32(widget + 0x90), stateIdx,
                  rd32(widget + 0x14C), rd8(widget + 0x154), state ? rd32(state) : 0, state ? rdf32(state + 4) : 0, state ? rdf32(state + 8) : 0,
                  state ? rdf32(state + 12) : 0, state ? rdf32(state + 16) : 0, state ? rdf32(state + 20) : 0, state ? rdf32(state + 24) : 0,
                  state ? rdf32(state + 28) : 0, state ? rdf32(state + 32) : 0);
    }
}

// 0x55CB10: void __thiscall CManager::ProcessEvent(int event)
extern "C" void host_ui_event(Ctx* c) {
    const uint32_t mgr = c->ecx, ev = rd32(c->esp + 4);
    if (ev <= EV_RIGHT && xboxMenus() && std::getenv("FABLE_UI_DUMP")) {
        static int dumps;
        if (dumps++ < 6) {
            const uint32_t savedEsp = c->esp;
            c->esp = (c->esp - 0x200) & ~0xFu;
            dumpComponents(mgr, c->esp + 0x20);  // needs 0x100 of scratch
            c->esp = savedEsp;
        }
    }
    // CInputProcessInventory sends INVENTORY_SELECT (A) to the menus as a mouse press (0x1A)
    // for the PC screens; the Xbox screens take the select event.
    // The same for the inventory sub-screens' processors (map 0x68AEA5, experience 0x68B355,
    // trade 0x68B83A, quests 0x68BF88).
    if (xboxMenus() && ev == EV_LEFT_PRESS) {
        const uint32_t from = rd32(c->esp);
        if (from == 0x68A291 || from == 0x68AEA5 || from == 0x68B355 || from == 0x68B83A || from == 0x68BF88) wr32(c->esp + 4, EV_SELECT);
    }
    // The select builds the chosen category's screen (CManager vfunc_14, 0x4200B0): through its
    // PC constructor (vtable +0x15C) when the player GUI's live-GUI byte (+0x2BE) is set, the
    // Xbox one (+0x134) otherwise, and the builders under it pick PC_ or UI_ parts by the same
    // byte. The host opens the Xbox inventory through the live-GUI action, so the byte is set
    // and Equipment -> Clothing built the PC screen into the Xbox one and crashed. A select
    // from the inventory processors runs with the byte clear (a toggle made inside carries over).
    if (xboxMenus() && rd32(c->esp + 4) == EV_SELECT) {
        const uint32_t from = rd32(c->esp), gui = rd32(0x13B8790);
        if (gui && rd8(gui + 0x2BE) && (from == 0x68A291 || from == 0x68AEA5 || from == 0x68B355 || from == 0x68B83A || from == 0x68BF88)) {
            wr8(gui + 0x2BE, 0);
            F_0055CB10_orig(c);
            if (rd32(0x13B8790) == gui) wr8(gui + 0x2BE, rd8(gui + 0x2BE) ? 0 : 1);
            return;
        }
    }
    // It also sends a direction on every "held" pad event (its second branch, 0x68A95D to
    // 0x68ABC6), i.e. every frame, which the Xbox screens take as one step each. They get
    // one step per press here and then a steady repeat while the direction is held.
    if (xboxMenus() && ev <= EV_RIGHT) {
        static double pressedAt[4], lastStep[4];
        const uint32_t from = rd32(c->esp);
        const double now = GetTickCount() / 1000.0;
        if (from >= 0x68A95D && from < 0x68ABC6) {
            if (now - pressedAt[ev] < 0.4 || now - lastStep[ev] < 0.15) {
                retStd(c, 0, 1);
                return;
            }
            lastStep[ev] = now;
        } else if (from >= 0x68A266 && from < 0x68A95D) {
            pressedAt[ev] = lastStep[ev] = now;
        }
    }
    static const bool menuTrace = std::getenv("FABLE_MENU_TRACE") != nullptr;
    if (ev != 0x19) HLOG(menuTrace ? 1 : 2, "menu: ui event %u to 0x%08X from 0x%08X (live gui %d)", ev, mgr, rd32(c->esp), liveGui() ? 1 : 0);
    // The navigator serves the front end only: events from CNewFrontendGameComponent::Input
    // (0x42E3EE..0x42E98F). The in-game processors (CInputProcessInGameMenu, the inventory
    // screens) send the Xbox screens their own select/back events, which must reach them
    // untouched (the navigator used to turn the pause menu's select into a mouse click).
    const uint32_t from = rd32(c->esp);
    // a mouse / touch press on the Video Options button: dropped when the host owns graphics
    if (hostGfx() && ev == EV_LEFT_PRESS && from >= 0x42E3EE && from < 0x42E98F && !liveGui()) {
        const uint32_t savedEsp = c->esp;
        c->esp = (c->esp - 0x80) & ~0xFu;
        bool block = false;
        {
            const uint32_t head = rd32(mgr + 4);
            uint32_t n = rd32(head);
            for (int guard = 0; n != head && guard < 1024; ++guard, n = rd32(n)) {
                const uint32_t widget = rd32(n + 8);
                if (widget && rd8(widget + 0x160) && defAction(widget, c->esp + 0x20) == kActionVideoOptions) block = true;
            }
        }
        c->esp = savedEsp;
        if (block) { HLOG(1, "menu: Video Options is host-owned; click ignored"); retStd(c, 0, 1); return; }
    }
    if (ev <= EV_BACK && from >= 0x42E3EE && from < 0x42E98F && mgr == guestCall(kGetUiManager, {})) {
        const uint32_t savedEsp = c->esp;
        c->esp = (c->esp - 0x80) & ~0xFu;
        const bool done = navigate(mgr, ev, c->esp + 0x20);
        c->esp = savedEsp;
        if (done) {
            retStd(c, 0, 1);
            return;
        }
    }
    F_0055CB10_orig(c);
}

// 0x661F70 bool CThingCreatureBase::IsChild(): [[this+0xE0]+0xC4] == 1 (the age stage). From the
// Xbox Skills screens, CTCInventoryAbilities::GetPrioritisedAbilityDefs (0x5F4B30) asks it of the
// inventory's owner (+4) while that creature's +0xE0 is still null (selecting a spell category
// or an expression crashed reading 0xC4). A null reads as "not a child".
extern "C" void F_00661F70_orig(Ctx* c);
extern "C" void host_is_child(Ctx* c) {
    const uint32_t self = c->ecx;
    if (self < 0x10000 || self >= kGuestLimit || rd32(self + 0xE0) < 0x10000 || rd32(self + 0xE0) >= kGuestLimit) {
        static int logged;
        if (logged++ < 5) HLOG(1, "creature: IsChild on 0x%08X with no +0xE0 (from 0x%08X): false", self, rd32(c->esp));
        c->eax = 0;
        c->esp += 4;  // ret
        return;
    }
    F_00661F70_orig(c);
}

// 0x530780 CComponent* CComponent::FindChild(CCharString name), recursive over the children
// (vtable +0xC), the string by value (the callee releases it). The inventory screens pick
// their parts by the player GUI's live-GUI byte (+0x2BE): PC_MENU_ENTRY_ATTACK_SPELLS with it
// set, UI_MENU_ENTRY_ATTACK_SPELLS without (CTCInventoryAbilities::GetMenuEntryFromCategory
// 0x5EE288, the equipment categories likewise). The host opens the Xbox inventory through the
// live-GUI action, so the byte is set while the Xbox screens (built from UI_ parts) are up:
// the PC_ lookup found nothing and the caller dereferenced the null (the crash entering a
// category). With the Xbox menus, a PC_ lookup that fails at the outermost call is retried
// with the UI_ name.
extern "C" void F_00530780_orig(Ctx* c);
extern "C" void host_find_child(Ctx* c) {
    static thread_local int depth = 0;
    const uint32_t self = c->ecx;
    std::string name;
    if (depth == 0 && xboxMenus()) {
        const uint32_t data = rd32(c->esp + 4);
        const uint32_t ptr = data >= 0x10000 && data < kGuestLimit ? rd32(data) : 0, len = data >= 0x10000 && data < kGuestLimit ? rd32(data + 4) : 0;
        if (ptr >= 0x10000 && ptr < kGuestLimit && len > 3 && len < 96) name.assign(gp<const char>(ptr), len);
    }
    ++depth;
    F_00530780_orig(c);
    --depth;
    if (c->eax || name.rfind("PC_", 0) != 0) return;
    const std::string alt = "UI_" + name.substr(3);
    const uint32_t mem = gcalloc(1, 8 + static_cast<uint32_t>(alt.size()) + 1);
    const uint32_t slot = mem, chars = mem + 8;
    std::memcpy(gp<char>(chars), alt.c_str(), alt.size() + 1);
    guestCallThis(0x99EBF0, slot, {chars, 0xFFFFFFFFu});  // CCharString(const char*, -1)
    const uint32_t eaxSaved = c->eax;
    ++depth;
    const uint32_t found = guestCallThis(0x530780, self, {rd32(slot)});  // releases the string
    --depth;
    gfree(mem);
    static std::set<std::string> logged;
    if (logged.insert(name).second) HLOG(1, "ui: FindChild(%s) failed in an Xbox screen: %s -> 0x%08X", name.c_str(), alt.c_str(), found);
    c->eax = found ? found : eaxSaved;
}

// 0x41DB1D CManager::CreateComponent(const CCharString& defName, int): every screen and
// component comes through here by def name. The PC screens the game still builds (PC_QUEST_*,
// the quest table, trading, the experience screen) are laid out for 1024x768; the Xbox ones
// (UI_*) for 640x480. The last top-level name seen decides whether host_ui_scale applies the
// Xbox 1.6: a PC_ name turns it off until a UI_ name turns it back on.
bool g_pcScreen = false;  // declared near the top
extern "C" void F_0041DB1D_orig(Ctx* c);
extern "C" void host_create_component(Ctx* c) {
    // CCharString (PDB): one pointer to CCharStringData {CBasicString<char> {char* +0, length +4}, refs +0xD}
    const uint32_t str = rd32(c->esp + 4);
    const uint32_t data = str ? rd32(str) : 0;
    const uint32_t ptr = data >= 0x10000 && data < kGuestLimit ? rd32(data) : 0, len = data >= 0x10000 && data < kGuestLimit ? rd32(data + 4) : 0;
    char name[48] = {};
    if (ptr >= 0x10000 && ptr < kGuestLimit && len > 0 && len < sizeof name) std::memcpy(name, gp<const char>(ptr), len);
    // Templates and helpers (PC_WEAPON_BUTTON_DYNAMIC_TEMPLATE, PC_BUTTON_BASTARD, ..._DUMMY)
    // are created inside Xbox screens too (the quick-access menu, the inventory's weapon
    // rows): they decide nothing. A PC_ screen part turns the Xbox zoom off; only an Xbox
    // screen root (a UI_ menu, list or screen) turns it back on.
    const std::string n(name);
    const bool helper = n.find("TEMPLATE") != std::string::npos || n.find("BASTARD") != std::string::npos || n.find("DUMMY") != std::string::npos;
    const bool pc = !helper && n.rfind("PC_", 0) == 0;
    const bool xbox = !helper && n.rfind("UI_", 0) == 0 &&
                      (n.rfind("UI_LIST_", 0) == 0 || n.find("MENU") != std::string::npos || n.find("SCREEN") != std::string::npos);
    static std::set<std::string> names;
    static const bool allNames = std::getenv("FABLE_UI_NAMES") != nullptr;  // every name, every time (diagnostic)
    if (allNames) HLOG(1, "ui: CreateComponent(\"%s\")", name);
    else if (pc && names.size() < 400 && names.insert(name).second) HLOG(1, "ui: CreateComponent(\"%s\")", name);
    if ((pc && !g_pcScreen) || (xbox && g_pcScreen)) {
        g_pcScreen = pc;
        HLOG(1, "ui: component %s created: the Xbox zoom is %s", name, pc ? "off (PC layout)" : "on");
    }
    F_0041DB1D_orig(c);
}

extern "C" void F_0041CF47_orig(Ctx* c);
// 0x41CF47 CManager::GetUIScale(C2DVector* out): window / 1024x768. The Xbox screens are laid
// out for 640x480, so they need 1.6 times that.
extern "C" void host_ui_scale(Ctx* c) {
    const uint32_t out = rd32(c->esp + 4), ret = rd32(c->esp);
    F_0041CF47_orig(c);
    // CComponent::UpdateZoom, CComponent::ChangePositionDelta (both, 0x52EA30 / 0x52EAF0: a
    // delta is scaled by the UI scale before it moves a component: the picture inside the
    // pause panel's ring was short of its ring by the unzoomed part) and CSprite's size
    // (0x52ECC0): the same 1.6 everywhere the Xbox screens are laid out
    // and CList's own layout (CanMove 0x537776, the view-area pass 0x5379F5: the list's
    // position offsets and view box, FABLE_XBOX_ZOOM_SITES=2). The view-area pass scales a
    // row's size (0x537AA7/ABF) and the view box's size (0x537D74/D92) before
    // C2DBoxF::ContainsPoint decides which rows show (alpha 255) and which hide (alpha 0);
    // with only the rows zoomed the box was 1/1.6 of its height: rows entering a category
    // list were missing and rows scrolled above the box still drew over the screen title.
    static const uint32_t kSites[] = {0x52F88C, 0x52F8AB, 0x52F8CB, 0x52F8E4, 0x52EA8E, 0x52EAA9, 0x52EB4E, 0x52EB69, 0x52ECFE, 0x52ED19,
                                      0x5377D7, 0x5377ED, 0x537872, 0x5378E6, 0x537AA7, 0x537ABF, 0x537D74, 0x537D92};
    static const int tier = [] { const char* e = std::getenv("FABLE_XBOX_ZOOM_SITES"); return e ? std::atoi(e) : 2; }();
    bool zoom = tier >= 3;  // diagnostic: 3 = every caller, -1 = none
    for (size_t i = 0; tier >= 0 && i < (tier >= 2 ? std::size(kSites) : tier == 1 ? 10u : 4u); ++i) zoom = zoom || ret == kSites[i];
    static const bool logCallers = std::getenv("FABLE_XBOX_ZOOM_LOG") != nullptr;
    if (logCallers && xboxMenus() && rd32(0x13B8790) && !g_pcScreen) {
        static std::set<uint32_t> seen;
        if (seen.insert(ret).second) HLOG(1, "ui: GetUIScale from 0x%08X (%s)", ret, zoom ? "zoomed" : "not zoomed");
    }
    if (xboxMenus() && rd32(0x13B8790) && zoom) {
        // The PC screens the game still builds (the quest table, quest start and completion,
        // trading, the experience screen) are laid out for 1024x768 and overflowed 720x480
        // under the 1.6; host_create_component (CManager::CreateComponent, by def name: PC_*
        // versus UI_*) tells which layout is up.
        const bool pcLayout = g_pcScreen;  // set by host_create_component from the def names
        if (!pcLayout) {
            wrf32(out, rdf32(out) * 1.6f);
            wrf32(out + 4, rdf32(out + 4) * 1.6f);
        }
    }
}

// 0x58F649 CTCInventory::OpenPCInventory, from the player's inventory mode. The Xbox build of
// the screen is still there, as CTCInventory's virtual 30 (0x58578B: UI_TOP_LEVEL_MENU_SCREEN,
// the Xbox inventory/hero screens; OpenPCInventory builds PC_TOP_LEVEL_LIST).
extern "C" void F_0058F649_orig(Ctx* c);
namespace { uint32_t g_xboxInventory = 0; }  // the CTCInventory whose Xbox screens are up
// The Xbox inventory screens are on screen: set when they open with the live GUI on (a user
// open, not the one at load), cleared by xboxScreensTick once the live GUI is off.
bool g_xboxScreensUp = false;
extern "C" void host_open_inventory(Ctx* c) {
    const uint32_t inv = c->ecx, xbox = rd32(rd32(inv) + 0x78);
    HLOG(1, "menu: inventory 0x%08X opens %s", inv, xboxMenus() ? "the Xbox screens" : "the PC menu");
    g_xboxInventory = xboxMenus() ? inv : 0;
    if (xboxMenus()) {
        if (liveGui()) { g_xboxScreensUp = true; HLOG(1, "menu: Xbox inventory screens up (live GUI on)"); }
        guestCallThis(xbox, inv, {});
        c->esp += 4;
        return;
    }
    F_0058F649_orig(c);
}

// 0x591E5E CTCInventory::ConstructScreenPC(CComponent* category): the UI manager's select
// handler (0x4200AB) calls this PC virtual when the player GUI's byte +0x2BE is set, and the
// Xbox one (virtual +0x134, 0x5907A6 -> CTCInventory::GetItemsListUIComponent, 0x590235)
// otherwise. With the Xbox screens up the PC version gets the Xbox category component and
// crashes building the PC list (device log: A on a category); the Xbox builder takes it.
extern "C" void F_00591E5E_orig(Ctx* c);
extern "C" void host_inventory_screen(Ctx* c) {
    const uint32_t inv = c->ecx, category = rd32(c->esp + 4);
    if (g_xboxInventory && inv == g_xboxInventory) {
        HLOG(1, "menu: category 0x%08X selected in the Xbox inventory: the Xbox items list", category);
        guestCallThis(0x590235, inv, {category});
        c->esp += 8;  // ret 4
        return;
    }
    F_00591E5E_orig(c);
}

#ifndef _WIN32
#include <sys/uio.h>
#include <unistd.h>
// A read of guest memory that fails instead of faulting on an unmapped page.
static bool safeRead(uint32_t addr, void* out, size_t n) {
    iovec local{out, n}, remote{reinterpret_cast<void*>(static_cast<uintptr_t>(addr)), n};
    return process_vm_readv(getpid(), &local, 1, &remote, 1, 0) == static_cast<ssize_t>(n);
}
#else
static bool safeRead(uint32_t addr, void* out, size_t n) { std::memcpy(out, gp<const void>(addr), n); return true; }
#endif

// Harness diagnostics: the name of a def by global index, read from the definition
// manager's entry table ([0x13B879C]+0xA4, entries holding the def at +0xC) by looking
// for pointers to printable text in the entry and the def; "" when nothing looks like a name.
static std::string defNameGuess(uint32_t id) {
    const uint32_t mgr = rd32(0x13B879C);
    if (!mgr || id > 0x100000) return "";
    const uint32_t table = rd32(mgr + 0xA4);
    if (!table) return "";
    uint32_t entry = 0;
    if (!safeRead(table + id * 4, &entry, 4) || !entry) return "";
    uint32_t probe[0x18];
    if (!safeRead(entry, probe, sizeof probe)) return "";
    auto text = [](uint32_t p) -> std::string {
        if (p < 0x10000 || p > 0x7FFFFFFF) return "";
        char buf[48];
        if (!safeRead(p, buf, sizeof buf)) return "";  // guest pages are not all mapped
        std::string s;
        for (int i = 0; i < 48 && buf[i]; ++i) { if (buf[i] < 0x20 || buf[i] > 0x7E) return ""; s += buf[i]; }
        return s.size() >= 4 ? s : "";
    };
    std::string out;
    for (uint32_t off = 0; off < 0x20; off += 4) { std::string t = text(rd32(entry + off)); if (!t.empty()) out += " e+" + std::to_string(off) + "=" + t; }
    const uint32_t def = rd32(entry + 0xC);
    if (def && safeRead(def, probe, sizeof probe)) for (uint32_t off = 4; off < 0x60; off += 4) { std::string t = text(rd32(def + off)); if (!t.empty()) out += " d+" + std::to_string(off) + "=" + t; }
    return out;
}

// The Xbox d-pad: CWorld::EAUseQuickAccessItem (0x62ABA0) takes a slot from the event:
// slots below the context-item count go to CTCInventory::UseContextSensitiveItem (0x583A92,
// the dynamic ids at this+0x154: left/right fill with context expressions), the rest to
// CTCInventory::UseQuickAccessItem (0x5840A6, the fixed assignments at this+0x148, which the
// PC exe pre-fills with a potion and three expressions), and -2 (the PC's G key, action 78)
// charges OBJECT_GUILD_SEAL_1. With the Xbox scheme both action 55 and 56 sit on the four
// d-pad directions, so a press reached the pre-filled quick slot and down played an
// expression. The Xbox layout is up: the quick-access menu, down: the Guild Seal, left and
// right: the context items; the wraps apply it when the native pad is on.
[[maybe_unused]] static void chargeGuildSeal(uint32_t inv) {  // kept: the instant use (opens the teleport menu)
    const uint32_t mgr = rd32(0x13B879C);  // CGameDefinitionManager
    if (!mgr) return;
    const uint32_t mem = gcalloc(1, 0x30), str = mem, param = mem + 0x20;
    guestCallThis(0x99EBF0, str, {0x1251B4Cu, 0xFFFFFFFFu});  // CCharString("OBJECT_GUILD_SEAL_1")
    const uint32_t def = guestCallThis(0x9AD410, mgr, {str});  // GetDefGlobalIndexFromName
    guestCallThis(0x99EAE0, str, {});
    wr32(param, 0xFFFFFFFEu);
    HLOG(1, "inventory: d-pad down: Guild Seal (def %u)", def);
    if (def) guestCallThis(rd32(rd32(inv) + 0x84), inv, {def, param});
    gfree(mem);
}
extern "C" void F_005840A6_orig(Ctx* c);
extern "C" void F_00583A92_orig(Ctx* c);
// With the right trigger held the d-pad is the favourites (the controller help screen:
// "Access Favourites (D-pad)" under "Favourites (Right Trigger)"): the four fixed
// quick-access assignments, used as the game's UseQuickAccessItem does.
extern "C" void host_use_quick_item(Ctx* c) {
    const uint32_t inv = c->ecx, slot = rd32(c->esp + 4);
    if (host::pad::nativeEnabled() && host::pad::rightTriggerHeld()) { HLOG(1, "inventory: d-pad favourite %u (trigger held)", slot); F_005840A6_orig(c); return; }
    if (host::pad::nativeEnabled() && slot < 4) {
        if (slot == 0) { HLOG(1, "inventory: d-pad up: quick-access menu"); guestCallThis(0x583BD5, inv, {}); c->esp += 8; return; }
        // Down: nothing here; it is CHARGE_GUILD_SEAL (78) alone and the game charges the seal
        // while it is held (the instant use through the -2 path opened the teleport menu at
        // once). Left/right: the context item of the moment, or nothing (the fixed assignment
        // underneath is the PC's default expression, not an Xbox context).
        if (slot == 1) { c->esp += 8; return; }
        const uint32_t cb = rd32(inv + 0x154), ce = rd32(inv + 0x158);
        if (ce >= cb + (slot + 1) * 4 && static_cast<int32_t>(rd32(cb + slot * 4)) > 0) {
            HLOG(1, "inventory: d-pad slot %u: context item %u", slot, rd32(cb + slot * 4));
            guestCallThis(0x583A92, inv, {slot});
        } else {
            HLOG(1, "inventory: d-pad slot %u: no context item", slot);
        }
        c->esp += 8;
        return;
    }
    F_005840A6_orig(c);
}
// D-pad down on the Xbox scheme: the Guild Seal (action 78, charged while held, as G on the
// PC), except when an NPC's follow/wait command is among the context items: the press uses
// that instead (OBJECT_EXPRESSION_FOLLOW_DUMMY / _WAIT_DUMMY, as F2-F4 offer it on the PC).
// The inventory is the one the HUD's quick-access icons last drew.
static uint32_t g_hudInv = 0;
static uint32_t defIndexByName(const char* name) {
    const uint32_t mgr = rd32(0x13B879C);  // CGameDefinitionManager
    if (!mgr) return 0;
    const size_t n = std::strlen(name) + 1;
    const uint32_t mem = gcalloc(1, 0x20 + static_cast<uint32_t>(n)), str = mem, text = mem + 0x10;
    std::memcpy(gp(text), name, n);
    guestCallThis(0x99EBF0, str, {text, 0xFFFFFFFFu});  // CCharString(const char*, -1)
    const uint32_t def = guestCallThis(0x9AD410, mgr, {str});  // GetDefGlobalIndexFromName
    guestCallThis(0x99EAE0, str, {});
    gfree(mem);
    return def;
}
namespace host::menu {
bool npcCommandPress() {
    const uint32_t inv = g_hudInv;
    if (!inv || !rd32(0x13B8790)) return false;
    static uint32_t follow, wait;
    if (!follow) follow = defIndexByName("OBJECT_EXPRESSION_FOLLOW_DUMMY"), wait = defIndexByName("OBJECT_EXPRESSION_WAIT_DUMMY");
    if (!follow && !wait) return false;
    const uint32_t cb = rd32(inv + 0x154), ce = rd32(inv + 0x158);
    for (uint32_t slot = 0; slot < 8 && cb + (slot + 1) * 4 <= ce; ++slot) {
        const uint32_t id = rd32(cb + slot * 4);
        if (id && (id == follow || id == wait)) {
            HLOG(1, "inventory: d-pad down: NPC %s (context slot %u)", id == follow ? "follow" : "wait", slot);
            guestCallThis(0x583A92, inv, {slot});
            return true;
        }
    }
    const uint32_t qb = rd32(inv + 0x148), qe = rd32(inv + 0x14C);
    for (uint32_t i = 0; i < 16 && qb + (i + 1) * 4 <= qe; ++i) {
        const uint32_t id = rd32(qb + i * 4);
        if (id && (id == follow || id == wait)) HLOG(1, "inventory: d-pad down: NPC %s offered at +0x148[%u] only", id == follow ? "follow" : "wait", i);
    }
    return false;
}
}  // namespace host::menu
extern "C" void host_use_context_item(Ctx* c) {
    const uint32_t inv = c->ecx, slot = rd32(c->esp + 4);
    if (host::pad::nativeEnabled() && host::pad::rightTriggerHeld() && slot < 4) {
        HLOG(1, "inventory: d-pad favourite %u (trigger held)", slot);
        guestCallThis(0x5840A6, inv, {slot});  // through the wrap's original: the fixed assignment
        c->esp += 8;
        return;
    }
    if (host::pad::nativeEnabled()) {
        if (slot == 0) { HLOG(1, "inventory: d-pad up: quick-access menu"); guestCallThis(0x583BD5, inv, {}); c->esp += 8; return; }
    }
    F_00583A92_orig(c);
}

// 0x581E33 int CTCInventory::GetQuickAccessItem(int index): index into the quick-access
// item ids at this+0x154 and, past their end, into the context-item ids at this+0x148 with
// no upper bound. CDrawQuickAccessIcons::Draw (0x652FA4) forms the index from the PC GUI
// def's icon counts (nine quick slots), so with the Xbox scheme's four quick-access slots
// the context lookups run past the four context ids and the garbage id crashed GetDef.
// The wrap answers 0 (no item) past the end of either vector.
extern "C" void host_quick_access_item(Ctx* c) {
    const uint32_t inv = c->ecx;
    uint32_t index = rd32(c->esp + 4);
    const uint32_t qb = rd32(inv + 0x154), qe = rd32(inv + 0x158), cb = rd32(inv + 0x148), ce = rd32(inv + 0x14C);
    const uint32_t qn = qe >= qb ? (qe - qb) / 4 : 0, cn = ce >= cb ? (ce - cb) / 4 : 0;
    uint32_t id = 0;
    if (index < qn) id = rd32(qb + index * 4);
    else if (index - qn < cn) id = rd32(cb + (index - qn) * 4);
    static int32_t lastQ[16];
    if (index < 16 && lastQ[index] != static_cast<int32_t>(id)) { lastQ[index] = static_cast<int32_t>(id); HLOG(1, "inventory: quick-access index %u (quick %u, context %u): id %u%s", index, qn, cn, id, defNameGuess(id).c_str()); }
    c->eax = id;
    c->esp += 8;  // ret 4
}

// 0x581FA4 int CTCInventory::GetContextSensitiveItemGraphicIndex(int slot): the id at
// [slot] of the context-item id vector at this+0x148, resolved through GetEngineGraphic
// (0x5D96A0). CDrawQuickAccessIcons::Draw (0x652FA4) asks for every icon of the player
// GUI's context-item list; after a d-pad context-item press the lookup read a bad id and
// crashed in GetPDefFromGlobalIndex. The wrap logs the slot, the vector and the id, and
// answers 0 (no graphic) for a slot past the vector or an id that cannot be a def index.
extern "C" void F_00581FA4_orig(Ctx* c);
extern "C" void host_csi_graphic(Ctx* c) {
    const uint32_t inv = c->ecx, slot = rd32(c->esp + 4);
    const uint32_t begin = rd32(inv + 0x148), end = rd32(inv + 0x14C);
    const uint32_t count = end >= begin ? (end - begin) / 4 : 0;
    const int32_t id = slot < count ? static_cast<int32_t>(rd32(begin + slot * 4)) : 0;
    g_hudInv = inv;
    static int32_t lastId[16];
    static uint32_t lastCount = 0xFFFFFFFF;
    if (slot < 16 && (lastId[slot] != id || lastCount != count)) {  // once per change, not per frame
        lastId[slot] = id, lastCount = count;
        HLOG(1, "inventory: context item graphic for slot %u of %u: id %d%s", slot, count, id, defNameGuess(static_cast<uint32_t>(id)).c_str());
    }
    if (slot >= count || id <= 0 || id > 0x100000) {
        if (slot < count && id > 0) HLOG(1, "inventory: context item slot %u has a bad id %d: no graphic", slot, id);
        c->eax = 0;
        c->esp += 8;  // ret 4
        return;
    }
    F_00581FA4_orig(c);
}

// 0x47E196 CTCInGameMenu::SaveGame(int slot): after writing the files it re-registers the PC
// mouse pointer component at this+0xEC (UI_MOUSE_POINTER, set only by OpenInGameMenuPC,
// 0x4804DE) with the UI manager as an observer (its CObserver part at +4:
// CObservable::RemoveObserver / AddObserver, 0x55C9C0 / 0x55CA90) and sends it ObserveEvent
// (the part's virtual 3) with the slot, with no null check. The PC build never saves from the
// Xbox pause menu, which leaves that member null (device crash on Hero Save). A stand-in
// observer whose virtuals are host no-ops with the interface's argument counts takes the
// calls (the manager notifies it while the save runs); afterwards it is removed from the
// manager again and the member is null as before.
namespace {
uint32_t g_saveStandIn = 0;
uint32_t g_closeMenuAfterSave = 0;  // the CTCInGameMenu to close at the next input update
// NUISystem::CObserver (vtable 0x1245DBC): 0 notify(CObservable*, EEvent), 1 deleting dtor(flags),
// 2/4/7 one argument, 3 ObserveEvent(EEvent), 5 detach (from RemoveObserver), 6 ObserveAllEvents
constexpr uint32_t kObserverArgs[8] = {2, 1, 1, 1, 1, 0, 0, 1};
void standInVirtual(Ctx* c, uintptr_t nargs) { retStd(c, 0, static_cast<int>(nargs)); }
}  // namespace
extern "C" void F_0047E196_orig(Ctx* c);
extern "C" void host_save_game(Ctx* c) {
    const uint32_t menu = c->ecx;
    const bool standIn = menu && rd32(menu + 0xEC) == 0;
    if (standIn) {
        if (!g_saveStandIn) {
            const uint32_t vt = gcalloc(8, 4), obj = gcalloc(2, 4);
            for (int i = 0; i < 8; ++i) wr32(vt + 4 * i, addTrap("CTCInGameMenu PC menu stand-in", standInVirtual, kObserverArgs[i]));
            wr32(obj + 4, vt);  // the observer part at +4 holds the vtable
            g_saveStandIn = obj;
        }
        wr32(menu + 0xEC, g_saveStandIn);
        HLOG(1, "menu: SaveGame(%d) from the Xbox pause menu: PC menu stand-in installed", static_cast<int>(rd32(c->esp + 4)));
    }
    F_0047E196_orig(c);
    if (standIn && rd32(menu + 0xEC) == g_saveStandIn) {
        if (const uint32_t manager = rd32(0x13B8710)) guestCallThis(0x55C9C0, manager, {g_saveStandIn + 4});  // CObservable::RemoveObserver
        wr32(menu + 0xEC, 0);
        // the PC flow leaves the save screen through its mouse-pointer notification; without
        // it the Xbox list stays open but dead (only Start left it): close the menu as Start
        // does (CloseInGameMenu, 0x47D78B), on the next input update, outside the UI update
        g_closeMenuAfterSave = menu;
    }
}

void host::pad::menuAfterSaveTick() {
    const uint32_t menu = g_closeMenuAfterSave;
    if (!menu) return;
    g_closeMenuAfterSave = 0;
    // CloseInGameMenuViaGameEvent (0x47D21C): the game event that closes the menu and takes
    // the game out of pause (CloseInGameMenu alone left it paused, user report)
    HLOG(1, "menu: closing the pause menu after the hero save");
    guestCallThis(0x47D21C, menu, {});
}

// 0x54EF00 NUISystem::CText's draw (virtual 2). The front end's labels are anchored a fixed
// (unscaled) 9-10 px below the top edge of their 29-px (design, 640x480) bars while the bars
// scale with the screen height and the glyphs do not (20 px lines at any size): measured
// 720x480: bar 257..286, caps 267..283 (3.5 px low); 1024x768: bar 411..458, caps 420..441
// (4 px high). The label is moved to the bar's centre: dy = (29 sy - 16) / 2 - 10, sy = the
// front end's screen height / 480. Front end only (0x13B8790, the in-game GUI, is null);
// FABLE_TEXT_CENTER=0 keeps the game's placement; FABLE_UI_TRACE=1 dumps the components.
bool uiTrace() { static const bool on = std::getenv("FABLE_UI_TRACE") != nullptr; return on; }
extern "C" void F_0054EF00_orig(Ctx* c);
extern "C" void host_text_draw(Ctx* c) {
    static const bool centre = [] { const char* e = std::getenv("FABLE_TEXT_CENTER"); return !e || std::atoi(e) != 0; }();
    const uint32_t self = c->ecx;
    if (uiTrace() && self) {
        static std::set<uint32_t> seen;
        if (seen.size() < 80 && seen.insert(self).second) {
            std::string f;
            for (uint32_t off = 0x30; off <= 0x110; off += 4) {
                char b[40];
                std::snprintf(b, sizeof b, " %X:%.3g", off, rdf32(self + off));
                f += b;
            }
            HLOG(1, "text: 0x%08X%s", self, f.c_str());
        }
    }
    if (!centre || !self || rd32(0x13B8790) != 0) { F_0054EF00_orig(c); return; }
    const float sy = rd8(0x13B8768) ? rdf32(0x13B8770) / 480.0f : 1.0f;
    const float dy = (29.0f * sy - 16.0f) * 0.5f - 10.0f;
    static bool once;
    if (!once) { once = true; HLOG(1, "text: front-end labels moved by %.1f px (screen height %.0f)", dy, sy * 480.0f); }
    // the draw anchors the glyphs at this+0xF8/+0xFC (the component's screen position)
    const uint32_t y0 = rd32(self + 0xFC);
    wrf32(self + 0xFC, rdf32(self + 0xFC) + dy);
    F_0054EF00_orig(c);
    wr32(self + 0xFC, y0);
}

// 0x52FFD0 CComponent::UpdatePosition(float dt): its tail computes the render position and
// the draw anchor: for a component whose virtual at +0x198 says it is absolutely placed,
// render = position as given (the list roots, the pictures inside the pause panel's ring),
// otherwise parent position + position x parent zoom. The Xbox screens are 640x480 designs
// zoomed by the screen (1.125 x 1.0 at 720x480, 1.6 at 1024x768) and their absolute
// components must scale from the origin too, or the picture sits where the 640x480 layout
// put it while its ring moved with the zoom (verified: aligned at 640x480 only).
extern "C" void F_0052FFD0_orig(Ctx* c);
// CComponent::Name (+0x24, PDB): a CCharString -> CCharStringData {char*, length}.
static std::string componentName(uint32_t self, uint32_t off = 0x1C) {
    const uint32_t data = rd32(self + off);
    if (data < 0x10000 || data >= kGuestLimit) return "";
    const uint32_t ptr = rd32(data), len = rd32(data + 4);
    if (ptr < 0x10000 || ptr >= kGuestLimit || len == 0 || len > 80) return "";
    std::string n(gp<const char>(ptr), len);
    for (char ch : n) if (ch < 0x20 || ch > 0x7E) return "";
    return n;
}
extern "C" void host_update_position(Ctx* c) {
    const uint32_t self = c->ecx;
    F_0052FFD0_orig(c);
    static const bool on = [] { const char* e = std::getenv("FABLE_XBOX_ABSOLUTE_ZOOM"); return !e || std::atoi(e) != 0; }();
    if (!on || !self || !xboxMenus() || !rd32(0x13B8790)) return;
    static const bool logPos = std::getenv("FABLE_UI_POS") != nullptr;  // diagnostic: each named component once, with its place
    if (logPos && g_xboxScreensUp) {
        std::string name = componentName(self, 0x1C);
        if (name.empty()) name = componentName(self, 0x20);
        if (name.empty()) name = componentName(self, 0x24);
        const float rx = rdf32(self + 0x54), ry = rdf32(self + 0x58);
        char key[64];
        std::snprintf(key, sizeof key, "%08X/%d/%d", rd32(self), static_cast<int>(rx) / 8, static_cast<int>(ry) / 8);
        static std::set<std::string> seen;
        if (seen.size() < 3000 && seen.insert(name.empty() ? std::string(key) : name).second)
            HLOG(1, "ui: pos %s 0x%08X vt 0x%08X render %.1f,%.1f anchor %.1f,%.1f size %.1f,%.1f zoom %.3f,%.3f def %d", name.empty() ? "-" : name.c_str(), self, rd32(self), rx, ry,
                 rdf32(self + 0xF8), rdf32(self + 0xFC), rdf32(self + 0xD4), rdf32(self + 0xD8), rdf32(self + 0x7C), rdf32(self + 0x80), static_cast<int>(rd32(self + 0x150)));
    }
    const uint32_t vt = rd32(self);
    if (!(guestCallThis(rd32(vt + 0x198), self, {}) & 0xFF)) return;  // parented: already zoomed
    const float zx = rdf32(self + 0x7C), zy = rdf32(self + 0x80);  // the component's render zoom
    if (!(zx > 0.01f && zx < 100.0f && zy > 0.01f && zy < 100.0f) || (zx == 1.0f && zy == 1.0f)) return;
    wrf32(self + 0x54, rdf32(self + 0x54) * zx), wrf32(self + 0x58, rdf32(self + 0x58) * zy);
    wrf32(self + 0xF8, rdf32(self + 0xF8) * zx), wrf32(self + 0xFC, rdf32(self + 0xFC) * zy);
}

// 0x533D30 CComponent::GetSize(C2DVector* out): the bounding box of the children. The Xbox
// pause list's entries carry their whole sub-screen as children (the description picture,
// the options sub-list, 770x400 and more), so CList's view-area test (the loop at
// 0x537A1E in vfunc 185: both corners of each child must lie inside the list's 640x480
// ViewArea) fails for every entry but the selected one and hides them (0x537E74..0x537EC3
// sets their alpha to 0 at once). For that test a menu entry's extent is its label (the
// text child), which is what the player sees of it in the list.
// FABLE_UI_DUMP=<seconds>: once, after that long, the component tree the Xbox list belongs
// to (class vtable, position, render position, draw anchor, size, render zoom, def), from
// its root, to see which part is laid out with which zoom.
void dumpTree(uint32_t node, int depth, int& count) {
    if (!node || depth > 12 || count > 3000) return;
    ++count;
    HLOG(1, "ui: %*s0x%08X vt 0x%08X pos %.1f,%.1f render %.1f,%.1f anchor %.1f,%.1f size %.1f,%.1f zoom %.3f,%.3f def %d", depth * 2, "", node, rd32(node),
         rdf32(node + 0x34), rdf32(node + 0x38), rdf32(node + 0x54), rdf32(node + 0x58), rdf32(node + 0xF8), rdf32(node + 0xFC), rdf32(node + 0xD4),
         rdf32(node + 0xD8), rdf32(node + 0x7C), rdf32(node + 0x80), static_cast<int>(rd32(node + 0x150)));
    const uint32_t b = rd32(node + 0xB0), e = rd32(node + 0xB4);
    if (b < e && e - b < 0x4000)
        for (uint32_t p = b; p < e; p += 8) dumpTree(rd32(p), depth + 1, count);
}
void maybeDumpTree(uint32_t entry) {
    static const int dumpAt = [] { const char* e = std::getenv("FABLE_UI_DUMP"); return e ? std::atoi(e) : 0; }();
    static bool dumped = false;
    if (!dumpAt || dumped || GetTickCount() < static_cast<DWORD>(dumpAt) * 1000u) return;
    dumped = true;
    uint32_t root = entry;
    for (int i = 0; i < 16; ++i) { const uint32_t p = rd32(root + 0xC8); if (!p || p >= kGuestLimit) break; root = p; }
    int count = 0;
    dumpTree(root, 0, count);
    HLOG(1, "ui: %d components from root 0x%08X", count, root);
}

extern "C" void F_00533D30_orig(Ctx* c);
extern "C" void host_component_size(Ctx* c) {
    const uint32_t self = c->ecx, ret = rd32(c->esp), out = rd32(c->esp + 4);
    constexpr uint32_t kMenuEntryVt = 0x0124662C, kTextVt = 0x01245DE4;
    constexpr uint32_t kListVt = 0x012462E4;
    if (xboxMenus() && rd32(self) == kMenuEntryVt && (ret == 0x537A81 || ret == 0x537A92)) {
        maybeDumpTree(self);
        // The list shows UpLimit entries above and DownLimit below the selected one (its def,
        // retail +0x138/+0x13C); the ones further out are outside, whatever their label's box.
        // (CList: scrolling children array +0x164..+0x168, selected child index +0x15C.)
        const uint32_t list = rd32(self + 0xC8);
        bool outside = false;
        if (list && rd32(list) == kListVt) {
            const uint32_t scratch = (c->esp - 0x40) & ~0xFu;
            wr32(scratch, 0);
            guestCallThis(rd32(kListVt + 0x1B0), list, {scratch});
            const uint32_t d = rd32(scratch);
            if (d) {
                // The pass runs before the list's selected index (+0x15C) moves, but the
                // children's target positions (vtable +0x1E8) are already laid out: the
                // selected entry sits at the list's position, the others one row apart.
                const int up = (int)rd32(d + 0x138), down = (int)rd32(d + 0x13C);
                guestCallThis(rd32(kListVt + 0x1E8), list, {scratch + 8});
                const float ly = rdf32(scratch + 12);
                float spacing = 0;
                for (uint32_t p = rd32(list + 0x164), end = rd32(list + 0x168); p < end && p; p += 4) {
                    const uint32_t ch = rd32(p);
                    if (!ch || rd32(ch) != kMenuEntryVt) continue;
                    guestCallThis(rd32(kMenuEntryVt + 0x1E8), ch, {scratch + 16});
                    const float dy = std::fabs(rdf32(scratch + 20) - ly);
                    if (dy > 0.5f && (spacing == 0 || dy < spacing)) spacing = dy;
                }
                guestCallThis(rd32(kMenuEntryVt + 0x1E8), self, {scratch + 16});
                const float rows = spacing > 0 ? (rdf32(scratch + 20) - ly) / spacing : 0;
                // The limit-th row is the wheel's alpha-0 one; beyond it the wheel's alpha
                // bounces back up, so those rows are hidden outright: their wheel states
                // (3/4/5, CUIState colour +0x18) get alpha 0 too, as the states are applied
                // after this pass.
                if (up > 0 && down > 0 && (rows < -up + 0.5f || rows > down - 0.5f)) {
                    outside = true;
                    for (uint32_t k = 3; k <= 5; ++k) {
                        const uint32_t st = guestCallThis(rd32(kMenuEntryVt + 0x21C), self, {k});
                        if (st) wr8(st + 0x1B, 0);
                    }
                }
                const uint32_t refs = rd32(d + 4) - 1;
                wr32(d + 4, refs);
                if (!refs) guestCallThis(rd32(rd32(d) + 4), d, {});
            }
        }
        if (outside) {
            wrf32(out, 100000.0f), wrf32(out + 4, 100000.0f);
            retStd(c, out, 1);
            return;
        }
        for (uint32_t p = rd32(self + 0xB0); p < rd32(self + 0xB4) && p; p += 8) {
            const uint32_t ch = rd32(p);
            if (ch && rd32(ch) == kTextVt) {
                guestCallThis(rd32(rd32(ch) + 0x5C), ch, {out});
                retStd(c, out, 1);
                return;
            }
        }
    }
    F_00533D30_orig(c);
}


// 0x41E6D3 NUISystem::CManager vfunc_14, the UI event handler: its actions (a category select at
// 0x4200B0, the description pane at 0x4237A0 / 0x423B65, ...) build PC screen parts when the
// player GUI's live-GUI byte (+0x2BE) is set and Xbox ones otherwise. The host shows the Xbox
// inventory with the live GUI on, so the handler built PC parts into the Xbox screens and
// crashed (Clothing, the description pane: PC_DESCRIPTION_INDEPENDANT). While the Xbox
// inventory screens are up (g_xboxScreensUp) the handler runs with the byte clear; a toggle it
// makes itself (closing the menu) carries over to the real value.
extern "C" void F_0041E6D3_orig(Ctx* c);
extern "C" void host_ui_manager_event(Ctx* c) {
    const uint32_t gui = rd32(0x13B8790);
    if (!g_xboxScreensUp || !gui || !rd8(gui + 0x2BE)) { F_0041E6D3_orig(c); return; }
    wr8(gui + 0x2BE, 0);
    F_0041E6D3_orig(c);
    if (rd32(0x13B8790) == gui) wr8(gui + 0x2BE, rd8(gui + 0x2BE) ? 0 : 1);
}

namespace host::menu {
void xboxScreensTick() {  // once per input update: the Xbox inventory screens end with the live GUI
    static int32_t lastCharge = -1;  // CInputProcessQuickAccessItems' charging slot (-2: the Guild Seal)
    const int32_t charge = static_cast<int32_t>(rd32(0x137EE78));
    if (charge != lastCharge) { lastCharge = charge; HLOG(1, "inventory: quick-access charge slot %d", charge); }
    if (g_xboxScreensUp && !liveGui()) { g_xboxScreensUp = false; HLOG(1, "menu: Xbox inventory screens closed (live GUI off)"); }
}
}  // namespace host::menu
