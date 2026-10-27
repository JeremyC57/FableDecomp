// behaviour test for 0x0043FEB0: GetDef refuses index <= 0 and missing defs, and re-points `out` with correct reference counts
#include <cstdio>
struct Def { long refs; bool dead; };
static Def g_defs[3];
static Def* Lookup(long i) { return (i > 0 && i < 3) ? &g_defs[i] : 0; }
static bool GetDef(long index, Def** out)
{
    if (index <= 0) return false;
    Def* d = Lookup(index); if (d) ++d->refs;
    if (!d) return false;
    if (*out != d) { if (*out && --(*out)->refs == 0) (*out)->dead = true; *out = d; ++d->refs; }
    if (--d->refs == 0) d->dead = true;
    return true;
}
int main()
{
    Def* out = 0; g_defs[1].refs = 1; g_defs[2].refs = 1;
    bool a = GetDef(0, &out), b = GetDef(5, &out), c = GetDef(1, &out), d = GetDef(2, &out);
    if (a || b || !c || !d || out != &g_defs[2] || g_defs[2].refs != 2 || g_defs[1].refs != 1 || g_defs[1].dead) { std::printf("0043feb0_TEST FAIL\n"); return 1; }
    std::printf("0043feb0_TEST PASS\n");
    return 0;
}