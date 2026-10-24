// _STL::list<CIntelligentPointer<CThing> >::erase(iterator position) -- STLport list erase: unlink the node, destroy its intelligent pointer through the virtual destructor, free the node, return the iterator to the next node
// Leaf: ?erase@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B7CA0, 45 bytes (48 with padding), `ret 8` (__fastcall: the hidden
//   iterator result and the position by value on the stack; `this` unused).
// Identity: called from CTCVillage::RemoveThingAndUpdateCameraPoints 0x0065B6DD (landed; "the shared
//   list<CIntelligentPointer<CThing> > copy") and 0x0065DEBA; the landed std::list<CIntelligentPointer<CThing>>::remove
//   0x0065DE8A family. The element is the 8-byte polymorphic CIntelligentPointer<CThing> (16-byte nodes).
// Callees: the element's virtual destructor (slot 0, `push 0 ; call [eax]`: an explicit `~T()` on a polymorphic class
//   goes through the scalar deleting destructor with flag 0), free 0x00BFEA14 (STLport malloc node allocator).
// Shape: STLport 4.x `erase(iterator)` verbatim (`next = pos->_M_next; prev = pos->_M_prev; prev->_M_next = next;
//   next->_M_prev = prev; _Destroy(&n->_M_data); _M_node.deallocate(n, 1); return iterator(next);`).
// Label correction: catalogued as `ListNode_RemoveWithCallback`; the real routine is
//   list<CIntelligentPointer<CThing> >::erase(iterator).
#include <stdlib.h>                                   // free @ 0x00BFEA14

class CThing;

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();               // slot 0 (scalar deleting destructor)
    CThing* m_pItem;                                  // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

struct _List_node_base {
    _List_node_base* _M_next;                         // +0x00
    _List_node_base* _M_prev;                         // +0x04
};
template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                                      // +0x08
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node<_Tp>* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> struct allocator {
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp> class list {                     // STLport list (the head node pointer: 4 bytes in retail)
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node;

    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};

template class list<CIntelligentPointer<CThing> >;