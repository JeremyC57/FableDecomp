// CWorldMap::~CWorldMap() -- close and drop the level WAD bank, remove every map but the first from the engine, then destroy the members (intelligent pointers, counted pointers, vectors, regions, map infos, the scripted-brush map, the thing map manager)
// Leaf: ??1CWorldMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00506940, 615 bytes (624 with int3 padding).
// Shape: the bank release nulls Info then Data unconditionally inside the Data test; CRegion range destruction goes
//   through an inline (first, last) helper so the end pointer is cached (retail pushes ebx around that loop only); the
//   CMapInfo range helper takes a default allocator temporary with a user ctor (no zero store); the map header free is
//   null-checked.
// Callees: CTBaseSingleton<CEngineManager>::Get 0x00436FB0 (__fastcall), CEngineManager::RemoveMap(long) 0x006403D0,
//   ~CBaseIntelligentPointer 0x00A01C10, operator delete 0x00BFE9BC, free 0x00BFEA14, ~CRegion 0x0051A110,
//   _Destroy_range<CMapInfo> 0x00515250 (__fastcall), _Rb_tree::_M_erase 0x0043020A, ~CBase 0x0099A300.
// Layout (retail, 12-byte STLport containers): vptrs +0x00 (CBase) / +0x04 (second base), PThingMapManager +0x10,
//   LoadedScriptedMapBrushes +0x14 (map), Maps +0x20, Regions +0x2C (0x58-byte CRegion), RegionTime +0x38,
//   PRegionGraph +0x44, MapPlacement +0x6C, +0x78, +0x84, +0x90 vectors, LevelWADBank +0xA0, +0xB0 vector,
//   +0xBC counted pointer, +0xC4 / +0xCC intelligent pointers.
#include <stdlib.h>
void operator delete(void* p);                         // 0x00BFE9BC

struct CBase {
    virtual ~CBase();                                  // 0x0099A300
    CBase();                                           // 0x0099A2F0
};
struct CWorldMapBase2 {
    virtual void v00();
    virtual ~CWorldMapBase2() {}
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
    void Release()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Info = 0;
        Data = 0;
    }
    T* operator->() const { return Data; }
};
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    ~CRetailVector() { if (_M_start) free(_M_start); }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    ~CBaseIntelligentPointer();                        // 0x00A01C10
};
struct CRegion {
    unsigned char _body[0x58];
    ~CRegion();                                        // 0x0051A110
};
static inline void DestroyRange(CRegion* first, CRegion* last)
{
    for (; first != last; ++first)
        first->~CRegion();
}
struct CRegionVector {
    CRegion* _M_start;
    CRegion* _M_finish;
    CRegion* _M_end_of_storage;
    ~CRegionVector()
    {
        DestroyRange(_M_start, _M_finish);
        if (_M_start) free(_M_start);
    }
};
struct CMapInfo { unsigned char _body[0x48]; };
struct SAllocTag { SAllocTag() {} };
void __fastcall DestroyMapInfoRange(CMapInfo* first, CMapInfo* last, const SAllocTag& alloc);   // 0x00515250
struct CMapInfoArray {
    CMapInfo* _M_start;
    CMapInfo* _M_finish;
    CMapInfo* _M_end_of_storage;
    long size() const { return _M_finish - _M_start; }
    ~CMapInfoArray()
    {
        DestroyMapInfoRange(_M_start, _M_finish, SAllocTag());
        if (_M_start) free(_M_start);
    }
};
struct SRbNode { char _M_color; SRbNode* _M_parent; SRbNode* _M_left; SRbNode* _M_right; long key; long value; };
struct CScriptedBrushMap {
    SRbNode* _M_header;
    long     _M_node_count;
    long     _M_compare;
    void _M_erase(SRbNode* node);                      // 0x0043020A
    ~CScriptedBrushMap()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_header->_M_parent);
            _M_header->_M_left = _M_header;
            _M_header->_M_parent = 0;
            _M_header->_M_right = _M_header;
            _M_node_count = 0;
        }
        if (_M_header) free(_M_header);
    }
};
struct CThingMapManager { virtual ~CThingMapManager(); };
template <class T> struct CAutoPtr {
    T* P;
    ~CAutoPtr() { if (P) delete P; }
};
struct CEngineManager {
    static CEngineManager* __fastcall Get();           // 0x00436FB0
    void RemoveMap(long map);                          // 0x006403D0
};
struct CWADBank {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void Close();                              // slot 5 (+0x14)
};
struct CRegionGraph;
struct CThing;

class CWorldMap : public CBase, public CWorldMapBase2 {
public:
    ~CWorldMap();
    void*                          World;              // +0x08
    void*                          Component;          // +0x0C
    CAutoPtr<CThingMapManager>     PThingMapManager;   // +0x10
    CScriptedBrushMap              LoadedScriptedMapBrushes; // +0x14
    CMapInfoArray                  Maps;               // +0x20
    CRegionVector                  Regions;            // +0x2C
    CRetailVector<long>            RegionTime;         // +0x38
    CCountedPointer<CRegionGraph>  PRegionGraph;       // +0x44
    bool                           SoundThemesDisabled; // +0x4C
    long                           Granularity;        // +0x50
    long                           GranularityShift;   // +0x54
    long                           MapPlacementPitch;  // +0x58
    long                           Area[4];            // +0x5C
    CRetailVector<unsigned short>  MapPlacement;       // +0x6C
    CRetailVector<unsigned char>   Vec78;              // +0x78
    CRetailVector<long>            Vec84;              // +0x84
    CRetailVector<long>            Vec90;              // +0x90
    long                           f9C;                // +0x9C
    CCountedPointer<CWADBank>      LevelWADBank;       // +0xA0
    bool                           UseWADFiles;        // +0xA8
    long                           fAC;                // +0xAC
    CRetailVector<long>            VecB0;              // +0xB0
    CCountedPointer<CThing>        CPtrBC;             // +0xBC
    CBaseIntelligentPointer        IPtrC4;             // +0xC4
    CBaseIntelligentPointer        IPtrCC;             // +0xCC
};

CWorldMap::~CWorldMap()
{
    if (LevelWADBank.Data)
    {
        LevelWADBank->Close();
        LevelWADBank.Release();
    }
    for (long i = 1; i < Maps.size(); ++i)
        CEngineManager::Get()->RemoveMap(i);
}
