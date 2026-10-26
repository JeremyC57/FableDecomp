// Behaviour test for CTCWife::HaveSex (0x006CF920): joins the global update list, starts a black fade with the def's
// wedding fade-out/fade-in times and hold -1, sends a pause event (type 9) for the current player, sets
// WaitingForFadeOut and WaitingForSex; a live hero's sex count goes up by one, the hero is healed by its max health
// (show effect) and the scoreboard gets activity 6 with the new count; a dead or missing hero is left alone.
#include <cstdio>

struct Hero {
    bool dead;
    bool hasStats;
    long timesHadSex;
    float maxHealth;
    float healed;
    int healCalls;
    int scoreActivity;
    float scoreValue;
};
static Hero* g_hero = 0;
static int g_updateList = 0, g_events = 0, g_eventType = 0, g_fades = 0;
static float g_fadeOut = 0, g_fadeIn = 0, g_hold = 0;
static unsigned char g_rgba[4];

class CHeroMarriageDef { public: float WeddingTimeForScreenToFadeOut, WeddingTimeForScreenToFadeIn; };
class CTCWife { public:
    const CHeroMarriageDef* PHeroMarriageDef;
    bool WaitingForFadeOut, WaitingForSex;
    void HaveSex()
    {
        ++g_updateList;
        ++g_fades; g_fadeOut = PHeroMarriageDef->WeddingTimeForScreenToFadeOut;
        g_fadeIn = PHeroMarriageDef->WeddingTimeForScreenToFadeIn; g_hold = -1.0f;
        g_rgba[0] = 0; g_rgba[1] = 0; g_rgba[2] = 0; g_rgba[3] = 255;
        ++g_events; g_eventType = 9;
        WaitingForFadeOut = true;
        WaitingForSex = true;
        Hero* hero = g_hero;
        if (hero && !hero->dead) {
            long* stats = 0;
            if (hero->hasStats) stats = &hero->timesHadSex;
            if (stats) ++*stats;
            hero->healed += hero->maxHealth; ++hero->healCalls;
            hero->scoreActivity = 6; hero->scoreValue = (float)*stats;
        }
    }
};

int main()
{
    int ok = 1;
    CHeroMarriageDef def = { 2.5f, 3.5f };
    CTCWife w = { &def, false, false };
    w.HaveSex();                                     // no hero
    ok &= g_updateList == 1 && g_fades == 1 && g_fadeOut == 2.5f && g_fadeIn == 3.5f && g_hold == -1.0f;
    ok &= g_rgba[3] == 255 && g_events == 1 && g_eventType == 9 && w.WaitingForFadeOut && w.WaitingForSex;
    Hero dead = { true, true, 4, 100.0f, 0, 0, 0, 0 };
    g_hero = &dead; w.HaveSex();
    ok &= dead.timesHadSex == 4 && dead.healCalls == 0;
    Hero h = { false, true, 4, 100.0f, 0, 0, 0, 0 };
    g_hero = &h; w.HaveSex();
    ok &= h.timesHadSex == 5 && h.healCalls == 1 && h.healed == 100.0f && h.scoreActivity == 6 && h.scoreValue == 5.0f;
    std::printf(ok ? "006cf920_TEST PASS\n" : "006cf920_TEST FAIL\n");
    return ok ? 0 : 1;
}