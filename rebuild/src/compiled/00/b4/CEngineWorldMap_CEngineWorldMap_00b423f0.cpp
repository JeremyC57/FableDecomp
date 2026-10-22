// CEngineWorldMap::CEngineWorldMap(const CEngineWorldMapSettings&) -- the engine world map: CEngineComponent base, empty map list / visible list / bank name, the static-map bank file, and defaults (32-unit map offsets, quality clamped to 0..10, minimum-rebuild flag from the settings)
// Leaf: ??0CEngineWorldMap
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B423F0, 223 bytes (+ int3 padding; `ret 4`, returns `this`).
// Label correction: catalogued as "CNavigationLayer::Initialise" (propagated). The vtable stored is 0x012A1F50 = CEngineWorldMap
//   (rtti-vtables.tsv) and the member stores follow Ego_r.pdb CEngineWorldMap.
// Callees: CEngineComponent::CEngineComponent 0x00B59710 (as the landed CEngineCamera ctor 0x00B31700), malloc 0x00BFEA0E (the
//   STLport list header node), CWideString::CWideString() 0x0099AED0, CBankFileAsync::CBankFileAsync 0x009D5F80 (landed) and the
//   empty `ret 4` setter 0x009D5230 called with true (as the landed CStaticMapBankFile ctor 0x00B3ECC0, vtable 0x012A1EE4).
// Layout (retail; Ego_r.pdb order, STLport 12-byte CArray / 4-byte list, a 0x164-byte bank file): Map +0x0c, MapArrayWidth +0x10,
//   MapArrayHeight +0x14, MapOffsetX +0x18, MapOffsetY +0x1c, MapList +0x20, VisibleMapList +0x2c, StaticMapBankName +0x30,
//   StaticMapFile +0x34, GameWorld +0x198, GameWorldMap +0x19c, StaticMapQuality +0x1a0, EnableMinimumStaticMapRebuild +0x1a4,
//   FirstWorldUpdate +0x1a5, StaticMapOpenState +0x1a8, DynamicUpdateEnabled +0x1ac, BuildRetailStaticMaps +0x1ad,
//   StaticMapBuildStatus +0x1b0, ChangedAreas +0x1b4. The retail class ends there (the PDB's second bank file and header
//   tables are not in this build). The settings record is modelled locally (quality +0x5c, flag word +0x60).
// Shape: member objects in declaration order (inline CArray/list/bank-file constructors), the scalars assigned in the body.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
struct CEngineComponent {                          // vtable 0x012A2544
    virtual void v00();
    long GlobalPointerAddress;                                      // +0x04
    bool m_enabled;                                // +0x08
    bool KeepAlive;                                      // +0x09
    CEngineComponent();                            // 0x00B59710
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
    CWideString();                                 // 0x0099AED0
};
struct RTL_CRITICAL_SECTION_ { void* DebugInfo; long LockCount; long RecursionCount; void* OwningThread; void* LockSemaphore; unsigned long SpinCount; };
class CBankFileAsync {                             // retail 0x164 bytes
public:
    virtual void v00();
    unsigned char _body[0x14c - 4];
    RTL_CRITICAL_SECTION_ CriticalSection;         // +0x14c
    CBankFileAsync();                              // 0x009D5F80
    void SetIsStaticMap(bool isStaticMap);         // 0x009D5230 (empty body: `ret 4`)
};
class CStaticMapBankFile : public CBankFileAsync { // vtable 0x012A1EE4
public:
    virtual void v00();
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
    virtual void v00();
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