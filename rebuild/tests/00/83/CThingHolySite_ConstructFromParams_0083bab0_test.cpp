#include <cstdio>
#include <cstring>
struct Def { unsigned char PlayerStart; };
static Def g_defs[4];
struct DM { bool GetDef(long i, Def*& out) { out = &g_defs[i]; return true; } };
static DM g_dm;
struct HolySite { unsigned short m_defIndex; Def* PDef; unsigned char PlayerStart; bool baseOk; long gotA4; char gotName[8];
    bool Base(long idx, long a4, const char* name) { gotA4 = a4; std::strcpy(gotName, name); if (idx > 0 && baseOk) { m_defIndex = (unsigned short)idx; return true; } return false; }
    bool ConstructFromParams(long idx, bool ps) {
        bool ok; { const char* name = ""; ok = Base(idx, 1, name); }
        if (ok) { g_dm.GetDef(m_defIndex, PDef); PlayerStart = ps != 0; return true; }
        return false; } };
int main()
{
    HolySite h = { 0, 0, 0, true, 0, "x" };
    bool ok = h.ConstructFromParams(2, true) && h.PDef == &g_defs[2] && h.PlayerStart == 1 && h.gotA4 == 1 && h.gotName[0] == 0;
    HolySite f = { 0, 0, 7, true, 0, "x" };
    ok = ok && !f.ConstructFromParams(0, true) && f.PlayerStart == 7;
    if (!ok) { std::printf("0083bab0_TEST FAIL\n"); return 1; }
    std::printf("0083bab0_TEST PASS\n");
    return 0;
}