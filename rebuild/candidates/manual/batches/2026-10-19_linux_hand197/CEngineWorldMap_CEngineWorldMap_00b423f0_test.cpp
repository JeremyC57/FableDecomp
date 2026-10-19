#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
// Behaviour test for 00b423f0 CEngineWorldMap::CEngineWorldMap: base and member constructors run once each (the list header is
// self-linked, the bank file is flagged static), the quality is clamped to 0..10 and the flag bit and defaults are set.
static int g_component = 0, g_strings = 0, g_banks = 0, g_staticSet = 0;
struct CEngineComponent {                          // vtable 0x012A2544
    virtual void v00() {}
    long m_4;                                      // +0x04
    bool m_enabled;                                // +0x08
    bool m_9;                                      // +0x09
    CEngineComponent() : m_4(7), m_enabled(true), m_9(true) { ++g_component; }
};
template <class T> struct CArray {                 // retail CArray (STLport vector): first / last / end
    T* m_first;
    T* m_last;
    T* m_end;
    CArray() : m_first(0), m_last(0), m_end(0) {}
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct list {                   // STLport list, 4 bytes
    _List_node<T>* _M_node;
    list() : _M_node(0)
    {
        _List_node<T>* node = (_List_node<T>*)malloc(sizeof(_List_node<T>));
        node->_M_next = node;
        node->_M_prev = node;
        _M_node = node;
    }
};
struct CWideString {
    void* m_pData;
    CWideString() : m_pData(0) { ++g_strings; }
};
struct RTL_CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
class CBankFileAsync {                             // retail 0x164 bytes
public:
    virtual void v00() {}
    unsigned char _body[0x14c - 4];
    RTL_CRITICAL_SECTION_ CriticalSection;         // +0x14c
    CBankFileAsync() { ++g_banks; }
    void SetIsStaticMap(bool isStaticMap) { g_staticSet = isStaticMap ? 1 : 2; }
};
class CStaticMapBankFile : public CBankFileAsync { // vtable 0x012A1EE4
public:
    virtual void v00() {}
    CStaticMapBankFile() { SetIsStaticMap(true); }
};
struct CEngineWorldMapSettings {                   // local model of the settings record passed in
    unsigned char _pad0[0x5c];
    long          StaticMapQuality;                // +0x5c
    unsigned long Flag0 : 1;                       // +0x60 bit 0
    unsigned long EnableMinimumStaticMapRebuild : 1;   // +0x60 bit 1
};
struct CEngineMap;
struct C2DBoxI;
class CIDrawWorld;
class CIDrawWorldMap;
class CEngineWorldMap : public CEngineComponent {  // vtable 0x012A1F50
public:
    short*               Map;                      // +0x0c
    long                 MapArrayWidth;            // +0x10
    long                 MapArrayHeight;           // +0x14
    long                 MapOffsetX;               // +0x18
    long                 MapOffsetY;               // +0x1c
    CArray<CEngineMap*>  MapList;                  // +0x20
    list<CEngineMap*>    VisibleMapList;           // +0x2c
    CWideString          StaticMapBankName;        // +0x30
    CStaticMapBankFile   StaticMapFile;            // +0x34
    CIDrawWorld*         GameWorld;                // +0x198
    CIDrawWorldMap*      GameWorldMap;             // +0x19c
    long                 StaticMapQuality;         // +0x1a0
    bool                 EnableMinimumStaticMapRebuild;   // +0x1a4
    bool                 FirstWorldUpdate;         // +0x1a5
    long                 StaticMapOpenState;       // +0x1a8 STATIC_MAP_OPEN_STATE
    bool                 DynamicUpdateEnabled;     // +0x1ac
    bool                 BuildRetailStaticMaps;    // +0x1ad
    long                 StaticMapBuildStatus;     // +0x1b0 STATIC_MAP_BUILD_STATUS
    CArray<C2DBoxI>      ChangedAreas;             // +0x1b4
    virtual void v00() {}
    CEngineWorldMap(const CEngineWorldMapSettings& settings);
};

CEngineWorldMap::CEngineWorldMap(const CEngineWorldMapSettings& settings)
{
    StaticMapQuality = settings.StaticMapQuality;
    if (StaticMapQuality < 0)
        StaticMapQuality = 0;
    else if (StaticMapQuality > 10)
        StaticMapQuality = 10;
    MapOffsetX = 32;
    MapOffsetY = 32;
    DynamicUpdateEnabled = false;
    BuildRetailStaticMaps = false;
    MapArrayWidth = 0;
    MapArrayHeight = 0;
    Map = 0;
    GameWorld = 0;
    GameWorldMap = 0;
    EnableMinimumStaticMapRebuild = settings.EnableMinimumStaticMapRebuild;
    StaticMapOpenState = 0;
    StaticMapBuildStatus = 0;
    FirstWorldUpdate = true;
}

static CEngineWorldMap* Build(long quality, bool flag)
{
    static CEngineWorldMapSettings settings;
    std::memset(&settings, 0, sizeof(settings));
    settings.StaticMapQuality = quality;
    settings.EnableMinimumStaticMapRebuild = flag ? 1 : 0;
    settings.Flag0 = 1;
    void* mem = std::malloc(sizeof(CEngineWorldMap));
    std::memset(mem, 0xcd, sizeof(CEngineWorldMap));
    return new (mem) CEngineWorldMap(settings);
}
int main()
{
    CEngineWorldMap* m = Build(5, true);
    bool ok = g_component == 1 && g_strings == 1 && g_banks == 1 && g_staticSet == 1;
    ok = ok && m->StaticMapQuality == 5 && m->EnableMinimumStaticMapRebuild && m->FirstWorldUpdate;
    ok = ok && m->Map == 0 && m->MapArrayWidth == 0 && m->MapArrayHeight == 0 && m->MapOffsetX == 32 && m->MapOffsetY == 32;
    ok = ok && m->MapList.m_first == 0 && m->MapList.m_last == 0 && m->MapList.m_end == 0;
    ok = ok && m->ChangedAreas.m_first == 0 && m->ChangedAreas.m_last == 0 && m->ChangedAreas.m_end == 0;
    ok = ok && m->VisibleMapList._M_node != 0 && m->VisibleMapList._M_node->_M_next == m->VisibleMapList._M_node && m->VisibleMapList._M_node->_M_prev == m->VisibleMapList._M_node;
    ok = ok && m->GameWorld == 0 && m->GameWorldMap == 0 && m->StaticMapOpenState == 0 && m->StaticMapBuildStatus == 0;
    ok = ok && !m->DynamicUpdateEnabled && !m->BuildRetailStaticMaps && m->m_4 == 7;
    ok = ok && Build(-3, false)->StaticMapQuality == 0 && !Build(-3, false)->EnableMinimumStaticMapRebuild;
    ok = ok && Build(11, false)->StaticMapQuality == 10 && Build(10, false)->StaticMapQuality == 10 && Build(0, false)->StaticMapQuality == 0;
    if (!ok) { std::printf("00b423f0_TEST FAIL\n"); return 1; }
    std::printf("00b423f0_TEST PASS\n");
    return 0;
}
