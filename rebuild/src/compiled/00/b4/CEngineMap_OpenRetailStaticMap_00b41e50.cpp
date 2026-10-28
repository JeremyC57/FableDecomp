// CEngineMap::OpenRetailStaticMap(CStaticMapBankFile& bankFile, CDataInputStream& stream, std::map<CCharString, long>& mapOffsets) -- open this map's static data from the retail level file: seek to the map's header offset (looked up by the static map name), read the info block, take the camera bounds, bind the bank entry to the file-block manager, open the landscape map and (when present) the local-detail cache map at their offsets, seek past the header and mark the map open
// Leaf: ?OpenRetailStaticMap@CEngineMap@@QAEXAAVCStaticMapBankFile@@AAVCDataInputStream@@AAV?$map@VCCharString@@JU?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@J@std@@@3@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B41E50, length 325 bytes, `ret 0Ch`.
// Name: FableWin.pdb (above; catalog label OpenRetailStaticMap agrees). Parameter names are descriptive.
// Callees: STLport 4.6 _Rb_tree<CCharString, pair<const CCharString, long>, ...>::_M_find(const CCharString&) const 0x0042B467
//   (COMDAT-shared by the CCharString-keyed trees; the mapped long at node +0x14), CEngineMap::CloseStaticMap 0x00B3EF40,
//   CEngineMap::ReadMapInfoBlock(CStaticMapInfoBlock&, CDataInputStream&) 0x00B3EFA0 (FableWin `?ReadMapInfoBlock@CEngineMap@@CIX
//   AAUCStaticMapInfoBlock@1@AAVCDataInputStream@@@Z`: private static __fastcall, nested struct), CBankFileAsync::GetAsyncEntry
//   (unsigned long) 0x009D58D0 (FableWin `?GetAsyncEntry@CBankFileAsync@@QAE?AV?$CCountedPointer@VCBankFileAsyncEntry@@@@K@Z`;
//   catalog "CEngineWorldMap::CreateNewBankMap"; CStaticMapBankFile derives from CBankFileAsync at offset 0, RTTI),
//   CEngineStaticMapFileBlockManager::SetBankFileEntry(CCountedPointer<CBankFileAsyncEntry>) 0x00BE03A0 (by value),
//   CEngineLandscapeMap::OpenStaticMap 0x00BDD0E0 / CLocalDetailCacheMap::OpenStaticMap 0x00BDF010 (FableWin: manager by pointer,
//   stream by reference), operator delete 0x00BFE9BC; the stream's virtual slot 1 (+0x04), taken as CDataInputStream::SetPosition
//   (unsigned long) (FableWin virtual `?SetPosition@CDataInputStream@@UAEXK@Z`; the slot number itself is probable).
// Layout: Ego_r.pdb CEngineMap (LandscapeMap +0x00, LocalDetailCacheMap +0x04, StaticMapName +0x1C, StaticMapOpenState +0x20,
//   CameraMapBounds +0x24, AsyncFileEntry +0x3C, FileBlockManager +0x44 -- equal to retail here) and CEngineMap::CStaticMapInfoBlock
//   (0x5C bytes: BankFileIndex +0x04, LandscapeMapPtr +0x10, LocalDetailMapPtr +0x14, CameraMapBounds +0x40, HeaderEndPtr +0x58).
//   No engine/ header exists for CEngineMap; CLocalDetailCacheMap derives from engine/CLocalDetailCacheMap.h; CDataInputStream is
//   modelled locally (its header flattens the vptr into a data member and SetPosition is virtual). The STATIC_MAP_OPEN_STATE enumerator 1 has no recovered name (cast literal).
// Shape: the find iterator is read after CloseStaticMap (`mov eax,[ebx+14h]`); the counted pointer's operator= reads the source's
//   Info and Data into locals before the compare (retail holds them in ebx/ebp across the release); the returned temporary dies
//   right after the assignment (its dtor's zero stores are dead); SetBankFileEntry gets a by-value copy (add-reference).
#define CLocalDetailCacheMap CLocalDetailCacheMap_Header
#include "engine/CLocalDetailCacheMap.h"
#undef CLocalDetailCacheMap
void __cdecl operator delete(void* block);             // 0x00BFE9BC
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
                operator delete(Info);
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
class CLocalDetailCacheMap : public CLocalDetailCacheMap_Header { public:   // engine/CLocalDetailCacheMap.h layout
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