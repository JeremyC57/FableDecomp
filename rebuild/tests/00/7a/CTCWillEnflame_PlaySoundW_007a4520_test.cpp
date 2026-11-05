// Behaviour test for 0x007a4520: CTCWillEnflame::PlaySoundW -- level cast sound through the sound player, then SIMPLE_NOISE
// Host model of the parent thing (interface bit + map lookup), the sound player and the creature noise.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CTCSoundPlayer { std::vector<std::string> Played;
    unsigned int PlayAttachedCriteria(const std::string& c, bool a, bool b, bool d, long e) { Played.push_back(c); CHECK(a && !b && !d && e == -1); return 1; } };
struct CThing { bool HasSound; CTCSoundPlayer Player; std::vector<std::string> Noises;
    bool GetTC(CTCSoundPlayer** out) { if (!HasSound) return false; *out = &Player; return true; }
    void MakeNoise(const std::string& n) { Noises.push_back(n); } };
struct CTCWillEnflame { CThing* ParentThing; long SpellLevel;
    void PlaySoundW() {
        CThing* parent = ParentThing; CTCSoundPlayer* sound;
        if (parent->GetTC(&sound)) {
            switch (SpellLevel) {
            case 0: sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_NO_POWERUP", true, false, false, -1); break;
            case 1: sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_LEVEL_1", true, false, false, -1); break;
            case 2: sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_LEVEL_2", true, false, false, -1); break;
            case 3: sound->PlayAttachedCriteria("HERO_ABILITY_ENFLAME_CAST_LEVEL_3", true, false, false, -1); break;
            }
        }
        parent->MakeNoise("SIMPLE_NOISE");
    } };
int main()
{
    CThing t; t.HasSound = true; CTCWillEnflame e; e.ParentThing = &t;
    e.SpellLevel = 0; e.PlaySoundW(); e.SpellLevel = 3; e.PlaySoundW(); e.SpellLevel = 7; e.PlaySoundW();
    CHECK(t.Player.Played.size() == 2);
    CHECK(t.Player.Played.size() == 2 && t.Player.Played[0] == "HERO_ABILITY_ENFLAME_CAST_NO_POWERUP" && t.Player.Played[1] == "HERO_ABILITY_ENFLAME_CAST_LEVEL_3");
    CHECK(t.Noises.size() == 3);
    t.HasSound = false; e.SpellLevel = 1; e.PlaySoundW();
    CHECK(t.Player.Played.size() == 2 && t.Noises.size() == 4 && t.Noises[3] == "SIMPLE_NOISE");
    if (g_fail) { std::printf("007a4520_TEST FAIL\n"); return 1; }
    std::printf("007a4520_TEST PASS\n");
    return 0;
}