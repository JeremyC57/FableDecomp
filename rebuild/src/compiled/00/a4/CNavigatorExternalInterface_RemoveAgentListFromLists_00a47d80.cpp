// CNavigatorExternalInterface::RemoveAgentListFromLists(vector<const CSteeringAgent*>* agentList) -- unregister an agent list: find it in the registered lists and erase its node
// Leaf: ?RemoveAgentListFromLists@CNavigatorExternalInterface@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A47D80, length 77 bytes (+3 int3 pad), static `__fastcall` (the list in ecx,
//   plain `ret`).
// Name: Ego_r.pdb `?RemoveAgentListFromLists@CNavigatorExternalInterface@@SIXPAV?$vector@PBVCSteeringAgent@@...@Z` (name set).
// Global: CNavigatorExternalInterface::AgentLists 0x013D2834 (STLport list<vector<const CSteeringAgent*>*>). Callees: free
//   0x00BFEA14 (the node deallocation).
// Shape: `AgentLists.erase(find(begin, end, agentList));` (the inlined STLport input-iterator find, then the unlink and free),
//   then a second find whose result is discarded (retail repeats the walk after the free: a release-mode check that
//   the list is gone, its comparison compiled away).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* p);                                                // 0x00BFEA14

class CSteeringAgent;

namespace std {
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

// vector (12 bytes)
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                   // +0x00
    _Tp* _M_finish;                                  // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (char*)memmove(__result, __first, ((const char*)__last - (const char*)__first)) +
        ((const char*)__last - (const char*)__first);
}

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    iterator erase(iterator __position)
    {
        if (__position + 1 != end())
            __copy_trivial(__position + 1, this->_M_finish, __position);
        --this->_M_finish;
        return __position;
    }
};

// list (4 bytes: the sentinel node pointer)
struct _List_node_base {
    _List_node_base* _M_next;                        // +0x00
    _List_node_base* _M_prev;                        // +0x04
};

template <class _Tp>
struct _List_node : public _List_node_base {
    _Tp _M_data;                                     // +0x08
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};

template <class _Tp>
struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp>*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    iterator begin() { return iterator((_Node*)(this->_M_node._M_data->_M_next)); }
    iterator end() { return iterator(this->_M_node._M_data); }
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};

template <class _InputIter, class _Tp>
inline _InputIter find(_InputIter __first, _InputIter __last, const _Tp& __val)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
}

class CNavigatorExternalInterface {
public:
    virtual ~CNavigatorExternalInterface();
    static std::list<std::vector<const CSteeringAgent*>*> AgentLists;     // 0x013D2834
    static void __fastcall RemoveAgentFromLists(const CSteeringAgent* agent);
    static void __fastcall RemoveAgentListFromLists(std::vector<const CSteeringAgent*>* agentList);
};

void __fastcall CNavigatorExternalInterface::RemoveAgentListFromLists(std::vector<const CSteeringAgent*>* agentList)
{
    AgentLists.erase(std::find(AgentLists.begin(), AgentLists.end(), agentList));
    std::find(AgentLists.begin(), AgentLists.end(), agentList);
}