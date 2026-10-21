#include <cstdio>
#include <cstring>
// Behaviour copy of CAIStateGroup_ApprenticeHero::GetDummy (0x0092A0C0).
static bool g_used[3];
static const char* g_found = 0;   // name of the only dummy type that exists in the world
struct Hero {
    long Action; const char* PDummy; long start; int lookups;
    static bool IsDummyTypeLegal(long t) { return t >= 0 && t < 3 && !g_used[t]; }
    static const char* Name(long t) { switch (t) { case 0: return "MeleePractice"; case 1: return "SkillTarget"; case 2: return "WillDummy"; } return ""; }
    const char* FindDummy(const char* n) { ++lookups; return (g_found && std::strcmp(n, g_found) == 0) ? g_found : 0; }
    bool GetDummy() {
        PDummy = 0; long i = 0;
        while (i < 3) {
            for (; i < 3; ++i) { Action = (start + i) % 3; if (IsDummyTypeLegal(Action)) break; }
            if (i == 3) break;
            PDummy = FindDummy(Name(Action));
            if (PDummy) return true;
            ++i;
        }
        return false;
    }
};
int main() {
    g_found = "WillDummy";
    Hero h = { -1, 0, 1, 0 };   // tries 1 (SkillTarget: missing), 2 (WillDummy: found)
    if (!h.GetDummy() || h.Action != 2 || h.lookups != 2) { std::printf("0092a0c0_TEST FAIL\n"); return 1; }
    g_used[2] = true; Hero k = { -1, 0, 2, 0 };   // 2 in use, 0 and 1 missing
    if (k.GetDummy() || k.lookups != 2 || k.PDummy) { std::printf("0092a0c0_TEST FAIL\n"); return 1; }
    std::printf("0092a0c0_TEST PASS\n"); return 0;
}
