// Behaviour test (0x00a3f900): CGameMapInterface::GetFlagsAt truncates the float position and asks the game map, 0 without a map
#include <cstdio>
struct C2DCoordsI { long X; long Y; };
class CGameMap { public:
    virtual long GetFlagsAt(const C2DCoordsI* pos) { return pos->X * 1000 + pos->Y; }
    virtual ~CGameMap() { }
};
struct CGameMapInterface {
    CGameMap* m_pMap;
    virtual long GetFlagsAt(float x, float y)
    {
        if (!m_pMap)
            return 0;
        C2DCoordsI pos;
        pos.X = (long)x;
        pos.Y = (long)y;
        return m_pMap->GetFlagsAt(&pos);
    }
    virtual ~CGameMapInterface() { }
};
int main()
{
    CGameMap map;
    CGameMapInterface gi; gi.m_pMap = 0;
    if (gi.GetFlagsAt(3.7f, 4.2f) != 0) { std::printf("00a3f900_TEST FAIL\n"); return 1; }
    gi.m_pMap = &map;
    if (gi.GetFlagsAt(3.7f, 4.2f) != 3004) { std::printf("00a3f900_TEST FAIL\n"); return 1; }
    if (gi.GetFlagsAt(-1.5f, 12.9f) != -1000 + 12) { std::printf("00a3f900_TEST FAIL\n"); return 1; }
    std::printf("00a3f900_TEST PASS\n");
    return 0;
}