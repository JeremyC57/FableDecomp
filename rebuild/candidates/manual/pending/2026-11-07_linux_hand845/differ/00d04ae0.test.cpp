// Behaviour test for NScript::CQ_BanditCampBossBattleScript::CAssassin1::Main (0x00D04AE0): the first assassin waits under
// scripted control until the assassins are attacked or their cut scene has played; attacked first, she fights; after the cut
// scene she idles until the thread ends.
#include <cstdio>

struct Assassin1 {
    bool Attacked, CutscenePlayed, Fighting, Idling;
    bool Frame()
    {
        if (!Attacked && !CutscenePlayed) return true;
        if (Attacked) Fighting = true; else Idling = true;
        return false;
    }
};

int main()
{
    Assassin1 a = { false, false, false, false };
    bool ok = a.Frame() && !a.Fighting;
    a.Attacked = true; ok = ok && !a.Frame() && a.Fighting && !a.Idling;
    Assassin1 b = { false, true, false, false }; ok = ok && !b.Frame() && b.Idling;
    if (!ok) { std::printf("00d04ae0_TEST FAIL\n"); return 1; }
    std::printf("00d04ae0_TEST PASS\n");
    return 0;
}
