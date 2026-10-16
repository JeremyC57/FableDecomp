#include <cstdio>
struct Def { float Health; };
static Def g_defs[4] = { { 1 }, { 25 }, { 50 }, { 75 } };
struct Obj { unsigned short m_defIndex; float MaxHealth, Health; Def* PDef; bool baseOk;
    bool Base(long idx) { if (idx > 0 && baseOk) { m_defIndex = (unsigned short)idx; return true; } return false; }
    bool ConstructFromParams(long idx) {
        if (Base(idx)) { PDef = &g_defs[m_defIndex]; MaxHealth = Health = PDef->Health; return true; }
        return false; } };
int main()
{
    Obj o = { 0, 0, 0, 0, true };
    bool ok = o.ConstructFromParams(2) && o.MaxHealth == 50 && o.Health == 50;
    Obj p = { 0, 3, 3, 0, false };
    ok = ok && !p.ConstructFromParams(2) && p.Health == 3;
    if (!ok) { std::printf("00703050_TEST FAIL\n"); return 1; }
    std::printf("00703050_TEST PASS\n");
    return 0;
}
