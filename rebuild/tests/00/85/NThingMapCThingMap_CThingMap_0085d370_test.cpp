#include <cstdio>
// Behaviour test for CThingMap::CThingMap (0x0085D370): members initialised, then InitMapwhoLevels + Clear.
struct C2DBoxI {
    long X0, Y0, X1, Y1;
    C2DBoxI(long a, long b, long c, long d) : X0(a), Y0(b), X1(c), Y1(d) { }
    C2DBoxI(const C2DBoxI& o) : X0(o.X0), Y0(o.Y0), X1(o.X1), Y1(o.Y1) { }
};
template<class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};
struct CThingMapManager { int id; };
static int g_order[4]; static int g_n = 0;
struct CThingMap {
    CRetailVector<int> MapwhoLevelInfo; CThingMapManager* Manager; CRetailVector<int> Cells; C2DBoxI Area; long NoThings;
    CThingMap(CThingMapManager* manager, const C2DBoxI& area)
        : MapwhoLevelInfo(), Manager(manager), Cells(), Area(area), NoThings(0) { InitMapwhoLevels(); Clear(); }
    void InitMapwhoLevels() { g_order[g_n++] = 1; }
    void Clear() { g_order[g_n++] = 2; }
};
int main()
{
    CThingMapManager mgr; mgr.id = 1;
    C2DBoxI box(1, 2, 30, 40);
    CThingMap m(&mgr, box);
    int fails = 0;
    if (m.Manager != &mgr || m.Area.X0 != 1 || m.Area.Y0 != 2 || m.Area.X1 != 30 || m.Area.Y1 != 40 || m.NoThings != 0) ++fails;
    if (m.MapwhoLevelInfo._M_start || m.Cells._M_finish || m.Cells._M_end_of_storage) ++fails;
    if (g_n != 2 || g_order[0] != 1 || g_order[1] != 2) ++fails;
    if (fails) { std::printf("0085d370_TEST FAIL\n"); return 1; }
    std::printf("0085d370_TEST PASS\n");
    return 0;
}