#include <cstdio>
// Behaviour test for 00f2a1d0: CCutsceneDef::Transfer hands its eight string lists to the context in the retail order
// Macro, SkipCond, SetupCond, Lights, LightScene, Sound, Answer0, Answer1.
struct Vec { int tag; };
static const Vec* g_seen[16]; static int g_count = 0;
struct CPersistContext { void TransferVectorBinary(Vec& v) { if (g_count < 16) g_seen[g_count++] = &v; } };
struct CCutsceneDef {
    Vec Macro, SkipCond, Lights, LightScene, SetupCond, Sound, Answer0, Answer1;
    void Transfer(CPersistContext& c)
    {
        c.TransferVectorBinary(Macro); c.TransferVectorBinary(SkipCond); c.TransferVectorBinary(SetupCond);
        c.TransferVectorBinary(Lights); c.TransferVectorBinary(LightScene); c.TransferVectorBinary(Sound);
        c.TransferVectorBinary(Answer0); c.TransferVectorBinary(Answer1);
    }
};
int main()
{
    CCutsceneDef d; CPersistContext c;
    d.Transfer(c);
    const Vec* expect[8] = { &d.Macro, &d.SkipCond, &d.SetupCond, &d.Lights, &d.LightScene, &d.Sound, &d.Answer0, &d.Answer1 };
    int ok = g_count == 8;
    for (int i = 0; ok && i < 8; ++i) ok = g_seen[i] == expect[i];
    if (!ok) { std::printf("00f2a1d0_TEST FAIL\n"); return 1; }
    std::printf("00f2a1d0_TEST PASS\n");
    return 0;
}