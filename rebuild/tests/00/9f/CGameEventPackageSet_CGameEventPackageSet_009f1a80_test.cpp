#include <cstdio>
#include <cstring>
#include <new>

struct CGameEvent { long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    CGameEvent() { Player = -1; Valid = false; } };
struct CGameEventPackage { long TimeStamp; unsigned long NoEvents; CGameEvent Events[40]; CGameEventPackage() : NoEvents(0) {} };
struct CGameEventPackageSet { unsigned long NoPackages; CGameEventPackage Packages[50]; CGameEventPackageSet() { NoPackages = 0; } };
static unsigned char g_raw[sizeof(CGameEventPackageSet)];
int main()
{
    std::memset(g_raw, 0xaa, sizeof g_raw);
    CGameEventPackageSet* set = new (g_raw) CGameEventPackageSet;
    if ((sizeof(long) == 4 && (sizeof(CGameEvent) != 0x28 || sizeof(CGameEventPackage) != 0x648)) || set->NoPackages != 0) { std::printf("009f1a80_TEST FAIL\n"); return 1; }
    for (int p = 0; p < 50; ++p) {
        if (set->Packages[p].NoEvents != 0) { std::printf("009f1a80_TEST FAIL\n"); return 1; }
        for (int e = 0; e < 40; ++e)
            if (set->Packages[p].Events[e].Player != -1 || set->Packages[p].Events[e].Valid) { std::printf("009f1a80_TEST FAIL\n"); return 1; }
    }
    std::printf("009f1a80_TEST PASS\n");
    return 0;
}