// CHeightMap::CHeightMap(CWorld* world, CMap* map, const SHeightMapSize& size) -- a landscape height map for one map: world, map and world-seed pointers, the cell counts (one extra row and column of cells), and when (SizeX+1)*(SizeY+1) fits a long, the cell array sized to it, no pending update areas and a cleared map
// Copy constructor: the C2DBoxI model DECLARES its memberwise copy constructor (retail's header class has one); with the
// implicit copy VC7.1 schedules the copies differently (found by the retry-10 copy-constructor sweep).
// Leaf: ??0CHeightMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x008F63C0, 229 bytes, `ret 0Ch`; built by 0x0081ED90 (`push 214h`) from the owning
//   CMap's size words (+0x1d14/+0x1d18) and its world-seed pointer (+0x24), passed by address.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, _allmul @ 0x00BFEB50 (the 64-bit cell count),
//   vector<CHeightMapCell>::resize(n, const T&) @ 0x008F6E30 (label "vector<CBrainBehaviour>::resize" is propagated),
//   CHeightMap::Clear @ 0x008F6050.
// Layout (Ego_r.pdb CHeightMap; retail 0x214 = PDB - 4 for the 12-byte STLport vector): World +4, Map +8, WorldSeed +0xc,
//   Cells +0x10, MapUpdateAreas (C2DBoxI[30], no ctor) +0x1c, NoMapUpdateAreas +0x1fc, Changed +0x200, SizeX +0x204,
//   SizeY +0x208, SizeXIncludingExtraCells +0x20c, SizeYIncludingExtraCells +0x210. Vtable 0x01288BEC (CHeightMap).
// Shape: the count guard is the inline 64-bit `0 <= n <= 0x7FFFFFFF` predicate over `((__int64)SizeY + 1) *
//   ((__int64)x + 1)` with x a named local (homed in the dead `size` argument slot); the resize takes a frame
//   CHeightMapCell() whose user constructor is empty; NoMapUpdateAreas and Clear() run only inside the guard.
class CWorld;
class CMap;

struct CBaseClassNonCopyable {
    virtual ~CBaseClassNonCopyable();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};

struct SHeightMapSize {                            // the map's cell counts and its world seed
    long           SizeX;
    long           SizeY;
    unsigned long* WorldSeed;
};

class CHeightMapCell { public:                            // 12 bytes
    float         Height;
    unsigned char EngineThemePaletteIndex[3];
    unsigned char EngineBlend[2];
    unsigned char Changed : 1;
    unsigned char Passable : 1;
    unsigned char ShorePoint : 1;
    unsigned char CameraPassable : 1;
    unsigned char NavPreferred : 1;
    CHeightMapCell() { }
};

template <class T> struct allocator { allocator() { } };
template <class T, class A> struct _STLP_alloc_proxy : A {
    T _M_data;
    _STLP_alloc_proxy(const A& a, T p) : A(a), _M_data(p) { }
};
template <class T> struct _Vector_base {
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage;
    _Vector_base(const allocator<T>& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, (T*)0) { }
};
struct CHeightMapCellVector : _Vector_base<CHeightMapCell> {   // STLport vector<CHeightMapCell> (12 bytes in retail)
    CHeightMapCellVector() : _Vector_base<CHeightMapCell>(allocator<CHeightMapCell>()) { }
    void resize(unsigned int n, const CHeightMapCell& value);   // 0x008F6E30
};

class C2DBoxI { public: long MinX, MinY, MaxX, MaxY;
    C2DBoxI() { } C2DBoxI(const C2DBoxI& o) : MinX(o.MinX), MinY(o.MinY), MaxX(o.MaxX), MaxY(o.MaxY) { }
};

static inline bool IsSafeCount(__int64 count)
{
    return count >= 0 && count <= 0x7FFFFFFF;
}

class CHeightMap : public CBaseClassNonCopyable { public:        // vtable 0x01288BEC
    CWorld*              World;                    // +0x004
    CMap*                Map;                      // +0x008
    unsigned long*       WorldSeed;                // +0x00c
    CHeightMapCellVector Cells;                    // +0x010
    C2DBoxI              MapUpdateAreas[30];       // +0x01c
    long                 NoMapUpdateAreas;         // +0x1fc
    bool                 Changed;                  // +0x200
    long                 SizeX;                    // +0x204
    long                 SizeY;                    // +0x208
    long                 SizeXIncludingExtraCells; // +0x20c
    long                 SizeYIncludingExtraCells; // +0x210
    CHeightMap(CWorld* world, CMap* map, const SHeightMapSize& size);
    void Clear();                                  // 0x008F6050
};

CHeightMap::CHeightMap(CWorld* world, CMap* map, const SHeightMapSize& size)
    : World(world)
    , Map(map)
    , WorldSeed(size.WorldSeed)
    , SizeX(size.SizeX)
    , SizeY(size.SizeY)
    , SizeXIncludingExtraCells(size.SizeX + 1)
    , SizeYIncludingExtraCells(size.SizeY + 1)
{
    long x = size.SizeX;
    if (IsSafeCount(((__int64)size.SizeY + 1) * ((__int64)x + 1))) {
        Cells.resize((size.SizeY + 1) * (x + 1), CHeightMapCell());
        NoMapUpdateAreas = 0;
        Clear();
    }
}