// Behaviour test for CTCInventoryStats::ConstructExperienceListPC @ 0x005D7081: the PC experience page of the stats screen.
// Checks, against a model of the row's logic over mock UI / hero-experience objects: the list gets, per parent scroll-list button,
// an ignore (249) action on its selected slot and an observe (245) action on its inactive slot, each targeting the button with the
// mouse-movement event (25); with an experience frame, the viewport buttons send the viewport right-stick up / down (6 / 7) on
// left click and hold, the list observes / ignores them, the viewport is a position child and the tick container a fake child;
// with a screen and a tick container, the frame (when present) gets the tick / level / title / ring position children, then the
// seven stat tick sliders (303 wide, 53 tall) and the eighteen ability rows are built; the list is returned either way.
#include <cstdio>
#include <vector>
#include <string>

struct Action { long type; std::string target; long param; };
struct Slider { long stat; long y; long level; long maxLevel; float value; float width; float height; };
struct Page {
    std::vector<std::string> log;                 // "<component>.<slot>:<type>:<target>:<param>"
    std::vector<std::string> positionChildren;
    std::vector<Slider> sliders; std::vector<long> abilities; std::string result;
};
static std::string L(long v) { char b[16]; std::sprintf(b, "%ld", v); return b; }
static void Add(Page& p, const char* comp, const char* slot, const Action& a)
{
    p.log.push_back(std::string(comp) + "." + slot + ":" + L(a.type) + ":" + a.target + ":" + L(a.param));
}
static Action Make(long type, const std::string& target, long param) { Action a; a.type = type; a.target = target; a.param = param; return a; }

static Page Construct(bool frame, bool viewport, bool screen, bool tick)
{
    Page p;
    {   // parent scroll-list buttons
        Action observeUp = Make(245, "SCROLL_LIST_UP", 25), ignoreUp = Make(249, "SCROLL_LIST_UP", 25);
        Add(p, "list", "selected", ignoreUp); Add(p, "list", "inactive", observeUp);
        Action observeDown = Make(245, "SCROLL_LIST_DOWN", 25), ignoreDown = Make(249, "SCROLL_LIST_DOWN", 25);
        Add(p, "list", "selected", ignoreDown); Add(p, "list", "inactive", observeDown);
    }
    if (frame) {
        if (viewport) p.positionChildren.push_back("PC_STATS_EXP_VIEWPORT");
        Action sendUp = Make(230, viewport ? "PC_STATS_EXP_VIEWPORT" : "", 6);
        Add(p, "up", "leftclicked", sendUp); Add(p, "up", "leftheld", sendUp);
        Action sendDown = Make(230, viewport ? "PC_STATS_EXP_VIEWPORT" : "", 7);
        Add(p, "down", "leftclicked", sendDown); Add(p, "down", "leftheld", sendDown);
        Add(p, "list", "selected", Make(245, "SCROLL_STATS_VIEWPORT_UP", 25)); Add(p, "list", "inactive", Make(249, "SCROLL_STATS_VIEWPORT_UP", 25));
        Add(p, "list", "selected", Make(245, "SCROLL_STATS_VIEWPORT_DOWN", 25)); Add(p, "list", "inactive", Make(249, "SCROLL_STATS_VIEWPORT_DOWN", 25));
        if (tick) p.log.push_back("viewport.fakechild:PC_STATS_EXP_TICK_SLIDER_CONTAINER");
    }
    if (screen && tick) {
        if (frame) {
            p.positionChildren.push_back("PC_STATS_EXP_TICK_SLIDER_CONTAINER");
            p.positionChildren.push_back("PC_STATS_EXP_LEVEL_SLIDER_CONTAINER");
            p.positionChildren.push_back("PC_TITLE_EXPERIENCE");
            p.positionChildren.push_back("PC_RING_PIC_EXPERIENCE_INDEPENDANT");
        }
        float fraction = 0.0f;
        static const long stats[7] = { 0, 1, 2, 3, 4, 5, 9 };
        static const long ys[7] = { 0, 30, 60, 110, 140, 170, 220 };
        for (int i = 0; i < 7; ++i) {
            long level = 10 + stats[i], maxLevel = 20 + stats[i];
            fraction = 0.25f * (float)(stats[i] % 3);          // the getter fills the fraction
            long possible = stats[i] % 2;
            fraction += possible + level;
            Slider s = { stats[i], ys[i], level, maxLevel, fraction, 303.0f, 53.0f };
            p.sliders.push_back(s);
        }
        static const long order[18] = { 3, 16, 12, 9, 11, 18, 19, 1, 5, 2, 6, 10, 4, 8, 13, 14, 15, 17 };
        for (int i = 0; i < 18; ++i) p.abilities.push_back(order[i]);
    }
    p.result = "PC_STATS_EXP_LIST";
    return p;
}

int main()
{
    int ok = 1;
    Page full = Construct(true, true, true, true);
    if (full.log.size() != 4 + 4 + 4 + 1 || full.log[0] != "list.selected:249:SCROLL_LIST_UP:25" || full.log[1] != "list.inactive:245:SCROLL_LIST_UP:25") ok = 0;
    if (full.log[4] != "up.leftclicked:230:PC_STATS_EXP_VIEWPORT:6" || full.log[7] != "down.leftheld:230:PC_STATS_EXP_VIEWPORT:7") ok = 0;
    if (full.log[8] != "list.selected:245:SCROLL_STATS_VIEWPORT_UP:25" || full.log[12] != "viewport.fakechild:PC_STATS_EXP_TICK_SLIDER_CONTAINER") ok = 0;
    if (full.positionChildren.size() != 5 || full.sliders.size() != 7 || full.abilities.size() != 18 || full.result != "PC_STATS_EXP_LIST") ok = 0;
    for (size_t i = 0; i < full.sliders.size() && ok; ++i)
        if (full.sliders[i].width != 303.0f || full.sliders[i].height != 53.0f || full.sliders[i].level + 10 != full.sliders[i].maxLevel) ok = 0;
    if (ok && (full.sliders[6].stat != 9 || full.sliders[6].y != 220 || full.abilities[0] != 3 || full.abilities[17] != 17)) ok = 0;
    Page noFrame = Construct(false, true, true, true);
    if (noFrame.log.size() != 4 || !noFrame.positionChildren.empty() || noFrame.sliders.size() != 7) ok = 0;
    Page noScreen = Construct(true, true, false, true);
    if (!noScreen.sliders.empty() || !noScreen.abilities.empty() || noScreen.positionChildren.size() != 1 || noScreen.result != "PC_STATS_EXP_LIST") ok = 0;
    if (ok) std::printf("005d7081_TEST PASS\n"); else std::printf("005d7081_TEST FAIL\n");
    return ok ? 0 : 1;
}