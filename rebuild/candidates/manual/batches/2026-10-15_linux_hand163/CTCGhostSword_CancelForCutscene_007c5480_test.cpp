// behaviour test for 0x007c5480 CTCGhostSword::CancelForCutscene
#include <cstdio>
struct Thing { bool dead; bool limbo; int calls; void SetInLimbo(bool b) { limbo = b; ++calls; } };
struct Sword {
    Thing* minions[3]; long count; bool CutSceneCancelled;
    void Run()
    {
        if (CutSceneCancelled == false)
        {
            CutSceneCancelled = true;
            for (long i = 0; i < count; ++i)
                if (minions[i] && !minions[i]->dead) minions[i]->SetInLimbo(true);
        }
    }
};
int main()
{
    Thing a = { false, false, 0 }, b = { true, false, 0 };
    Sword s = { { &a, 0, &b }, 3, false };
    s.Run();
    bool ok = s.CutSceneCancelled == true && a.limbo == true && a.calls == 1 && b.calls == 0;
    s.Run();
    ok = ok && a.calls == 1;
    if (!ok) { std::printf("007c5480_TEST FAIL\n"); return 1; }
    std::printf("007c5480_TEST PASS\n");
    return 0;
}
