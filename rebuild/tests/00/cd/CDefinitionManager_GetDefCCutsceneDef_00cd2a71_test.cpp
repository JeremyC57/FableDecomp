// behaviour test for 0x00CD2A71 CDefinitionManager::GetDef<CCutsceneDef> (model: hit replaces and addrefs, miss leaves out alone)
#include <cstdio>
struct Def { int count; int destroyed; };
static Def g_hit = { 1, 0 }, g_old = { 1, 0 };
static Def* Lookup(bool hit) { if (!hit) return 0; ++g_hit.count; return &g_hit; }
static bool GetDef(bool hit, Def** out)
{
    Def* def = Lookup(hit);
    if (def) {
        if (*out != def) { if (*out) { if (--(*out)->count == 0) (*out)->destroyed = 1; *out = 0; } *out = def; ++def->count; }
        if (--def->count == 0) def->destroyed = 1;
        return true;
    }
    return false;
}
int main()
{
    Def* out = &g_old;
    int ok = GetDef(true, &out) && out == &g_hit && g_old.destroyed == 1 && g_hit.count == 2;
    ok = ok && !GetDef(false, &out) && out == &g_hit;
    if (!ok) { std::printf("00cd2a71_TEST FAIL\n"); return 1; }
    std::printf("00cd2a71_TEST PASS\n");
    return 0;
}