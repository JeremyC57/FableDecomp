// behaviour test for 0x007c54f0 CTCGhostSword::ResumeForCutscene
#include <cstdio>
struct Thing { bool dead; bool limbo; int calls; void SetInLimbo(bool b) { limbo = b; ++calls; } };
struct Sword {
    Thing* minions[3]; long count; bool CutSceneCancelled;
    void Run()
    {
        if (CutSceneCancelled == true)
        {
            CutSceneCancelled = false;
            for (long i = 0; i < count; ++i)
                if (minions[i] && !minions[i]->dead) minions[i]->SetInLimbo(false);
        }
    }
};
int main()
{
    Thing a = { false, true, 0 }, b = { true, true, 0 };
    Sword s = { { &a, 0, &b }, 3, true };
    s.Run();
    bool ok = s.CutSceneCancelled == false && a.limbo == false && a.calls == 1 && b.calls == 0;
    s.Run();
    ok = ok && a.calls == 1;
    if (!ok) { std::printf("007c54f0_TEST FAIL\n"); return 1; }
    std::printf("007c54f0_TEST PASS\n");
    return 0;
}
