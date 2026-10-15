#include <cstdio>

struct C2DCoordsI { long X; long Y; };
static long g_x = 0, g_y = 0;
struct CGameMap {
    virtual ~CGameMap() { }
    virtual float GetHeightAt(const C2DCoordsI* pos) { g_x = pos->X; g_y = pos->Y; return (float)(pos->X * 100 + pos->Y); }
};
struct CGameMapInterface {
    CGameMap* m_pMap;
    float GetHeightAt(float x, float y)
    {
        if (!m_pMap)
            return 0.0f;
        C2DCoordsI pos;
        pos.X = (long)x;
        pos.Y = (long)y;
        return m_pMap->GetHeightAt(&pos);
    }
};

int main()
{
    CGameMap map; CGameMapInterface i; i.m_pMap = 0;
    if (i.GetHeightAt(3.7f, 4.2f) != 0.0f) { std::printf("00a3f8b0_TEST FAIL\n"); return 1; }
    i.m_pMap = &map;
    float h = i.GetHeightAt(3.7f, -4.9f);              // truncation toward zero
    if (g_x != 3 || g_y != -4 || h != 296.0f) { std::printf("00a3f8b0_TEST FAIL\n"); return 1; }
    std::printf("00a3f8b0_TEST PASS\n");
    return 0;
}