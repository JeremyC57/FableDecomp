// behaviour test for 0x00661D20
#include <stdio.h>
class C3DVector { public: float X, Y, Z; };
class CThing;
static const C3DVector* g_pos; static const CThing* g_ignore = (const CThing*)1;
class CWorldMap {
public:
    long GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const { g_pos = &pos; g_ignore = ignore; return pos.Z > 0 ? 2 : 0; }
};
static CWorldMap g_map;
class CThingCreatureBase {
public:
    CWorldMap& GetWorldMap() const { return g_map; }
    long GetNavigationLevelAt(const C3DVector& pos) const;
};
long CThingCreatureBase::GetNavigationLevelAt(const C3DVector& pos) const
{
    return GetWorldMap().GetNavigationLayerAt(pos, 0);
}
int main()
{
    CThingCreatureBase c; C3DVector p = { 1, 2, 3 }; int ok = 1;
    ok &= c.GetNavigationLevelAt(p) == 2 && g_pos == &p && g_ignore == 0;
    p.Z = -1; ok &= c.GetNavigationLevelAt(p) == 0;
    printf(ok ? "00661d20_TEST PASS\n" : "00661d20_TEST FAIL\n");
    return ok ? 0 : 1;
}