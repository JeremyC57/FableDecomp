// std::list::erase(iterator position) -- (list<CCountedPointer<CVillageTaskTorch> >) STLport 4.6 single-node erase: unlink the node, release its counted pointer, free the node and return the iterator to the following node
// Leaf: ?erase@?$list@V?$CCountedPointer@VCVillageTaskTorch@@@@V?$allocator@V?$CCountedPointer@VCVillageTaskTorch@@@@@std@@@std@@QAE?AU?$_List_iterator@V?$CCountedPointer@VCVillageTaskTorch@@@@U?$_Nonconst_traits@V?$CCountedPointer@VCVillageTaskTorch@@@@@std@@@2@U32@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00660117, 42 bytes, thiscall, `ret 8` (hidden iterator result + the iterator by
//   value), returns the result pointer.
// Identity (proven): list<CCountedPointer<CVillageTaskTorch> >::erase(iterator); evidence: its callers are CTCVillage::RemoveThingFromTaskList<CVillageTaskTorch> 0x00660787 (called from CTCVillage::RemoveThingFromAllTasks 0x00659EA9 on this+0x4C = Ego_r.pdb CTCVillage::TorchTasks, list<CCountedPointer<CVillageTaskTorch> >, retail 4-byte lists after the nine Village* lists from +0x28) and list::remove_if 0x006603A0; FableWin.pdb names ?erase@?$list@V?$CCountedPointer@VCVillageTaskTorch@@@@...; the element destructor it calls is this list's own.
// Label: the manifest's "erase" is propagated across family H020 (seven byte-identical bodies that differ only in the
//   destructor call; retail has no identical-code folding, so each address is one instantiation).
// Callees: CCountedPointer<CVillageTaskTorch>::~CCountedPointer() 0x0065D574 (thiscall on the node's element, out of line), free 0x00BFEA14
//   (the node allocator's deallocate).
// Shape: the landed STLport erase 0x0065DEE4 with a non-virtual out-of-line element destructor: next and prev read from the
//   node, prev->_M_next = next, next->_M_prev = prev, `_Destroy(&n->_M_data)`, `_M_node.deallocate(n, 1)`, then `return
//   iterator(next)` through the hidden pointer. Size-optimised module (`pop ecx` cleans free's argument).
#pragma optimize("s", on)
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14

class CVillageTaskTorch;
template <class T> class CCountedPointer {               // 8 bytes: the object, the shared count block
public:
    ~CCountedPointer();                                  // out of line
    T* PObject;                                          // +0x0
    void* PCount;                                        // +0x4
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

template list<CCountedPointer<CVillageTaskTorch > >::iterator list<CCountedPointer<CVillageTaskTorch > >::erase(iterator);

} // namespace std