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
#include <vector>

using namespace host;

extern "C" void F_0055CB10_orig(Ctx* c);

namespace {

constexpr uint32_t kGetUiManager = 0x41E5F2;      // CFrontEndManager::GetInstance
constexpr uint32_t kManagerDispatch = 0x55CB10;   // CManager::ProcessEvent (wrapped)
constexpr uint32_t kClickableProcess = 0x55AD60;  // CClickable::ProcessEvent (CNavButton)
constexpr uint32_t kFrontEndButtonProcess = 0x54DBC0;

constexpr uint32_t kUiManager = 0x13B8710;      // its instance (0 before the first GetInstance)
constexpr uint32_t kDefAction = 228;             // CUIDef::Action (the Ego_r PDB has it at 224)
constexpr uint32_t kActionBack = 86;             // the action of Back / Cancel / No buttons

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
    if (xboxMenus() && ev == EV_LEFT_PRESS && rd32(c->esp) == 0x68A291) wr32(c->esp + 4, EV_SELECT);
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
    if (ev <= EV_BACK && !liveGui() && mgr == guestCall(kGetUiManager, {})) {
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

extern "C" void F_0041CF47_orig(Ctx* c);
// 0x41CF47 CManager::GetUIScale(C2DVector* out): window / 1024x768. The Xbox screens are laid
// out for 640x480, so they need 1.6 times that.
extern "C" void host_ui_scale(Ctx* c) {
    const uint32_t out = rd32(c->esp + 4), ret = rd32(c->esp);
    F_0041CF47_orig(c);
    const bool zoom = ret == 0x52F88C || ret == 0x52F8AB || ret == 0x52F8CB || ret == 0x52F8E4;  // CComponent::UpdateZoom
    if (xboxMenus() && rd32(0x13B8790) && zoom) {
        wrf32(out, rdf32(out) * 1.6f);
        wrf32(out + 4, rdf32(out + 4) * 1.6f);
    }
}

// 0x58F649 CTCInventory::OpenPCInventory, from the player's inventory mode. The Xbox build of
// the screen is still there, as CTCInventory's virtual 30 (0x58578B: UI_TOP_LEVEL_MENU_SCREEN,
// the Xbox inventory/hero screens; OpenPCInventory builds PC_TOP_LEVEL_LIST).
extern "C" void F_0058F649_orig(Ctx* c);
extern "C" void host_open_inventory(Ctx* c) {
    const uint32_t inv = c->ecx, xbox = rd32(rd32(inv) + 0x78);
    HLOG(1, "menu: inventory 0x%08X opens %s", inv, xboxMenus() ? "the Xbox screens" : "the PC menu");
    if (xboxMenus()) {
        guestCallThis(xbox, inv, {});
        c->esp += 4;
        return;
    }
    F_0058F649_orig(c);
}

// 0x533D30 CComponent::GetSize(C2DVector* out): the bounding box of the children. The Xbox
// pause list's entries carry their whole sub-screen as children (the description picture,
// the options sub-list, 770x400 and more), so CList's view-area test (the loop at
// 0x537A1E in vfunc 185: both corners of each child must lie inside the list's 640x480
// ViewArea) fails for every entry but the selected one and hides them (0x537E74..0x537EC3
// sets their alpha to 0 at once). For that test a menu entry's extent is its label (the
// text child), which is what the player sees of it in the list.
extern "C" void F_00533D30_orig(Ctx* c);
extern "C" void host_component_size(Ctx* c) {
    const uint32_t self = c->ecx, ret = rd32(c->esp), out = rd32(c->esp + 4);
    constexpr uint32_t kMenuEntryVt = 0x0124662C, kTextVt = 0x01245DE4;
    constexpr uint32_t kListVt = 0x012462E4;
    if (xboxMenus() && rd32(self) == kMenuEntryVt && (ret == 0x537A81 || ret == 0x537A92)) {
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
