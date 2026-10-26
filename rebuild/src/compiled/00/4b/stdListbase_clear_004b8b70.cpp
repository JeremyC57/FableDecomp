// std::_List_base::clear() -- (list<<pair<CCharString, unsigned long> >>) STLport 4.6 list clear: walk the nodes from the sentinel's next, destroy each element and free its node, then relink the sentinel to itself
// Leaf: ?clear@?$_List_base@U?$pair@VCCharString@@K@std@@V?$allocator@U?$pair@VCCharString@@K@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B8B70, 56 bytes (+ int3 padding to 64), thiscall, plain `ret`.
// Identity (proven): list<<pair<CCharString, unsigned long> >>; evidence: its callers clear the member at +0x3C: the landed ~CQuestManager 0x004B4770 and CQuestManager::LoadGameState 0x004B5500 -- the landed CQuestManager models put PendingQuestsToDeactivate, Ego_r.pdb list<pair<CCharString, unsigned long> >, at +0x3C (AddPendingQuestToDeactivate 0x004B0A30 links {name, reason} pairs there); only the CCharString is destroyed
// Label: the manifest's "FrameUpdate" is propagated (family H066: six clear() bodies that differ only in the element
//   destructor; retail has no identical-code folding, so each address is one instantiation); this is STLport 4.6
//   `_List_base::clear` (`_list.c`), the out-of-line half of ~list and list::clear.
// Callees: CCharString::~CCharString 0x0099EAE0 on pair.first (node +8), free 0x00BFEA14 (allocator<_Node>::deallocate; no null test: the node was dereferenced).
extern "C" void __cdecl free(void* block);             // 0x00BFEA14
class CCharString {                                  // 4 bytes: shared string block
public:
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}

namespace std {

template <class _Tp> class allocator {
public:
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Tp _M_data;
};

struct _List_node_base {
    _List_node_base* _M_next;                           // +0x0
    _List_node_base* _M_prev;                           // +0x4
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                        // +0x8
};

template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    _STLP_alloc_proxy<_List_node_base*, allocator<_Node> > _M_node;   // +0x0: the sentinel node
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
}   // namespace std

template class std::_List_base<std::pair<CCharString, unsigned long>, std::allocator<std::pair<CCharString, unsigned long> > >;