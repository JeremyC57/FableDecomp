#include <cstdio>
struct C2DCoordI { long x, y; };
struct CWorldMap { unsigned char _pad[0x4c]; bool m_disabled; long GetBlockValue(const C2DCoordI& b) { return b.x * 10 + b.y; } };
struct CWorld {
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual CWorldMap* GetMap() { return map; }
    CWorldMap* map;
};
struct CGame { unsigned char _pad[0x24]; CWorld* m_pWorld; };
CGame* g_pGame = 0;
struct CMyAtmosMap {
    virtual long GetBlockValue(const C2DCoordI& block) { if (g_pGame) { if (!g_pGame->m_pWorld->GetMap()->m_disabled) { CWorldMap* map = g_pGame->m_pWorld->GetMap(); return map->GetBlockValue(block); } } return 0; }
    virtual float GetBlockHeight(const C2DCoordI&) { return 0.0f; }
};
int main()
{
    CMyAtmosMap m; C2DCoordI c = { 3, 4 };
    if (m.GetBlockValue(c) != 0) { std::printf("006b1580_TEST FAIL\n"); return 1; }
    CWorldMap wm; wm.m_disabled = true; CWorld w; w.map = &wm; CGame g; g.m_pWorld = &w; g_pGame = &g;
    if (m.GetBlockValue(c) != 0) { std::printf("006b1580_TEST FAIL\n"); return 1; }
    wm.m_disabled = false;
    if (m.GetBlockValue(c) != 34) { std::printf("006b1580_TEST FAIL\n"); return 1; }
    std::printf("006b1580_TEST PASS\n");
    return 0;
}