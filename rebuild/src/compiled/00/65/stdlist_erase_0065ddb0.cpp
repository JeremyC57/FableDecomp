// std::list::erase(iterator position) -- (list<CIntelligentPointer<CThingBuilding> >) STLport 4.6 single-node erase: unlink the node, destroy its element through the element's virtual destructor, free the node and return the iterator to the following node
// Leaf: ?erase@?$list@V?$CIntelligentPointer@VCThingBuilding@@@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065DDB0, 43 bytes (+ int3 padding), thiscall, `ret 8` (hidden iterator result + the
//   iterator by value), returns the result pointer.
// Identity: list<CIntelligentPointer<CThingBuilding> > -- proven; evidence: its callers are CTCVillage::RemoveThingAndUpdateCameraPoints 0x0065B6DD (landed; PDB name RemoveVillageMember), which erases with it from the list at CTCVillage+0x28 after find 0x0065F532, and list::remove_if 0x0065DDDB (the next function, called by the landed CTCVillage::ClearOutNullMembers 0x0065890B); retail CTCVillage lists are 4-byte STLport head pointers in Ego_r.pdb order, so +0x28 is VillageBuildings (list<CIntelligentPointer<CThingBuilding> >); the CThing-typed VillageTorches erase is the separate 0x004B7CA0 (no ICF); the PDB name sets hold list<CIntelligentPointer<CThingBuilding> >::erase
//   Family G071 (7 byte-identical bodies, each its own COMDAT: retail has no ICF, so each address is one element type); the
//   manifest label "DoublyLinkedList_RemoveNode" is propagated/behavioural.
// Callees: the element's virtual destructor (vtable slot 0, pushed 0 = destroy only, no delete), free 0x00BFEA14 (the node
//   allocator's deallocate: no null test in retail).
// Shape: STLport 4.6 list `erase(iterator)`: next and prev read from the node, prev->_M_next = next, next->_M_prev = prev,
//   `_Destroy(&n->_M_data)` (explicit ~T() through the vtable), `_M_node.deallocate(n, 1)`, then `return iterator(next)`
//   stored through the hidden pointer; the iterator's user copy constructor makes it a by-value class argument. Size-optimised
//   module without a frame pointer (`pop ecx` cleans free's argument): optimize("s").
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CThingBuilding;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                          // 8 bytes (Ego_r.pdb): vptr, PData
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
};

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _List_node_base {
    _List_node_base* _M_next;                              // +0x0
    _List_node_base* _M_prev;                              // +0x4
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                           // +0x8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp* pointer; typedef _Tp& reference; };
template <class _Tp> struct _Const_traits { typedef _Tp value_type; typedef const _Tp* pointer; typedef const _Tp& reference; };

template <class _Tp, class _Traits>
struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _AllocProxy;
    _AllocProxy _M_node;                                   // +0x0: the sentinel node
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;

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

template list<CIntelligentPointer<CThingBuilding> >::iterator list<CIntelligentPointer<CThingBuilding> >::erase(iterator);

} // namespace std