// test for 0x00B41E50 CEngineMap::OpenRetailStaticMap -- seek sequence, info block use, counted-pointer hand-over, open state
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static long g_seeks[8]; static int g_nseeks = 0;
static int g_landscape = 0, g_detail = 0, g_destroyed = 0, g_closed = 0;
static void* g_lastManager = 0;
namespace std {
template <class _Tp> class allocator { };
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Pair> struct _Select1st { };
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base { _Value _M_value_field; };
template <class _Value, class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Value _M_data; };
template <class _Value> struct _Rb_tree_const_iterator {
    _Rb_tree_node_base* _M_node;
    __forceinline _Rb_tree_const_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    __forceinline const _Value* operator->() const { return &((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<_Value> _Node;
    typedef _Rb_tree_const_iterator<_Value> const_iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
    __forceinline const_iterator find(const _Key& __x) const { return const_iterator(_M_find(__x)); }
    _Rb_tree_node<_Value>* _M_find(const _Key& __k) const;   // 0x0042B467
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<_Key, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::const_iterator const_iterator;
    _Rep_type _M_t;
    __forceinline const_iterator find(const _Key& __x) const { return _M_t.find(__x); }
};

}
class CCharString { public: void* PStringData; };
class CCPPointerInfo { public:
    long  References;                                  // +0x00
    void (__fastcall* Destroy)(void* object);          // +0x04
    void* Object;                                      // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->References;
    }
    void Release()
    {
        if (Info) {
            --Info->References;
            if (Info->References == 0) {
                Info->Destroy(Info->Object);
                free(Info);
            }
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info;
        T* data = other.Data;
        if (Info != info) {
            Release();
            Data = data;
            Info = info;
            if (info)
                ++info->References;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        Release();
        Data = 0;
        Info = 0;
    }
};
class C3DVector { public: float X, Y, Z; };
class C3DBoundingBox { public: C3DVector MinCoord, MaxCoord; };
class CDataInputStream { public:
    virtual ~CDataInputStream();
    virtual void SetPosition(unsigned long position);   // slot 1 (+0x04)
};
class CBankFileAsyncEntry;
class CBankFileAsync { public:
    CCountedPointer<CBankFileAsyncEntry> GetAsyncEntry(unsigned long index);   // 0x009D58D0
};
class CStaticMapBankFile : public CBankFileAsync { public: };
class CEngineStaticMapFileBlockManager { public:
    CCountedPointer<CBankFileAsyncEntry> BankFileEntry;  // +0x00
    void SetBankFileEntry(CCountedPointer<CBankFileAsyncEntry> entry);   // 0x00BE03A0
};
class CEngineLandscapeMap { public:
    void OpenStaticMap(CEngineStaticMapFileBlockManager* fileBlockManager, CDataInputStream& stream);   // 0x00BDD0E0
};
class CLocalDetailCacheMap { public:
    void OpenStaticMap(CEngineStaticMapFileBlockManager* fileBlockManager, CDataInputStream& stream);   // 0x00BDF010
};
class CIDrawMap;
enum STATIC_MAP_OPEN_STATE { };
class CEngineMap { public:
    struct CStaticMapInfoBlock {
        long           VersionID;                      // +0x00
        long           BankFileIndex;                  // +0x04
        long           EdgeHeightFilePtr;              // +0x08
        long           EdgeHeightFileSize;             // +0x0c
        long           LandscapeMapPtr;                // +0x10
        long           LocalDetailMapPtr;              // +0x14
        long           ChecksumBlockFilePtr;           // +0x18
        long           ChecksumBlockFileSize;          // +0x1c
        long           MapWidth;                       // +0x20
        long           MapHeight;                      // +0x24
        long           WorldX;                         // +0x28
        long           WorldY;                         // +0x2c
        long           Quality;                        // +0x30
        long           ShorePointArraySize;            // +0x34
        long           ShorePointArrayStart;           // +0x38
        unsigned long  LevelChecksum;                  // +0x3c
        C3DBoundingBox CameraMapBounds;                // +0x40
        long           HeaderEndPtr;                   // +0x58
    };
    CEngineLandscapeMap*                 LandscapeMap;          // +0x00
    CLocalDetailCacheMap*                LocalDetailCacheMap;   // +0x04
    CIDrawMap*                           GameMap;               // +0x08
    C3DVector                            MapWorldPosition;      // +0x0c
    long                                 MapUID;                // +0x18
    CCharString                          StaticMapName;         // +0x1c
    STATIC_MAP_OPEN_STATE                StaticMapOpenState;    // +0x20
    C3DBoundingBox                       CameraMapBounds;       // +0x24
    CCountedPointer<CBankFileAsyncEntry> AsyncFileEntry;        // +0x3c
    CEngineStaticMapFileBlockManager     FileBlockManager;      // +0x44
    void CloseStaticMap();                                      // 0x00B3EF40
private:
    static void __fastcall ReadMapInfoBlock(CStaticMapInfoBlock& info, CDataInputStream& stream);   // 0x00B3EFA0
public:
    void OpenRetailStaticMap(CStaticMapBankFile& bankFile, CDataInputStream& stream, std::map<CCharString, long>& mapOffsets);
};
void CEngineMap::OpenRetailStaticMap(CStaticMapBankFile& bankFile, CDataInputStream& stream, std::map<CCharString, long>& mapOffsets)
{
    std::map<CCharString, long>::const_iterator it = mapOffsets.find(StaticMapName);
    CloseStaticMap();
    stream.SetPosition(it->second);
    CStaticMapInfoBlock info;
    ReadMapInfoBlock(info, stream);
    CameraMapBounds = info.CameraMapBounds;
    AsyncFileEntry = bankFile.GetAsyncEntry(info.BankFileIndex);
    FileBlockManager.SetBankFileEntry(AsyncFileEntry);
    stream.SetPosition(info.LandscapeMapPtr);
    LandscapeMap->OpenStaticMap(&FileBlockManager, stream);
    if (LocalDetailCacheMap) {
        stream.SetPosition(info.LocalDetailMapPtr);
        LocalDetailCacheMap->OpenStaticMap(&FileBlockManager, stream);
    }
    stream.SetPosition(info.HeaderEndPtr);
    StaticMapOpenState = (STATIC_MAP_OPEN_STATE)1;
}

typedef std::pair<const CCharString, long> MapValue;
typedef std::_Rb_tree_node<MapValue> MapNode;
static MapNode* g_found;
static const CCharString* g_lookedUp;
template <> MapNode* std::_Rb_tree<CCharString, MapValue, std::_Select1st<MapValue>, std::less<CCharString>, std::allocator<MapValue> >::_M_find(const CCharString& k) const
{
    g_lookedUp = &k;
    return g_found;
}
CDataInputStream::~CDataInputStream() { }
void CDataInputStream::SetPosition(unsigned long position) { g_seeks[g_nseeks++] = (long)position; }
static void __fastcall DestroyEntry(void*) { ++g_destroyed; }
static unsigned long g_entryIndex = 0;
static CBankFileAsyncEntry* g_entryObject = (CBankFileAsyncEntry*)&g_entryIndex;
CCountedPointer<CBankFileAsyncEntry> CBankFileAsync::GetAsyncEntry(unsigned long index)
{
    g_entryIndex = index;
    static unsigned char raw[sizeof(CCountedPointer<CBankFileAsyncEntry>)];
    CCountedPointer<CBankFileAsyncEntry>* p = (CCountedPointer<CBankFileAsyncEntry>*)raw;
    p->Data = g_entryObject;
    p->Info = (CCPPointerInfo*)malloc(sizeof(CCPPointerInfo));
    p->Info->References = 0;
    p->Info->Destroy = DestroyEntry;
    p->Info->Object = 0;
    return *p;                               // copy: one reference
}
void CEngineStaticMapFileBlockManager::SetBankFileEntry(CCountedPointer<CBankFileAsyncEntry> entry) { BankFileEntry = entry; }
void CEngineLandscapeMap::OpenStaticMap(CEngineStaticMapFileBlockManager* manager, CDataInputStream&) { ++g_landscape; g_lastManager = manager; }
void CLocalDetailCacheMap::OpenStaticMap(CEngineStaticMapFileBlockManager* manager, CDataInputStream&) { ++g_detail; g_lastManager = manager; }
void CEngineMap::CloseStaticMap() { ++g_closed; }
void __fastcall CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock& info, CDataInputStream&)
{
    memset(&info, 0, sizeof(info));
    info.BankFileIndex = 42;
    info.LandscapeMapPtr = 0x100;
    info.LocalDetailMapPtr = 0x200;
    info.HeaderEndPtr = 0x300;
    info.CameraMapBounds.MinCoord.X = -5.0f;
    info.CameraMapBounds.MaxCoord.Z = 7.0f;
}
int main()
{
    int ok = 1;
    static unsigned char mapRaw[sizeof(std::map<CCharString, long>)];
    std::map<CCharString, long>& offsets = *(std::map<CCharString, long>*)mapRaw;
    static unsigned char nodeRaw[sizeof(MapNode)];
    g_found = (MapNode*)nodeRaw;
    *(long*)&g_found->_M_value_field.second = 0x50;
    static CEngineLandscapeMap landscape;
    static CLocalDetailCacheMap detail;
    static CStaticMapBankFile bank;
    CDataInputStream stream;
    for (int pass = 0; pass < 2; ++pass) {
        static unsigned char engineMapRaw[sizeof(CEngineMap)];
        memset(engineMapRaw, 0, sizeof(engineMapRaw));
        CEngineMap& m = *(CEngineMap*)engineMapRaw;
        m.LandscapeMap = &landscape;
        m.LocalDetailCacheMap = pass == 0 ? &detail : 0;
        g_nseeks = 0; g_landscape = g_detail = 0;
        m.OpenRetailStaticMap(bank, stream, offsets);
        if (g_lookedUp != &m.StaticMapName || g_closed != pass + 1) ok = 0;
        if (pass == 0) {
            if (g_nseeks != 4 || g_seeks[0] != 0x50 || g_seeks[1] != 0x100 || g_seeks[2] != 0x200 || g_seeks[3] != 0x300) ok = 0;
            if (g_detail != 1) ok = 0;
        } else {
            if (g_nseeks != 3 || g_seeks[0] != 0x50 || g_seeks[1] != 0x100 || g_seeks[2] != 0x300 || g_detail != 0) ok = 0;
        }
        if (g_landscape != 1 || g_lastManager != &m.FileBlockManager) ok = 0;
        if (g_entryIndex != 42 || m.AsyncFileEntry.Data != g_entryObject || m.FileBlockManager.BankFileEntry.Info != m.AsyncFileEntry.Info) ok = 0;
        if (!m.AsyncFileEntry.Info || m.AsyncFileEntry.Info->References != 2) ok = 0;   // the map's and the manager's
        if (m.CameraMapBounds.MinCoord.X != -5.0f || m.CameraMapBounds.MaxCoord.Z != 7.0f) ok = 0;
        if ((long)m.StaticMapOpenState != 1 || g_destroyed != 0) ok = 0;
    }
    if (!ok) { printf("00b41e50_TEST FAIL\n"); return 1; }
    printf("00b41e50_TEST PASS\n");
    return 0;
}