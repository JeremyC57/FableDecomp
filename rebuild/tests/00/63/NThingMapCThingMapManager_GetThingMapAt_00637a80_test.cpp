#include <cstdio>
// Behaviour test for CThingMapManager::GetThingMapAt (0x00637A80).
class C2DCoordI { public: long X, Y; };
class C2DBoxI { public:
    long X0, Y0, X1, Y1;
    bool Contains(const C2DCoordI& p) const { return p.X >= X0 && p.X < X1 && p.Y >= Y0 && p.Y < Y1; }
};
struct CThingMap { C2DBoxI Area; };
struct CThingMapInfo { CThingMap* PMap; };
struct CThingMapManager {
    CThingMapInfo** First; CThingMapInfo** Last;
    CThingMap* GetThingMapAt(const C2DCoordI& pos)
    {
        for (CThingMapInfo** it = First; it != Last; ++it) {
            CThingMapInfo* info = *it;
            if (info->PMap->Area.Contains(pos)) return info->PMap;
        }
        return 0;
    }
};
int main()
{
    CThingMap a; a.Area.X0 = 0; a.Area.Y0 = 0; a.Area.X1 = 10; a.Area.Y1 = 10;
    CThingMap b; b.Area.X0 = 10; b.Area.Y0 = 0; b.Area.X1 = 20; b.Area.Y1 = 5;
    CThingMapInfo ia, ib; ia.PMap = &a; ib.PMap = &b;
    CThingMapInfo* list[2]; list[0] = &ia; list[1] = &ib;
    CThingMapManager m; m.First = list; m.Last = list + 2;
    C2DCoordI p;
    int fails = 0;
    p.X = 0; p.Y = 0; if (m.GetThingMapAt(p) != &a) ++fails;
    p.X = 9; p.Y = 9; if (m.GetThingMapAt(p) != &a) ++fails;
    p.X = 10; p.Y = 4; if (m.GetThingMapAt(p) != &b) ++fails;
    p.X = 10; p.Y = 5; if (m.GetThingMapAt(p) != 0) ++fails;
    p.X = -1; p.Y = 0; if (m.GetThingMapAt(p) != 0) ++fails;
    m.Last = list; p.X = 1; p.Y = 1; if (m.GetThingMapAt(p) != 0) ++fails;
    if (fails) { std::printf("00637a80_TEST FAIL\n"); return 1; }
    std::printf("00637a80_TEST PASS\n");
    return 0;
}