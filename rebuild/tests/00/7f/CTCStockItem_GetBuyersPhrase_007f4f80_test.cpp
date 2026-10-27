#include <cstdio>
// Behaviour model: read a def field, then drop the reference the lookup took (destroy on the last one).
struct Def { long RefCount; int destroyed; long Field; };
static Def* g_def;
static bool GetSubDef(Def** out) { ++g_def->RefCount; *out = g_def; return true; }
static long Getter()
{
    Def* def = 0;
    GetSubDef(&def);
    long value = def->Field;
    if (--def->RefCount == 0) ++def->destroyed;
    return value;
}
int main()
{
    Def d = { 1, 0, 1234 }; g_def = &d;
    if (Getter() != 1234 || d.RefCount != 1 || d.destroyed != 0) { std::printf("007f4f80_TEST FAIL\n"); return 1; }
    d.RefCount = 0;                                          // the lookup's reference is the last one
    if (Getter() != 1234 || d.destroyed != 1) { std::printf("007f4f80_TEST FAIL\n"); return 1; }
    std::printf("007f4f80_TEST PASS\n");
    return 0;
}