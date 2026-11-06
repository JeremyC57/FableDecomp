// Behaviour test for CNOVI_Theresa::Main (0x00DB97A0): the per-frame step of Theresa at the Oakvale intro -- the meeting cut-scene
// once (the chocolate box offered on a yes), then talk / present / hit answers (chocolates accepted once, "get a present" once then
// "really get a present"), and the outro once the chocolates are given and the hero reaches M_TriggerOutro.
#include <cstdio>
#include <cstring>

struct Theresa {
    bool DoneIntro, SpokenAlready, GivenChocolates, GivenTheresaChocs, HeroHasBox, AttackOver;
    long Cutscenes, BadDeeds;
    char Last[48];
    void Say(const char* t) { std::strcpy(Last, t); }
    void Give() { GivenTheresaChocs = true; HeroHasBox = false; GivenChocolates = true; }
    // returns false when the outro has run (Main returns)
    bool Step(bool nearHero, bool talked, const char* presented, bool hit, long answer, float heroToOutro)
    {
        if (!DoneIntro) {
            if (!nearHero) return true;                   // skipping until the hero is within 5 units
            ++Cutscenes;
            if (HeroHasBox && answer == 1) { ++Cutscenes; Give(); }
            DoneIntro = true;
            return true;
        }
        if (talked && HeroHasBox) {
            if (answer == 1) { Say("HELLO"); Give(); } else Say("REALLY_GET_PRESENT");
        } else if (presented && std::strcmp(presented, "OBJECT_CHOCOLATE_BOX_UNGIVEABLE") == 0) {
            Say("HELLO"); Give();
        } else if (presented) {
            Say("BETTER_PRESENT");
        } else if (talked) {
            if (GivenChocolates) Say("HELLO");
            else if (!SpokenAlready) { SpokenAlready = true; Say("GET_PRESENT"); }
            else Say("REALLY_GET_PRESENT");
        } else if (hit) {
            ++BadDeeds; Say("DONT_HIT");
        }
        if (GivenChocolates && heroToOutro < 2.0f) { ++Cutscenes; AttackOver = true; return false; }
        return true;
    }
};

int main()
{
    Theresa t; std::memset(&t, 0, sizeof t); t.HeroHasBox = true;
    bool ok = true;
    ok = ok && t.Step(false, false, 0, false, -1, 9.0f) && !t.DoneIntro;
    ok = ok && t.Step(true, false, 0, false, 0, 9.0f) && t.DoneIntro && t.Cutscenes == 1 && !t.GivenChocolates;
    ok = ok && t.Step(true, true, 0, false, 0, 9.0f) && std::strcmp(t.Last, "REALLY_GET_PRESENT") == 0;
    ok = ok && t.Step(true, false, "OBJECT_APPLE", false, -1, 9.0f) && std::strcmp(t.Last, "BETTER_PRESENT") == 0;
    ok = ok && t.Step(true, false, 0, true, -1, 9.0f) && t.BadDeeds == 1;
    t.HeroHasBox = false;
    ok = ok && t.Step(true, true, 0, false, -1, 9.0f) && std::strcmp(t.Last, "GET_PRESENT") == 0 && t.SpokenAlready;
    ok = ok && t.Step(true, true, 0, false, -1, 9.0f) && std::strcmp(t.Last, "REALLY_GET_PRESENT") == 0;
    ok = ok && t.Step(true, false, "OBJECT_CHOCOLATE_BOX_UNGIVEABLE", false, -1, 9.0f) && t.GivenTheresaChocs && t.GivenChocolates;
    ok = ok && t.Step(true, true, 0, false, -1, 9.0f) && std::strcmp(t.Last, "HELLO") == 0;
    ok = ok && !t.Step(true, false, 0, false, -1, 1.0f) && t.AttackOver && t.Cutscenes == 2;
    if (!ok) { std::printf("00db97a0_TEST FAIL\n"); return 1; }
    std::printf("00db97a0_TEST PASS\n");
    return 0;
}
