#include <cstdio>
// Behaviour test for CThingMap::GetMemoryFootprint (0x0085CFE0).
struct CThingMapCell { void* PFirstNode; };
template<class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
static long g_NoMapwhoLevels = 0;
struct CThingMap {
    CRetailVector< CRetailVector<CThingMapCell> > Cells;
    long GetMemoryFootprint()
    {
        long footprint = 0;
        for (long level = 0; level < g_NoMapwhoLevels; ++level)
            footprint += Cells[level].size() * sizeof(CThingMapCell);
        return footprint;
    }
};
int main()
{
    CThingMapCell cells[10];
    CRetailVector<CThingMapCell> levels[3];
    levels[0]._M_start = cells; levels[0]._M_finish = cells + 4;
    levels[1]._M_start = cells; levels[1]._M_finish = cells + 1;
    levels[2]._M_start = cells; levels[2]._M_finish = cells + 10;
    CThingMap m; m.Cells._M_start = levels; m.Cells._M_finish = levels + 3;
    int fails = 0;
    g_NoMapwhoLevels = 0; if (m.GetMemoryFootprint() != 0) ++fails;
    g_NoMapwhoLevels = 2; if (m.GetMemoryFootprint() != (long)(5 * sizeof(CThingMapCell))) ++fails;
    g_NoMapwhoLevels = 3; if (m.GetMemoryFootprint() != (long)(15 * sizeof(CThingMapCell))) ++fails;
    if (fails) { std::printf("0085cfe0_TEST FAIL\n"); return 1; }
    std::printf("0085cfe0_TEST PASS\n");
    return 0;
}