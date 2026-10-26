// Behaviour test for 00515250 (_Destroy_range_CMapInfo): each CMapInfo destroys its three strings and releases its counted map (the last reference destroys the map and frees the block).
#include <cstdio>
#include <new>
#ifdef _MSC_VER
typedef unsigned __int64 TStamp;
#else
typedef unsigned long long TStamp;
#endif
static int g_strings = 0;
static int g_mapsDestroyed = 0;
class CCharString { public:
    void* m_pData;
    CCharString() : m_pData(0) { }
    ~CCharString() { ++g_strings; }                 // stands in for 0x0099EAE0
};
struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
class CMap { public: int id; };
static void DestroyMap(void* p) { ++g_mapsDestroyed; delete static_cast<CMap*>(p); }
struct CCountedPointerCMap {
    CMap* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointerCMap() : m_pObject(0), m_pCount(0) { }
    ~CCountedPointerCMap()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
class C2DBoxI { public: long x0, y0, x1, y1; };
class CMapInfo { public:
    C2DBoxI Area;
    CCountedPointerCMap PMap;
    CCharString LevelName;
    CCharString LevelScriptName;
    unsigned long MapUID;
    bool Used, Placed, Activated, Loading, LoadedOnPlayerProximity;
    CCharString SerialiseScript;
    TStamp LevStamp;
    TStamp TngStamp;
    bool IsSea;
};
struct SAllocator { };
void _Destroy_range_CMapInfo(CMapInfo* first, CMapInfo* last, SAllocator)
{
    for (; first != last; ++first)
        first->~CMapInfo();
}
union UStorage { double d; TStamp t; char bytes[sizeof(CMapInfo) * 3]; };
int main()
{
    UStorage storage;
    CMapInfo* maps = reinterpret_cast<CMapInfo*>(storage.bytes);
    for (int i = 0; i < 3; ++i)
        new (&maps[i]) CMapInfo();
    SCountBlock* sole = new SCountBlock;
    sole->m_references = 1; sole->m_pfnDestroy = DestroyMap; sole->m_pObject = new CMap;
    SCountBlock shared;
    shared.m_references = 2; shared.m_pfnDestroy = DestroyMap; shared.m_pObject = 0;
    maps[0].PMap.m_pObject = static_cast<CMap*>(sole->m_pObject); maps[0].PMap.m_pCount = sole;
    maps[1].PMap.m_pCount = &shared;
    _Destroy_range_CMapInfo(maps, maps, SAllocator());
    if (g_strings != 0) { std::printf("00515250_TEST FAIL\n"); return 1; }
    _Destroy_range_CMapInfo(maps, maps + 3, SAllocator());
    if (g_strings != 9 || g_mapsDestroyed != 1 || shared.m_references != 1) { std::printf("00515250_TEST FAIL\n"); return 1; }
    if (maps[0].PMap.m_pCount != 0 || maps[0].PMap.m_pObject != 0 || maps[1].PMap.m_pCount != 0) { std::printf("00515250_TEST FAIL\n"); return 1; }
    std::printf("00515250_TEST PASS\n");
    return 0;
}