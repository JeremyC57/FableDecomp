#include <cstdio>
class C2DCoordI { public: long x, y; };
class CWorldMap { public: unsigned char _pad[0x4c]; bool m_disabled; float GetGroundSizeZAtBlock(const C2DCoordI& b) { return (float)(b.x + b.y) * 0.5f; } };
class CWorld { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual CWorldMap* GetMap() { return map; }
    CWorldMap* map;
};
class CGame { public: unsigned char _pad[0x24]; CWorld* m_pWorld; };
CGame* g_pGame = 0;
struct CMyAtmosMap {
    virtual long GetBlockValue(const C2DCoordI&) { return 0; }
    virtual float GetBlockHeight(const C2DCoordI& block) { if (g_pGame) { if (!g_pGame->m_pWorld->GetMap()->m_disabled) { CWorldMap* map = g_pGame->m_pWorld->GetMap(); float height = map->GetGroundSizeZAtBlock(block); return height; } } return 0.0f; }
};
int main()
{
    CMyAtmosMap m; C2DCoordI c = { 3, 4 };
    if (m.GetBlockHeight(c) != 0.0f) { std::printf("006b15c0_TEST FAIL\n"); return 1; }
    CWorldMap wm; wm.m_disabled = true; CWorld w; w.map = &wm; CGame g; g.m_pWorld = &w; g_pGame = &g;
    if (m.GetBlockHeight(c) != 0.0f) { std::printf("006b15c0_TEST FAIL\n"); return 1; }
    wm.m_disabled = false;
    if (m.GetBlockHeight(c) != 3.5f) { std::printf("006b15c0_TEST FAIL\n"); return 1; }
    std::printf("006b15c0_TEST PASS\n");
    return 0;
}