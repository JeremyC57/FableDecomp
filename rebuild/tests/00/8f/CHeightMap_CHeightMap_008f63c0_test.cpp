// Behaviour test for 008f63c0: src/008f63c0.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CHeightMap::CHeightMap(CWorld* world, CMap* map, const SHeightMapSize& size)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CHeightMap::CHeightMap(CWorld* world, CMap* map, const SHeightMapSize& size) -- a landscape height map for one map: world, map and world-seed pointers, the cell counts (one extra row and column of cells), and when (SizeX+1)*(SizeY+1) fits a long, the cell array sized to it, no pending update areas and a cleared map
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

// ---- stubs and driver ----

inline void* operator new(unsigned int, void* p) { return p; }
static int g_base = 0, g_baseDtor = 0;
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_base; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_baseDtor; }
static unsigned int g_resizeN = 0; static int g_resizes = 0; static CHeightMapCellVector* g_resizeThis;
void CHeightMapCellVector::resize(unsigned int n, const CHeightMapCell&) { g_resizeN = n; ++g_resizes; g_resizeThis = this; }
static int g_clears = 0; static CHeightMap* g_clearThis;
void CHeightMap::Clear() { ++g_clears; g_clearThis = this; }
int main()
{
    unsigned long seed = 7;
    SHeightMapSize size; size.SizeX = 4; size.SizeY = 3; size.WorldSeed = &seed;
    CWorld* world = (CWorld*)0x10; CMap* map = (CMap*)0x20;
    static unsigned char store[sizeof(CHeightMap)]; memset(store, 0xcd, sizeof store);
    CHeightMap* h = new (store) CHeightMap(world, map, size);
    CHECK(g_base == 1); CHECK(h->World == world); CHECK(h->Map == map); CHECK(h->WorldSeed == &seed);
    CHECK(h->SizeX == 4); CHECK(h->SizeY == 3); CHECK(h->SizeXIncludingExtraCells == 5); CHECK(h->SizeYIncludingExtraCells == 4);
    CHECK(h->Cells._M_start == 0); CHECK(h->Cells._M_finish == 0); CHECK(h->Cells._M_end_of_storage._M_data == 0);
    CHECK(g_resizes == 1); CHECK(g_resizeN == 20); CHECK(g_resizeThis == &h->Cells);
    CHECK(h->NoMapUpdateAreas == 0); CHECK(g_clears == 1); CHECK(g_clearThis == h);
    // an unsafe cell count: no allocation, no clear
    size.SizeX = 0x10000; size.SizeY = 0x10000;
    static unsigned char store2[sizeof(CHeightMap)]; memset(store2, 0xcd, sizeof store2);
    CHeightMap* big = new (store2) CHeightMap(world, map, size);
    CHECK(g_resizes == 1); CHECK(g_clears == 1); CHECK(big->SizeXIncludingExtraCells == 0x10001); CHECK(big->NoMapUpdateAreas == (long)0xcdcdcdcd);
    if (fails == 0) printf("008f63c0_TEST PASS\n"); else printf("008f63c0_TEST FAIL\n");
    return fails;
}