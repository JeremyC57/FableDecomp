#include <cstdio>
static int g_calls = 0;
static unsigned long g_lastFlags = 99;
struct CTCGraphicAppearance { void SetActivePersistenceFlags(unsigned long flags) { ++g_calls; g_lastFlags = flags; } };
struct CThing { CTCGraphicAppearance* PGraphicAppearance; };
struct CTCPhysicsBase {
    CThing* sub;
    void ClearGraphicPersistenceFlags()
    {
        CTCGraphicAppearance* appearance = sub->PGraphicAppearance;
        if (appearance)
            appearance->SetActivePersistenceFlags(0);
    }
};
int main()
{
    CTCGraphicAppearance app; CThing t; t.PGraphicAppearance = 0; CTCPhysicsBase b; b.sub = &t;
    b.ClearGraphicPersistenceFlags();
    if (g_calls != 0) { std::printf("006b0150_TEST FAIL\n"); return 1; }
    t.PGraphicAppearance = &app; b.ClearGraphicPersistenceFlags();
    if (g_calls != 1 || g_lastFlags != 0) { std::printf("006b0150_TEST FAIL\n"); return 1; }
    std::printf("006b0150_TEST PASS\n");
    return 0;
}