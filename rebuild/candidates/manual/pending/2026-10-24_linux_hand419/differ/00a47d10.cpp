// CNavigatorExternalInterface::RemoveAgentFromLists(const CSteeringAgent* agent) -- erase every occurrence of the steering agent from each registered agent list
// Grade: DIFFER(110v97) -- the lane strength-reduces `pos + 1` into a fifth register (ebx, the i+4 induction variable) and spills `agent` to a frame slot, splitting `--_M_finish` into load/add/store; retail keeps agent in ebp, copies pos to edx and forms pos+1 with `lea ecx,[edx+4]` only on the match arm, with a single `add [edi+4],-4`
// Leaf: ?RemoveAgentFromLists@CNavigatorExternalInterface@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A47D10, length 97 bytes (+15 int3 pad), static `__fastcall` (agent in ecx,
//   plain `ret`).
// Name: Ego_r.pdb `?RemoveAgentFromLists@CNavigatorExternalInterface@@SIXPBVCSteeringAgent@@@Z` (name set).
// Global: CNavigatorExternalInterface::AgentLists 0x013D2834 (PDB `dynamic initializer for 'CNavigatorExternalInterface::
//   AgentLists'`; an STLport list<vector<const CSteeringAgent*>*>, 4 bytes = the sentinel node pointer; the landed
//   0x006CE670 / 0x01224320 call it g_registeredObstacles / g_list_013d2834). Callees: memmove 0x00BFEAE6.
// Tried: `(*it)->erase(i); --i;`, `(*it)->erase(i--);`, `i = (*it)->erase(i); --i;`, and the layered STLport erase
//   (__copy_ptrs + __true_type + _Destroy): all four compile to the same 110 bytes.
// Next: a same-TU noinline erase stand-in is ruled out (retail inlines memmove); try a list<>::iterator copy into a local
//   vector reference per outer step, or an index loop over (*it)->size() to keep pos+1 off the induction set.
// Shape: STLport 4.6 list walk; per list, `for (i = (*it)->begin(); i != (*it)->end(); ++i) if (*i == agent) { (*it)->erase(i); --i; }` (the list node's vector pointer re-read for each end() test, as retail); vector::erase(iterator) is STLport's
//   `if (pos + 1 != end()) __copy_ptrs(pos + 1, finish, pos, __true_type()); --finish; _Destroy(finish);` (memmove of the byte span, trivial destroy).
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

struct __true_type { __true_type() { } };

template <class _InputIter, class _OutputIter>
inline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __true_type&)
{
    return (_OutputIter)__copy_trivial(__first, __last, __result);
}

template <class _Tp>
inline void _Destroy(_Tp* __pointer)
{
    __pointer;
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
            __copy_ptrs(__position + 1, this->_M_finish, __position, __true_type());
        --this->_M_finish;
        _Destroy(this->_M_finish);
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

void __fastcall CNavigatorExternalInterface::RemoveAgentFromLists(const CSteeringAgent* agent)
{
    for (std::list<std::vector<const CSteeringAgent*>*>::iterator it = AgentLists.begin(); it != AgentLists.end(); ++it) {
        for (std::vector<const CSteeringAgent*>::iterator i = (*it)->begin(); i != (*it)->end(); ++i) {
            if (*i == agent) {
                (*it)->erase(i);
                --i;
            }
        }
    }
}
