#include <cstdio>

struct G { unsigned char pad[0x12]; bool Enabled; };
struct E { long Key; G* Group; };
struct Brain { unsigned char pad[0x28]; E* Begin; E* End;
    void Set(bool b) { for (E* it = Begin; it != End; ++it) it->Group->Enabled = b; } };
int main()
{
    G g0, g1; E e[2]; e[0].Group = &g0; e[1].Group = &g1; g0.Enabled = g1.Enabled = false;
    Brain b; b.Begin = e; b.End = e + 2; b.Set(true);
    if (!g0.Enabled || !g1.Enabled) { std::printf("0088b8c0_TEST FAIL\n"); return 1; }
    b.End = e; b.Set(false); if (!g0.Enabled) { std::printf("0088b8c0_TEST FAIL\n"); return 1; }
    std::printf("0088b8c0_TEST PASS\n");
    return 0;
}