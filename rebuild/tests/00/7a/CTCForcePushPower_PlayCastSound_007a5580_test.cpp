// Behaviour test for 0x007a5580: CTCForcePushPower::PlayCastSound -- level cast sound through the parent's sound player
#include <cstdio>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CTCSoundPlayer { std::vector<std::string> Played;
    unsigned int PlayAttachedCriteria(const std::string& c, bool a, bool b, bool d, long e) { Played.push_back(c); CHECK(a && !b && !d && e == -1); return 1; } };
struct CThing { bool HasSound; CTCSoundPlayer Player;
    bool GetTC(CTCSoundPlayer** out) { if (!HasSound) return false; *out = &Player; return true; } };
struct CTCForcePushPower { CThing* ParentThing; long SpellLevel;
    void PlayCastSound() {
        CTCSoundPlayer* sound;
        if (ParentThing->GetTC(&sound)) {
            switch (SpellLevel) {
            case 0: sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_NO_POWERUP", true, false, false, -1); break;
            case 1: sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_1", true, false, false, -1); break;
            case 2: sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_2", true, false, false, -1); break;
            case 3: sound->PlayAttachedCriteria("HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_3", true, false, false, -1); break;
            }
        }
    } };
int main()
{
    CThing t; t.HasSound = true; CTCForcePushPower p; p.ParentThing = &t;
    for (long l = 0; l < 5; ++l) { p.SpellLevel = l; p.PlayCastSound(); }
    CHECK(t.Player.Played.size() == 4);
    CHECK(t.Player.Played.size() == 4 && t.Player.Played[1] == "HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_1" && t.Player.Played[2] == "HERO_ABILITY_FORCE_PUSH_CAST_LEVEL_2");
    t.HasSound = false; p.SpellLevel = 0; p.PlayCastSound(); CHECK(t.Player.Played.size() == 4);
    if (g_fail) { std::printf("007a5580_TEST FAIL\n"); return 1; }
    std::printf("007a5580_TEST PASS\n");
    return 0;
}