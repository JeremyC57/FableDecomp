// Behaviour test for CThing::vfunc_17 @ 0x004C7B80: def present, scannable, activated, alive, not in limbo, def InMapSearches.
#include <cstdio>
struct CThingBaseDef { unsigned char low : 7; unsigned char InMapSearches : 1; };
struct CThing {
    CThingBaseDef* PBaseDef;
    unsigned char Dead : 1;
    unsigned char CreatedByOwnerThingCreation : 1;
    unsigned char IncludeInThingScans : 1;
    unsigned char Activated : 1;
    unsigned char GloballyLoaded : 1;
    unsigned char InLimbo : 1;
    bool IsAlive() const { return !Dead; }
    bool vfunc_17()
    {
        CThingBaseDef* def = PBaseDef;
        if (!def)
            return false;
        return IncludeInThingScans && Activated && IsAlive() && !InLimbo && def->InMapSearches;
    }
};
int main()
{
    CThingBaseDef def; def.low = 0; def.InMapSearches = 1;
    CThing t; t.PBaseDef = &def; t.Dead = 0; t.CreatedByOwnerThingCreation = 0; t.IncludeInThingScans = 1; t.Activated = 1;
    t.GloballyLoaded = 0; t.InLimbo = 0;
    if (!t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    t.Dead = 1; if (t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    t.Dead = 0; t.InLimbo = 1; if (t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    t.InLimbo = 0; t.Activated = 0; if (t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    t.Activated = 1; t.IncludeInThingScans = 0; if (t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    t.IncludeInThingScans = 1; def.InMapSearches = 0; if (t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    def.InMapSearches = 1; t.PBaseDef = 0; if (t.vfunc_17()) { std::printf("004c7b80_TEST FAIL\n"); return 1; }
    std::printf("004c7b80_TEST PASS\n");
    return 0;
}