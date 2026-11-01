// Test for 0x00BDED60 CLocalDetailCacheMap::~CLocalDetailCacheMap: the palette's entry array, then the area update list are freed
// (only when allocated), then the QuadTree root element is destroyed.
#include <cstdio>
#include <cstdlib>
static int g_seq[4], g_n = 0, g_quadDtors = 0;
static void Log(int v) { if (g_n < 4) g_seq[g_n++] = v; }
template <class T> struct SVector {                         // models the 12-byte STLport vector of trivial elements
    T* Start; T* Finish; T* EndOfStorage; int Tag;
    ~SVector() { if (Start != 0) { std::free(Start); Log(Tag); } }
};
class C2DBoxI { public: int TLX, TLY, BRX, BRY; };
class CLocalDetailObjectCollectionType;
class CLocalDetailCacheMap { public:
    class CQuadTreeElement { public: virtual ~CQuadTreeElement() { ++g_quadDtors; Log(7); } };
    class CObjectTypeCollectionPalette { public: SVector<const CLocalDetailObjectCollectionType*> Entries; };
    ~CLocalDetailCacheMap();
    bool LockedFromMemoryRelocation;
    void* FileBlockManager;
    void* Map;
    CQuadTreeElement QuadTree;
    SVector<C2DBoxI> AreaUpdateList;
    CObjectTypeCollectionPalette CollectionTypePalette;
};
CLocalDetailCacheMap::~CLocalDetailCacheMap() { }
int main()
{
    {
        CLocalDetailCacheMap m;
        m.AreaUpdateList.Start = (C2DBoxI*)std::malloc(sizeof(C2DBoxI)); m.AreaUpdateList.Tag = 1;
        m.CollectionTypePalette.Entries.Start = (const CLocalDetailObjectCollectionType**)std::malloc(sizeof(void*)); m.CollectionTypePalette.Entries.Tag = 2;
    }
    if (g_n != 3 || g_seq[0] != 2 || g_seq[1] != 1 || g_seq[2] != 7 || g_quadDtors != 1) { std::printf("00bded60_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CLocalDetailCacheMap m; m.AreaUpdateList.Start = 0; m.CollectionTypePalette.Entries.Start = 0; }
    if (g_n != 1 || g_seq[0] != 7 || g_quadDtors != 2) { std::printf("00bded60_TEST FAIL\n"); return 1; }
    std::printf("00bded60_TEST PASS\n");
    return 0;
}