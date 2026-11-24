// Behaviour test for CTCInventoryStats::ConstructExperienceList @ 0x005D6748: the console experience page of the stats screen.
// Checks, against a model of the row's logic over mock UI / hero-experience objects: the back action on the dummy entry (type
// CHANGESTATE 3, the top-level menu screen's action-param base or null, then state 1) and the ring picture are only added when the
// dummy entry exists; the seven stat tick sliders (stats 0,1,2,3,4,5,9 at y 0/30/60/110/140/170/220, red / yellow / blue, value =
// level + possible level-ups + the getter's fraction, level and max level as the long arguments, 270 wide, 20 tall) and the
// eighteen ability rows (three groups, the row position advanced 30 per row by AddHeroAbility and 15 extra between groups) are
// only built when the screen and its tick-slider container exist; the list component is returned either way.
#include <cstdio>
#include <vector>
#include <string>

struct Colour { float r, g, b, a; };
static Colour MakeColour(float r, float g, float b, float a) { Colour c; c.r = r; c.g = g; c.b = b; c.a = a; return c; }
struct Slider { long stat; long y; long level; long maxLevel; Colour colour; float value; float width; float height; };
struct Ability { long ability; float y; };

struct MockHeroExperience {
    long Level(long stat) const { return 10 + stat; }
    long MaxLevel(long stat) const { return 20 + stat; }
    long PossibleLevelUp(long stat, float& fraction) const { fraction = 0.25f * (float)(stat % 3); return stat % 2; }
};

struct Page {
    bool dummyEntry, screen, tickContainer;
    long backActionType; bool backActionAdded; bool backParamIsScreen; long backState; bool ringPicAdded;
    std::vector<Slider> sliders; std::vector<Ability> abilities; std::string result;
};

// the model of the row (same control flow and constants as rebuild src/005d6748.cpp)
static Page Construct(bool hasTopScreen, bool dummyEntry, bool screen, bool tickContainer)
{
    MockHeroExperience hero;
    Page p = { dummyEntry, screen, tickContainer, 3, false, false, 0, false };
    p.backParamIsScreen = hasTopScreen;        // UIActionParams.push_back(parent->FindChild("UI_TOP_LEVEL_MENU_SCREEN")): null when missing
    p.backState = 1;                            // new CActionParamState(1)
    if (dummyEntry) { p.backActionAdded = true; p.ringPicAdded = true; }
    if (screen && tickContainer) {
        float fraction = 0.0f;
        static const long stats[7] = { 0, 1, 2, 3, 4, 5, 9 };
        static const long ys[7] = { 0, 30, 60, 110, 140, 170, 220 };
        for (int i = 0; i < 7; ++i) {
            long level = hero.Level(stats[i]);
            long maxLevel = hero.MaxLevel(stats[i]);
            long possibleLevels = hero.PossibleLevelUp(stats[i], fraction);
            fraction += possibleLevels + level;
            Colour c = i < 3 ? MakeColour(0.7f, 0.0f, 0.0f, 1.0f) : i < 6 ? MakeColour(0.7f, 0.7f, 0.0f, 1.0f) : MakeColour(0.0f, 0.0f, 0.7f, 1.0f);
            Slider s = { stats[i], ys[i], level, maxLevel, c, fraction, 270.0f, 20.0f };
            p.sliders.push_back(s);
        }
        {
            float yPos = 270.0f;
            static const long group1[7] = { 3, 16, 12, 9, 11, 18, 19 }, group2[5] = { 1, 5, 2, 6, 10 }, group3[6] = { 4, 8, 13, 14, 15, 17 };
            for (int i = 0; i < 7; ++i) { Ability a = { group1[i], yPos }; p.abilities.push_back(a); yPos += 30.0f; }   // AddHeroAbility: y, then += 30
            yPos += 15.0f;
            for (int i = 0; i < 5; ++i) { Ability a = { group2[i], yPos }; p.abilities.push_back(a); yPos += 30.0f; }
            yPos += 15.0f;
            for (int i = 0; i < 6; ++i) { Ability a = { group3[i], yPos }; p.abilities.push_back(a); yPos += 30.0f; }
        }
    }
    p.result = "UI_STATS_EXP_LIST";
    return p;
}

static bool Near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }

int main()
{
    int ok = 1;
    Page full = Construct(true, true, true, true);
    if (full.backActionType != 3 || !full.backActionAdded || !full.backParamIsScreen || full.backState != 1 || !full.ringPicAdded) ok = 0;
    if (full.sliders.size() != 7 || full.abilities.size() != 18 || full.result != "UI_STATS_EXP_LIST") ok = 0;
    static const long stats[7] = { 0, 1, 2, 3, 4, 5, 9 };
    static const long ys[7] = { 0, 30, 60, 110, 140, 170, 220 };
    for (int i = 0; i < 7 && ok; ++i) {
        const Slider& s = full.sliders[i];
        float fraction = 0.25f * (float)(stats[i] % 3);
        float value = (float)((10 + stats[i]) + (stats[i] % 2)) + fraction;
        if (s.stat != stats[i] || s.y != ys[i] || s.level != 10 + stats[i] || s.maxLevel != 20 + stats[i]) ok = 0;
        if (!Near(s.value, value) || !Near(s.width, 270.0f) || !Near(s.height, 20.0f)) ok = 0;
        Colour want = i < 3 ? MakeColour(0.7f, 0.0f, 0.0f, 1.0f) : i < 6 ? MakeColour(0.7f, 0.7f, 0.0f, 1.0f) : MakeColour(0.0f, 0.0f, 0.7f, 1.0f);
        if (!Near(s.colour.r, want.r) || !Near(s.colour.g, want.g) || !Near(s.colour.b, want.b) || !Near(s.colour.a, want.a)) ok = 0;
    }
    static const long order[18] = { 3, 16, 12, 9, 11, 18, 19, 1, 5, 2, 6, 10, 4, 8, 13, 14, 15, 17 };
    float y = 270.0f;
    for (int i = 0; i < 18 && ok; ++i) {
        if (i == 7 || i == 12) y += 15.0f;
        if (full.abilities[i].ability != order[i] || !Near(full.abilities[i].y, y)) ok = 0;
        y += 30.0f;
    }
    if (ok && !Near(y, 270.0f + 18 * 30.0f + 30.0f)) ok = 0;                    // 840: three groups, two 15-point gaps
    Page noDummy = Construct(false, false, true, true);
    if (noDummy.backActionAdded || noDummy.ringPicAdded || noDummy.backParamIsScreen || noDummy.sliders.size() != 7) ok = 0;
    Page noScreen = Construct(true, true, false, true);
    if (!noScreen.sliders.empty() || !noScreen.abilities.empty() || noScreen.result != "UI_STATS_EXP_LIST") ok = 0;
    Page noTicks = Construct(true, true, true, false);
    if (!noTicks.sliders.empty() || !noTicks.abilities.empty() || !noTicks.ringPicAdded) ok = 0;
    if (ok) std::printf("005d6748_TEST PASS\n"); else std::printf("005d6748_TEST FAIL\n");
    return ok ? 0 : 1;
}