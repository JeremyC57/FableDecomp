// Behaviour test for 0x00e80060: NScript::CV_FishingCompetitionScript::GetHeroTitleTextTag -- GUI tag of the hero title
// Host model: a stub game interface returns each title; the reconstructed switch maps it to the TEXT_GUI_HERO_TITLE_ tag.
#include <cstdio>
#include <cstring>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
class CCharString {
public:
    std::string S;
    CCharString(const char* text, long length = -1) : S(text) { (void)length; }
    CCharString(const CCharString& o) : S(o.S) { }
    const CCharString& operator=(const char* t) { S = t; return *this; }
};
enum EHeroTitle { TITLE_NONE=0, TITLE_REAPER=1, TITLE_SHADOWHUNTER=2, TITLE_MALEFICUS=3, TITLE_DEATHBRINGER=4, TITLE_ASSASSIN=5, TITLE_NECROMANCER=6, TITLE_AVATAR=7, TITLE_PILGRIM=8, TITLE_LIBERATOR=9, TITLE_PALADIN=10, TITLE_DRUID=11, TITLE_RANGER=12, TITLE_RUNEMASTER=13, TITLE_HOOD=14, TITLE_GLADIATOR=15, TITLE_SABRE=16, TITLE_ARROWDODGER=17, TITLE_PIEMASTER=18, TITLE_CHICKEN_CHASER=19, TITLE_ARSEFACE=20, TITLE_JACK=21 };
struct CGame { EHeroTitle Title; EHeroTitle GetHeroTitle() const { return Title; } };
struct CV_FishingCompetitionScript {
    CGame* Game;
    CCharString GetHeroTitleTextTag()
    {
        CCharString tag = "";
        switch (Game->GetHeroTitle())
        {
        case TITLE_ARROWDODGER:    tag = "TEXT_GUI_HERO_TITLE_ARROWDODGER"; break;
        case TITLE_ARSEFACE:       tag = "TEXT_GUI_HERO_TITLE_ARSEFACE"; break;
        case TITLE_ASSASSIN:       tag = "TEXT_GUI_HERO_TITLE_ASSASSIN"; break;
        case TITLE_AVATAR:         tag = "TEXT_GUI_HERO_TITLE_AVATAR"; break;
        case TITLE_CHICKEN_CHASER: tag = "TEXT_GUI_HERO_TITLE_CHICKEN_CHASER"; break;
        case TITLE_DEATHBRINGER:   tag = "TEXT_GUI_HERO_TITLE_DEATHBRINGER"; break;
        case TITLE_DRUID:          tag = "TEXT_GUI_HERO_TITLE_DRUID"; break;
        case TITLE_GLADIATOR:      tag = "TEXT_GUI_HERO_TITLE_GLADIATOR"; break;
        case TITLE_HOOD:           tag = "TEXT_GUI_HERO_TITLE_HOOD"; break;
        case TITLE_LIBERATOR:      tag = "TEXT_GUI_HERO_TITLE_LIBERATOR"; break;
        case TITLE_MALEFICUS:      tag = "TEXT_GUI_HERO_TITLE_MALEFICUS"; break;
        case TITLE_NECROMANCER:    tag = "TEXT_GUI_HERO_TITLE_NECROMANCER"; break;
        case TITLE_PALADIN:        tag = "TEXT_GUI_HERO_TITLE_PALADIN"; break;
        case TITLE_PIEMASTER:      tag = "TEXT_GUI_HERO_TITLE_PIEMASTER"; break;
        case TITLE_PILGRIM:        tag = "TEXT_GUI_HERO_TITLE_PILGRIM"; break;
        case TITLE_RANGER:         tag = "TEXT_GUI_HERO_TITLE_RANGER"; break;
        case TITLE_REAPER:         tag = "TEXT_GUI_HERO_TITLE_REAPER"; break;
        case TITLE_RUNEMASTER:     tag = "TEXT_GUI_HERO_TITLE_RUNEMASTER"; break;
        case TITLE_SABRE:          tag = "TEXT_GUI_HERO_TITLE_SABRE"; break;
        case TITLE_SHADOWHUNTER:   tag = "TEXT_GUI_HERO_TITLE_SHADOWHUNTER"; break;
        }
        return tag;
    }
};
static void run()
{
    // retail table 0x00E80148 order: value v -> tag
    static const char* const want[21] = { "", "REAPER", "SHADOWHUNTER", "MALEFICUS", "DEATHBRINGER", "ASSASSIN", "NECROMANCER",
        "AVATAR", "PILGRIM", "LIBERATOR", "PALADIN", "DRUID", "RANGER", "RUNEMASTER", "HOOD", "GLADIATOR", "SABRE",
        "ARROWDODGER", "PIEMASTER", "CHICKEN_CHASER", "ARSEFACE" };
    CGame g; CV_FishingCompetitionScript s; s.Game = &g;
    for (int v = 1; v <= 20; ++v) {
        g.Title = (EHeroTitle)v;
        std::string w = std::string("TEXT_GUI_HERO_TITLE_") + want[v];
        CHECK(s.GetHeroTitleTextTag().S == w);
    }
    g.Title = TITLE_NONE; CHECK(s.GetHeroTitleTextTag().S == "");
    g.Title = TITLE_JACK; CHECK(s.GetHeroTitleTextTag().S == "");
}
int main()
{
    run();
    if (g_fail) { std::printf("00e80060_TEST FAIL\n"); return 1; }
    std::printf("00e80060_TEST PASS\n");
    return 0;
}