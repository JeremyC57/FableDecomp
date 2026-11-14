// Behaviour test for 0x00598A1C NUISystem::CFrontEndManager::BuildComponents: the media-player error screen (and its message) exists only
// when asked; the main-menu and live-aware entries are null, the other screens are created; the press-start screen gets the go-to-main-menu
// click action; resolution entries read "<w>X<h>X<bpp>", anti-aliasing entries "<n>X" or the OFF text (none, 1 and unknown levels); only
// non-null screens are initialised; a detail slider with Min == Max and a grandparent (or a text slider with no MaxChild and a parent) has
// both arrows halved in alpha and its text made unavailable; refresh rates get " Hz"; the first screen is the main menu after a quit from
// the game (with the profile loaded), else the error screen when asked, else press-start.
#include <cstdio>
#include <string>
#include <map>
#include <vector>
struct Comp { std::string name, value; bool initialised, unavailable; int alpha; bool preventObserving; int action; Comp() : initialised(false), unavailable(false), alpha(255), preventObserving(false), action(0) {} };
struct Slider { float minX, maxX; bool hasParent, hasGrand; int maxChild; Comp left, right, text; };
struct Mgr {
    std::map<int, Comp*> screens; std::vector<Comp*> owned; bool quitFromGame; std::string loaded; int first; std::string error;
    Comp* Create(const char* n) { Comp* c = new Comp; c->name = n; owned.push_back(c); return c; }
    ~Mgr() { for (size_t i = 0; i < owned.size(); ++i) delete owned[i]; }
};
static std::string Int(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static std::string AAText(int level) { if (level >= 2 && level <= 16) return Int(level) + "X"; return "TEXT_GUI_MENU_OFF"; }
static void Grey(Comp& left, Comp& right, Comp& text) {
    left.preventObserving = true; left.alpha /= 2; right.preventObserving = true; right.alpha /= 2; text.unavailable = true;
}
static void Build(Mgr& m, bool showError, const std::vector<long>& modes, const std::vector<int>& aa, std::vector<std::string>& res,
                  std::vector<std::string>& aaOut, Slider& detail, Slider& textSlider, std::vector<std::string>& rates) {
    if (showError) { m.screens[-1] = m.Create("UI_FRONTEND_MEDIA_PLAYER_ERROR"); m.screens[-1]->value = "ERR\n\nCODE " + m.error; }
    m.screens[0] = 0;
    const int keys[] = { 20, 7, 10, 8, 1, 4 };
    for (int i = 0; i < 6; ++i) m.screens[keys[i]] = m.Create("S");
    m.screens[2] = 0;
    const int keys2[] = { 3, 9, 19, 12, 15, 16 };
    for (int i = 0; i < 6; ++i) m.screens[keys2[i]] = m.Create("S");
    m.screens[20]->action = 229;
    const int keys3[] = { 21, 22, 5, 23, 24, 25, 27, 26, 28 };
    for (int i = 0; i < 9; ++i) m.screens[keys3[i]] = m.Create("S");
    for (size_t i = 0; i + 2 < modes.size() + 1; i += 3) res.push_back(Int(modes[i]) + "X" + Int(modes[i + 1]) + "X" + Int(modes[i + 2]));
    for (size_t i = 0; i < aa.size(); ++i) aaOut.push_back(AAText(aa[i]));
    for (std::map<int, Comp*>::iterator it = m.screens.begin(); it != m.screens.end(); ++it) if (it->second) it->second->initialised = true;
    if (detail.minX == detail.maxX && detail.hasParent && detail.hasGrand) Grey(detail.left, detail.right, detail.text);
    if (textSlider.maxChild == 0 && textSlider.hasParent) Grey(textSlider.left, textSlider.right, textSlider.text);
    for (size_t i = 0; i < rates.size(); ++i) rates[i] += " Hz";
    if (!m.quitFromGame) m.first = showError ? -1 : 20; else { m.loaded = "profile"; m.first = 0; }
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    std::vector<long> modes; modes.push_back(800); modes.push_back(600); modes.push_back(32);
    std::vector<int> aa; aa.push_back(0); aa.push_back(1); aa.push_back(2); aa.push_back(16); aa.push_back(17);
    {
        Mgr m; m.quitFromGame = false; m.error = "E42"; std::vector<std::string> res, aaOut, rates; rates.push_back("60");
        Slider d = { 1.0f, 1.0f, true, true, 0 }, t = { 0, 0, true, false, 3 };
        Build(m, true, modes, aa, res, aaOut, d, t, rates);
        check(m.screens.count(-1) && m.screens[-1]->value == "ERR\n\nCODE E42", "error screen and message");
        check(m.screens[0] == 0 && m.screens[2] == 0 && m.screens[28] != 0 && m.screens.size() == 24, "screen table");
        check(m.screens[20]->action == 229, "press-start action");
        check(res.size() == 1 && res[0] == "800X600X32", "resolution text");
        check(aaOut[0] == "TEXT_GUI_MENU_OFF" && aaOut[1] == "TEXT_GUI_MENU_OFF" && aaOut[2] == "2X" && aaOut[3] == "16X" && aaOut[4] == "TEXT_GUI_MENU_OFF", "AA texts");
        check(m.screens[5]->initialised && m.screens[-1]->initialised, "screens initialised");
        check(d.left.alpha == 127 && d.right.preventObserving && d.text.unavailable, "flat detail slider greyed");
        check(!t.text.unavailable && t.left.alpha == 255, "text slider with entries untouched");
        check(rates[0] == "60 Hz", "refresh rate suffix");
        check(m.first == -1 && m.loaded == "", "error screen first");
    }
    {
        Mgr m; m.quitFromGame = false; std::vector<std::string> res, aaOut, rates;
        Slider d = { 0.0f, 1.0f, true, true, 0 }, t = { 0, 0, true, false, 0 };
        Build(m, false, modes, aa, res, aaOut, d, t, rates);
        check(!m.screens.count(-1) && m.first == 20, "no error screen: press start first");
        check(!d.text.unavailable && t.text.unavailable && t.right.alpha == 127, "ranged slider kept, empty text slider greyed");
    }
    {
        Mgr m; m.quitFromGame = true; std::vector<std::string> res, aaOut, rates;
        Slider d = { 1.0f, 1.0f, true, false, 0 }, t = { 0, 0, false, false, 0 };
        Build(m, true, modes, aa, res, aaOut, d, t, rates);
        check(m.first == 0 && m.loaded == "profile", "quit from game: main menu with the profile loaded");
        check(!d.text.unavailable && !t.text.unavailable, "no grandparent / no parent: untouched");
    }
    if (!fails) std::printf("00598a1c_TEST PASS\n");
    return fails;
}
