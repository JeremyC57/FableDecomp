// Behaviour test for CEngineLightingManager::ResetAlreadyFoundFlags @ 0x00B44CE0.
#include <cstdio>
class CEngineLocalLightElement { public: bool m_alreadyFound; };
struct CLocalLightElementLink { void* u; CEngineLocalLightElement* m_pElement; CLocalLightElementLink* m_pNext; };
class CEngineLightingManager { public:
    CLocalLightElementLink* m_lightLookupGrid[16][16];
    void ResetAlreadyFoundFlags(long minX, long minY, long maxX, long maxY)
    {
        for (long y = minY; y <= maxY; ++y)
            for (long x = minX; x <= maxX; ++x)
                for (CLocalLightElementLink* link = m_lightLookupGrid[x][y]; link; link = link->m_pNext)
                    link->m_pElement->m_alreadyFound = false;
    }
};
int main()
{
    static CEngineLightingManager m; static CEngineLocalLightElement e[16][16][2]; static CLocalLightElementLink l[16][16][2];
    for (int x = 0; x < 16; ++x) for (int y = 0; y < 16; ++y) {
        for (int k = 0; k < 2; ++k) { e[x][y][k].m_alreadyFound = true; l[x][y][k].m_pElement = &e[x][y][k]; }
        l[x][y][0].m_pNext = &l[x][y][1]; l[x][y][1].m_pNext = 0;
        m.m_lightLookupGrid[x][y] = (x == 5 && y == 5) ? 0 : &l[x][y][0];
    }
    m.ResetAlreadyFoundFlags(2, 3, 6, 4);
    int ok = 1;
    for (int x = 0; x < 16; ++x) for (int y = 0; y < 16; ++y) for (int k = 0; k < 2; ++k) {
        bool inside = x >= 2 && x <= 6 && y >= 3 && y <= 4;
        if (e[x][y][k].m_alreadyFound == inside) ok = 0;
    }
    m.ResetAlreadyFoundFlags(8, 8, 7, 9);                  // empty x range: nothing touched
    ok = ok && e[8][8][0].m_alreadyFound && e[8][9][1].m_alreadyFound;
    m.ResetAlreadyFoundFlags(8, 9, 8, 8);                  // empty y range
    ok = ok && e[8][8][0].m_alreadyFound;
    if (!ok) { std::printf("00b44ce0_TEST FAIL\n"); return 1; }
    std::printf("00b44ce0_TEST PASS\n");
    return 0;
}