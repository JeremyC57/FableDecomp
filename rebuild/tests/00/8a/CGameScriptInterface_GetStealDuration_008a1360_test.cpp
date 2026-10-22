// Behaviour test for 0x008A1360 CGameScriptInterface::GetStealDuration: 2 unless the thing is stealable, its item has a stealable def and the thing has a stats def; then MaxDuration above the skill cap, else Min + skill*(Max-Min)/cap.
#include <cstdio>
struct StealDef { float cap, minD, maxD; };
struct Thing { bool stealable; bool hasItem; const StealDef* itemSteal; bool hasDef; const int* skill; };
static int GetStealDuration(const Thing* t)
{
    if (t && t->stealable) {
        if (t->hasItem) {
            const StealDef* s = t->itemSteal;
            if (s) {
                float minDuration = s->minD;
                float maxDuration = s->maxD;
                float maxSkill = s->cap;
                if (t->hasDef && t->skill) {
                    float skill = (float)*t->skill;
                    if (skill > maxSkill)
                        return (int)maxDuration;
                    return (int)((maxDuration - minDuration) / maxSkill * skill + minDuration);
                }
            }
        }
    }
    return 2;
}
int main()
{
    int bad = 0;
    StealDef d; d.cap = 10.0f; d.minD = 100.0f; d.maxD = 50.0f;
    int s5 = 5, s20 = 20, s10 = 10;
    Thing a = { true, true, &d, true, &s5 };
    Thing b = { true, true, &d, true, &s20 };
    Thing c = { true, true, &d, true, &s10 };
    Thing n1 = { false, true, &d, true, &s5 };
    Thing n2 = { true, false, &d, true, &s5 };
    Thing n3 = { true, true, 0, true, &s5 };
    Thing n4 = { true, true, &d, false, &s5 };
    Thing n5 = { true, true, &d, true, 0 };
    if (GetStealDuration(&a) != 75) { std::printf("a %d\n", GetStealDuration(&a)); ++bad; }
    if (GetStealDuration(&b) != 50) ++bad;
    if (GetStealDuration(&c) != 50) ++bad;
    if (GetStealDuration(0) != 2 || GetStealDuration(&n1) != 2 || GetStealDuration(&n2) != 2 ||
        GetStealDuration(&n3) != 2 || GetStealDuration(&n4) != 2 || GetStealDuration(&n5) != 2) ++bad;
    if (bad) { std::printf("008a1360_TEST FAIL\n"); return 1; }
    std::printf("008a1360_TEST PASS\n");
    return 0;
}