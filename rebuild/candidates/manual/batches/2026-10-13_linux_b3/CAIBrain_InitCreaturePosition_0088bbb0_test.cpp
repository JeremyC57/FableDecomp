#include <cstdio>

static int g_prep = 0, g_calls = 0;
struct G { bool ok; virtual void v00(){} virtual void v04(){} virtual void v08(){} virtual bool InitCreaturePosition() { ++g_calls; return ok; } };
struct E { long Key; G* Group; };
struct Brain { unsigned char pad[0x28]; E* Begin; E* End;
    void Prep() { ++g_prep; }
    void Init() { Prep(); for (E* it = Begin; it != End; ++it) if (it->Group->InitCreaturePosition()) return; } };
int main()
{
    G g0, g1, g2; g0.ok = false; g1.ok = true; g2.ok = true; E e[3]; e[0].Group = &g0; e[1].Group = &g1; e[2].Group = &g2;
    Brain b; b.Begin = e; b.End = e + 3; b.Init();
    if (g_prep != 1 || g_calls != 2) { std::printf("0088bbb0_TEST FAIL\n"); return 1; }
    std::printf("0088bbb0_TEST PASS\n");
    return 0;
}
