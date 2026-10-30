// NThingMap::CThingMap::InitMapwhoLevels() -- size the per-level info and cell vectors to the mapwho level count, then for each level: a level of size -1 is a single cell covering everything (extents 1x1, no scale shift); otherwise its cell extents are the map area divided by the level's cell size (rounded up), its cell vector is sized to extents.X * extents.Y, and its scale shift and size are taken from the level table
// Grade: DIFFER(528v481) -- 190 difflines (parked 429v481/169 had the single-cell resize out of line); __forceinline single-arg resize with resize(n, const T&) under auto_inline(off) gives retail's callee set exactly (resize 0x85E870, 0x85ED20, _M_fill_insert 0x85E6C0 inline on the -1 path, resize 0x85E8D0 on the other). Left: frame 0x30 vs 0x24 (retail shares the info/vector temps and the cell temp with the erase counter), level-def row pointer in ebp, i in memory. Tried: STLport by-value resize (576, all inline), out-of-class resize without pragma (528, all inline). Next: temporaries in block scopes to share slots.
// Leaf: InitMapwhoLevels
// Callees: vector<CMapwhoLevelInfo>::resize @ 0x0085E870, vector<vector<CThingMapCell> >::resize @ 0x0085ED20,
//          vector<CThingMapCell>::_M_fill_insert @ 0x0085E6C0, vector<CThingMapCell>::resize @ 0x0085E8D0 (inlined for the
//          single-cell level); globals: level count 0x013BA848, level table 0x0137DAD8 (16-byte rows: size, scale shift).
// Layout: retail CThingMap (STLport vectors are 12 bytes): MapwhoLevelInfo +0x04, Manager +0x10, Cells +0x14, Area +0x20
//          (TopLeft +0x20/+0x24, BottomRight +0x28/+0x2c); CMapwhoLevelInfo (Ego_r.pdb, 20 bytes).
namespace NThingMap {

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned int size() const { return _M_finish - _M_start; }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    void _M_fill_insert(T* pos, unsigned int n, const T& x);
    T* erase(T* first, T* last)
    {
        T* result = first;
        T* source = last;
        for (int n = _M_finish - last; n > 0; --n) {
            *result = *source;
            ++source;
            ++result;
        }
        _M_finish = result;
        return first;
    }
    void insert(T* pos, unsigned int n, const T& x) { _M_fill_insert(pos, n, x); }
    void resize(unsigned int n, const T& x);
    __forceinline void resize(unsigned int n)
    {
        T x;
        if (n < size())
            erase(begin() + n, end());
        else
            insert(end(), n - size(), x);
    }
};
#pragma auto_inline(off)
template <class T> void vector<T>::resize(unsigned int n, const T& x)
{
    if (n < size())
        erase(begin() + n, end());
    else
        insert(end(), n - size(), x);
}
#pragma auto_inline(on)

struct C2DCoordI { long X, Y; };
struct C2DBoxI { C2DCoordI TopLeft; C2DCoordI BottomRight; };
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() { }
};
struct CMapwhoLevelInfo {
    long        Size;                                   // +0x00
    C2DExtentsI Extents;                                // +0x04
    long        ScaleShift;                             // +0x0c
    long        NoThings;                               // +0x10
    CMapwhoLevelInfo() { }
};
struct CThingMapNode;
struct CThingMapCell {
    CThingMapNode* PFirst;
    CThingMapCell() : PFirst(0) { }
};
struct SMapwhoLevelDef { long Size; long ScaleShift; long _f08; long _f0c; };
extern long g_NoMapwhoLevels;                           // 0x013BA848
extern SMapwhoLevelDef g_MapwhoLevelDefs[];             // 0x0137DAD8
struct CThingMapManager;

class CThingMap {
public:
    virtual void v00();
    vector<CMapwhoLevelInfo>          MapwhoLevelInfo;  // +0x04
    CThingMapManager*                 Manager;          // +0x10
    vector< vector<CThingMapCell> >   Cells;            // +0x14
    C2DBoxI                           Area;             // +0x20
    long                              NoThings;         // +0x30
protected:
    void InitMapwhoLevels();
};

void CThingMap::InitMapwhoLevels()
{
    MapwhoLevelInfo.resize(g_NoMapwhoLevels, CMapwhoLevelInfo());
    Cells.resize(g_NoMapwhoLevels, vector<CThingMapCell>());
    for (long i = 0; i < g_NoMapwhoLevels; ++i) {
        MapwhoLevelInfo[i].NoThings = 0;
        long size = g_MapwhoLevelDefs[i].Size;
        if (size == -1) {
            MapwhoLevelInfo[i].Size = -1;
            MapwhoLevelInfo[i].Extents.X = 1;
            MapwhoLevelInfo[i].Extents.Y = 1;
            MapwhoLevelInfo[i].ScaleShift = -1;
            Cells[i].resize(1);
        } else {
            MapwhoLevelInfo[i].Extents.X = (Area.BottomRight.X - Area.TopLeft.X) / size;
            MapwhoLevelInfo[i].Extents.Y = (Area.BottomRight.Y - Area.TopLeft.Y) / size;
            if (Area.BottomRight.X - Area.TopLeft.X > MapwhoLevelInfo[i].Extents.X * size)
                ++MapwhoLevelInfo[i].Extents.X;
            if (Area.BottomRight.Y - Area.TopLeft.Y > MapwhoLevelInfo[i].Extents.Y * size)
                ++MapwhoLevelInfo[i].Extents.Y;
            Cells[i].resize(MapwhoLevelInfo[i].Extents.Y * MapwhoLevelInfo[i].Extents.X, CThingMapCell());
            MapwhoLevelInfo[i].ScaleShift = g_MapwhoLevelDefs[i].ScaleShift;
            MapwhoLevelInfo[i].Size = g_MapwhoLevelDefs[i].Size;
        }
    }
}

}
