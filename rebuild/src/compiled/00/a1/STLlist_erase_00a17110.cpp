// _STL::list::erase(iterator position) -- STLport list<CNavQuadTreeMap>: unlink one node, destroy the map entry (section name, counted quad tree), free the node, return the next position
// Leaf: ?erase@?$list@VCNavQuadTreeMap@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A17110, 93 bytes, __fastcall `ret 8` (hidden iterator result, position); `this` unused.
// Identity: STLport 4.6 list::erase(iterator) out of line; the node payload at +8 is Ego_r.pdb CNavQuadTreeMap (32 bytes):
//   PQuadTree +0 (CCountedPointer<CNavQuadTree>: Data, Info), Index +8, MapArea +0xC (C2DBoxI), SectionName +0x1C
//   (CCharString) -- node +8 / +0xC / +0x24 as retail touches them.
// Callees: CCharString::~CCharString @ 0x0099EAE0, the counted pointer's CCPPointerInfo::DeleteFunc (__fastcall, slot +4),
//   operator delete @ 0x00BFE9BC (the info block), free @ 0x00BFEA14 (the node).
// Shape: CNavQuadTreeMap declares no destructor; the inline CCountedPointer destructor re-reads Info at every step
//   (`if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } Data = 0;
//   Info = 0;`), its two clears scheduled after the `push node`. The iterator (user ctors) comes back through the hidden pointer.
extern "C" void __cdecl free(void* p);                  // 0x00BFEA14
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PData;
    ~CCharString();                                     // 0x0099EAE0
};

struct CCPPointerInfo {
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void*         Data;                                 // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

class CNavQuadTree;

struct C2DBoxI {
    long MinX, MinY, MaxX, MaxY;
};

class CNavQuadTreeMap {                                 // 32 bytes (Ego_r.pdb)
public:
    CCountedPointer<CNavQuadTree> PQuadTree;            // +0x00
    long        Index;                                  // +0x08
    C2DBoxI     MapArea;                                // +0x0C
    CCharString SectionName;                            // +0x1C
};

namespace _STL {

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) {}
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Traits> _Self;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) {}
    _List_iterator(const _Self& __x) : _List_iterator_base(__x._M_node) {}
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, _Node, allocator<_Node> > _M_node;   // +0x00: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

    void _M_deallocate(_Node* __p) { if (__p != 0) free(__p); }

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        _M_deallocate(__n);
        return iterator(__next_node);
    }
};

template list<CNavQuadTreeMap>::iterator list<CNavQuadTreeMap>::erase(iterator);

} // namespace _STL