// std::list::remove_if(CMiniMapDisplay::CIsMarkerBoundToThing pred) -- erase every manually placed mini-map marker bound to the predicate's thing (each erased marker's thing pointer destroyed, then its node freed)
// Instantiation: std::list<CMiniMapDisplay::CMarkerInfo>::remove_if
// Leaf: ??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@?$list@VCMarkerInfo@CMiniMapDisplay@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0082C770, 78 bytes, thiscall, `ret 4` (the one-word predicate).
// Identity: FableWin.pdb `??$remove_if@VCIsMarkerBoundToThing@CMiniMapDisplay@@@?$list@VCMarkerInfo@CMiniMapDisplay@@...@Z`;
//   reached through the jmp thunks 0x0082A383 and 0x0082C890.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CBaseIntelligentPointer::~CBaseIntelligentPointer @ 0x00A01C10,
//   free @ 0x00BFEA14 (the node).
// Layout: as 0x0082C6E0; Ego_r.pdb CIsMarkerBoundToThing { CThing* PThing +0 }.
// Shape: STLport 4.6 `remove_if` + 4.6 `erase`; the predicate is `marker.PThing.Get() == PThing && !marker.AutoMarker`
//   (the marker address formed once in edi for the GetPItem call, the AutoMarker byte and the destructor).
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CThing : public CBaseObject { };
class CBaseObjectPointer;

class CBaseIntelligentPointer {                             // Ego_r.pdb: vptr +0, PData +4 (vtable 0x0129C95C)
public:
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10 (slot 0: the scalar deleting destructor)
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    CBaseObjectPointer* PData;                              // +0x4
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // no declared destructor in retail
public:
    T* Get() const { return (T*)GetPItem(); }
};

template <class T>
bool __fastcall operator==(const CIntelligentPointer<T>& a, const CIntelligentPointer<T>& b)
{
    return a.GetPItem() == b.GetPItem();
}

class CMiniMapDisplay {
public:
    class CMarkerInfo {                                     // Ego_r.pdb, 0x10 bytes
    public:
        CIntelligentPointer<const CThing> PThing;           // +0x0
        long                              GraphicIndex;     // +0x8
        bool                              AutoMarker;       // +0xc
    };
    class CIsMarkerAutoMarker {                             // Ego_r.pdb: empty
    public:
        CIsMarkerAutoMarker() { }
        bool operator()(const CMarkerInfo& marker) const { return marker.AutoMarker; }
    };
    class CIsMarkerBoundToThing {                           // Ego_r.pdb: PThing +0
    public:
        const CThing* PThing;
        CIsMarkerBoundToThing(const CThing& thing) : PThing(&thing) { }
        bool operator()(const CMarkerInfo& marker) const { return marker.PThing.Get() == PThing && !marker.AutoMarker; }
    };
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

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp> struct _List_iterator : public _List_iterator_base {
    typedef _List_node<_Tp> _Node;
    _List_iterator(_List_node_base* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_Node*)_M_node)->_M_data; }
    _Tp* operator->() const { return &((_Node*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator(__next_node);
    }
    void remove(const _Tp& __val)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__val == *__first)
                erase(__first);
            __first = __next;
        }
    }
    template <class _Predicate> void remove_if(_Predicate __pred)
    {
        iterator __first = begin();
        iterator __last = end();
        while (__first != __last) {
            iterator __next = __first;
            ++__next;
            if (__pred(*__first))
                erase(__first);
            __first = __next;
        }
    }
};
}

template void std::list<CMiniMapDisplay::CMarkerInfo>::remove_if(CMiniMapDisplay::CIsMarkerBoundToThing);