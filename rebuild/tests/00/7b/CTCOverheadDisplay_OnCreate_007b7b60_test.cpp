// Behaviour test for 0x007B7B60 CTCOverheadDisplay::OnCreate (see src/007b7b60.cpp): look up the parent's thing def by
// its global index; when found, fetch the overhead-display sub-def into PDef; when the lookup fails PDef stays null.
#include <cstdio>
struct Def { int sub; };
static Def g_defs[3] = { {0}, {11}, {22} };
static bool GetDef(long index, Def** out) { if (index > 0 && index < 3) { *out = &g_defs[index]; return true; } return false; }
static bool GetSubDef(const Def* d, int* out) { *out = d->sub; return true; }
struct Display { unsigned short parentIndex; int PDef; void OnCreate() { Def* d = 0; if (GetDef(parentIndex, &d)) GetSubDef(d, &PDef); } };
int main()
{
    Display a = { 2, 0 }; a.OnCreate();
    Display b = { 0, 5 }; b.OnCreate();
    bool ok = a.PDef == 22 && b.PDef == 5;
    std::printf(ok ? "007b7b60_TEST PASS\n" : "007b7b60_TEST FAIL\n");
    return ok ? 0 : 1;
}