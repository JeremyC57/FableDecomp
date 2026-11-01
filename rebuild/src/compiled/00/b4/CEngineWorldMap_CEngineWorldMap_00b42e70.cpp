// CEngineWorldMap::~CEngineWorldMap() -- close the static map file and delete every engine map, then destroy the members (ChangedAreas, StaticMapFile, StaticMapBankName, VisibleMapList, MapList) and run ~CEngineComponent
// Leaf: ??1CEngineWorldMap@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B42E70, 88 bytes, thiscall, tail `jmp ~CEngineComponent`.
// Identity (proven): stores vtable 0x012A1F50 first (RTTI .?AVCEngineWorldMap@@, rtti-vtables.tsv); sole caller is that
//   vtable's slot-0 `??_G` 0x00B44440. Ego_r.pdb (0x009E8650) and FableWin.pdb (0x02D5F5F0) spell ??1CEngineWorldMap@@UAE@XZ.
// Callees: CEngineWorldMap::CloseStaticMapFile 0x00B40000 (Ego_r ?CloseStaticMapFile@CEngineWorldMap@@QAEXXZ: the body closes
//   every map's static map, the water renderer's and the StaticMapFile, then clears StaticMapOpenState), the private
//   CEngineWorldMap::DeleteMap() 0x00B42C70 (Ego_r / FableWin ?DeleteMap@CEngineWorldMap@@AAEXXZ, bound; it calls
//   DeleteMap(long) per index), free 0x00BFEA14, CBankFileAsync::~CBankFileAsync 0x009D6490 (the implicit
//   ~CStaticMapBankFile adds nothing: its own `??_G` 0x00B43320 calls 0x009D6490 too), CWideString::~CWideString 0x0099B510,
//   the VisibleMapList's out-of-line STLport list destructor 0x00B43BB0, CEngineComponent::~CEngineComponent 0x00B59730.
// Shape: user destructor `{ CloseStaticMapFile(); DeleteMap(); }`; members in reverse order.
// Layout (retail offsets from this body and the landed 0x00B40000 / 0x00B42C70): CEngineComponent 0xC, Map +0x0c, MapList
//   (CArray<CEngineMap*>, STLport vector) +0x20, VisibleMapList (STLport list) +0x2c, StaticMapBankName +0x30, StaticMapFile
//   (CStaticMapBankFile : CBankFileAsync, 0x164) +0x34, ... ChangedAreas (CArray<C2DBoxI>) +0x1b4; this build ends there (the
//   Ego_r members after ChangedAreas are not destroyed). engine/CEngineWorldMap.h flattens the vptr and keeps those later
//   Ego_r members (header debt).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

class CIDrawWorld;
class CIDrawWorldMap;
class CEngineMap;
class CWideStringData;

class CWideString {
public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};

class C2DBoxI {
public:
    long X0, Y0, X1, Y1;
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef allocator<_Node> _Node_allocator_type;
    ~_List_base() { clear(); _M_node.deallocate(_M_node._M_data, 1); }
    void clear();
protected:
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;
};

template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    ~list() { }
};

}   // namespace std

template <class T> class CArray : public std::vector<T, std::allocator<T> > { };

class CEngineComponent {
public:
    virtual ~CEngineComponent();                            // 0x00B59730
    void** GlobalPointerAddress;                            // +0x04
    bool RenderEnabled;                                     // +0x08
    unsigned char KeepAlive;                                // +0x09
};

class CBankFileAsync {
public:
    virtual ~CBankFileAsync();                              // 0x009D6490
    unsigned char _body[0x160];
};

class CStaticMapBankFile : public CBankFileAsync {          // vtable 0x012A1EE4, 0x164 bytes; implicit destructor
};

enum STATIC_MAP_OPEN_STATE { STATIC_MAP_CLOSED = 0 };
enum STATIC_MAP_BUILD_STATUS { STATIC_MAP_BUILD_NONE = 0 };

class CEngineWorldMap : public CEngineComponent {          // vtable 0x012A1F50
public:
    virtual ~CEngineWorldMap();
    void CloseStaticMapFile();                              // 0x00B40000
private:
    void DeleteMap();                                       // 0x00B42C70
public:
    short* Map;                                             // +0x0c
    long MapArrayWidth;                                     // +0x10
    long MapArrayHeight;                                    // +0x14
    long MapOffsetX;                                        // +0x18
    long MapOffsetY;                                        // +0x1c
    CArray<CEngineMap*> MapList;                            // +0x20
    std::list<CEngineMap*> VisibleMapList;                  // +0x2c
    CWideString StaticMapBankName;                          // +0x30
    CStaticMapBankFile StaticMapFile;                       // +0x34
    CIDrawWorld* GameWorld;                                 // +0x198
    CIDrawWorldMap* GameWorldMap;                           // +0x19c
    long StaticMapQuality;                                  // +0x1a0
    bool EnableMinimumStaticMapRebuild;                     // +0x1a4
    bool FirstWorldUpdate;                                  // +0x1a5
    STATIC_MAP_OPEN_STATE StaticMapOpenState;               // +0x1a8
    bool DynamicUpdateEnabled;                              // +0x1ac
    bool BuildRetailStaticMaps;                             // +0x1ad
    STATIC_MAP_BUILD_STATUS StaticMapBuildStatus;           // +0x1b0
    CArray<C2DBoxI> ChangedAreas;                           // +0x1b4
};

CEngineWorldMap::~CEngineWorldMap()
{
    CloseStaticMapFile();
    DeleteMap();
}