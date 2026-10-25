// std::_List_base::clear() -- free every quad-tree map entry of the navigator manager's list (section name destroyed, counted quad tree released) and relink the sentinel to itself
// Instantiation: std::_List_base<CNavQuadTreeMap,std::allocator<CNavQuadTreeMap> >::clear
// Leaf: ?clear@?$_List_base@VCNavQuadTreeMap@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A17390, 97 bytes (+ int3 padding), thiscall, plain `ret`.
// Identity: called by the two STLport `~list` bodies 0x00A17510 / 0x00A17570 (landed as std::list_T_00a17510 / _00a17570);
//   0x00A17570 is CNavigatorManager's QuadTrees list (its deleting destructor 0x00A175A0, vtable 0x0129CA84, destroys
//   +0x2c then this list at +0x24). Element = Ego_r.pdb CNavQuadTreeMap (PQuadTree +0, Index +8, MapArea +0xc,
//   SectionName +0x1c). The catalog label `remove_if<NEntityEvents::CConvFilterToCP<..> >` is propagated.
// Callees: CCharString::~CCharString @ 0x0099EAE0, the count block's DeleteFunc, operator delete @ 0x00BFE9BC (the count
//   block), free @ 0x00BFEA14 (the node).
// Shape: STLport 4.x `_List_base::clear()` (as the landed 0x00ADC250): the cursor advances before the node is destroyed,
//   the implicit ~CNavQuadTreeMap runs SectionName first and then the counted pointer (released, then both words zeroed
//   from the shared zero register), the sentinel is re-read every trip and relinked to itself at the end.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

struct CCPPointerInfo {                                     // Ego_r.pdb
    unsigned long RefCount;                                 // +0x0
    void (__fastcall* DeleteFunc)(void* data);              // +0x4
    void* Data;                                             // +0x8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                                   // +0x0
    CCPPointerInfo* Info;                                   // +0x4
    T* operator->() const { return Data; }
    bool operator==(const CCountedPointer& other) const { return Data == other.Data; }
    void ReleaseInfo()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    ~CCountedPointer() { ReleaseInfo(); Data = 0; Info = 0; }
};

class CCharString {
public:
    void* PStringData;                                      // +0x0
    ~CCharString();                                         // 0x0099EAE0
};

class CNavQuadTree;
class C2DBoxI {                                             // 16 bytes
public:
    long TopLeftX, TopLeftY, BottomRightX, BottomRightY;
};
class CNavQuadTreeMap {                                     // Ego_r.pdb, 0x20 bytes
public:
    CCountedPointer<CNavQuadTree> PQuadTree;                // +0x00
    long                          Index;                    // +0x08
    C2DBoxI                       MapArea;                  // +0x0c
    CCharString                   SectionName;              // +0x1c
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

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x0
    _List_node_base* _M_prev;                               // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x8
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // the sentinel
    void clear();
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
}

template class std::_List_base<CNavQuadTreeMap, std::allocator<CNavQuadTreeMap> >;