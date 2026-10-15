// NThingMap::CThingMap::GetMemoryFootprint() -- bytes held by the per-level cell vectors: sum over the mapwho levels of Cells[level].size() * sizeof(CThingMapCell)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0085CFE0, length 35 bytes.
// No callees; the level count is the global at 0x013BA848 (shared by every thing map).
// Layout (PDB NThingMap::CThingMap, retail vectors 12 bytes): CBaseClassNonCopyable vptr +0,
// MapwhoLevelInfo +0x04, Manager +0x10, Cells (vector<vector<CThingMapCell>>) +0x14, Area +0x20,
// NoThings +0x30. CThingMapCell is one pointer (4 bytes).
// Shape: STLport vector size() is (finish - start) / sizeof(T) (`sar esi,2`), multiplied back
// (`lea eax,[eax+esi*4]`); the indexed element is `*(begin() + level)` (strength-reduced to
// a pointer stepping by 0xc), the loop counts the global down in edx.
namespace NThingMap {
struct CMapwhoNode;
struct CThingMapCell {
    CMapwhoNode* PFirstNode;
};
template<class T> struct CRetailVector {           // STLport vector: {start, finish, end_of_storage}
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) const { return *(begin() + n); }
};
extern long g_NoMapwhoLevels;                      // 0x013BA848
struct CThingMap {
    virtual void v00();
    CRetailVector<long>                          MapwhoLevelInfo;   // +0x04
    void*                                        Manager;           // +0x10
    CRetailVector< CRetailVector<CThingMapCell> > Cells;            // +0x14
    long GetMemoryFootprint();
};

long CThingMap::GetMemoryFootprint()
{
    long footprint = 0;
    for (long level = 0; level < g_NoMapwhoLevels; ++level)
        footprint += Cells[level].size() * sizeof(CThingMapCell);
    return footprint;
}
}