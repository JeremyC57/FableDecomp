// Behaviour test for CTCMultiArrow::ReleaseShot (0x007CC710): a model of the volley bookkeeping.
#include <cstdio>
#include <vector>
struct Thing { bool dead; int kills; Thing() : dead(false), kills(0) {} void Kill(bool) { ++kills; dead = true; } };
struct Arrow {
    long NumberOfShots, NumberInVolley, NumOfVolleys, CreateAmmoEffectCount; int cleanups;
    std::vector<Thing*> AmmoEffects;
    void FinalCleanup() { ++cleanups; }
    void ReleaseShot()
    {
        long volley = --NumberOfShots / NumberInVolley;
        if (volley < NumOfVolleys) {
            if (AmmoEffects.empty())
                --CreateAmmoEffectCount;
            else {
                Thing* effect = AmmoEffects.back();
                if (effect && !effect->dead)
                    effect->Kill(true);
                AmmoEffects.pop_back();
            }
            NumOfVolleys = volley;
        }
        if (NumberOfShots <= 0)
            FinalCleanup();
    }
};
int main()
{
    Thing a, b;
    Arrow m; m.NumberOfShots = 4; m.NumberInVolley = 2; m.NumOfVolleys = 2; m.CreateAmmoEffectCount = 3; m.cleanups = 0;
    m.AmmoEffects.push_back(&a); m.AmmoEffects.push_back(&b);
    m.ReleaseShot();   // 3/2 = 1 < 2: kill b
    if (m.NumOfVolleys != 1 || b.kills != 1 || m.AmmoEffects.size() != 1 || m.cleanups) { std::printf("007cc710_TEST FAIL\n"); return 1; }
    m.ReleaseShot();   // 2/2 = 1: nothing
    if (m.AmmoEffects.size() != 1) { std::printf("007cc710_TEST FAIL\n"); return 1; }
    m.ReleaseShot();   // 1/2 = 0 < 1: kill a
    m.AmmoEffects.clear();
    m.NumOfVolleys = 1; m.ReleaseShot();   // 0/2 = 0 < 1, empty: count down; shots 0 -> cleanup
    if (a.kills != 1 || m.CreateAmmoEffectCount != 2 || m.cleanups != 1) { std::printf("007cc710_TEST FAIL\n"); return 1; }
    std::printf("007cc710_TEST PASS\n");
    return 0;
}