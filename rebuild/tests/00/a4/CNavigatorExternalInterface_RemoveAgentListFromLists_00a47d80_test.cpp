// Behaviour test for 0x00A47D80: CNavigatorExternalInterface::RemoveAgentListFromLists (find the list node and unlink and free it)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
#endif

static int g_fail = 0;
static void Check(bool c) { if (!c) g_fail = 1; }
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

class CSteeringAgent;
namespace _STL {
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
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp>
struct _List_node : public _List_node_base {
    _Tp _M_data;
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
    // test helpers
    void Init()
    {
        this->_M_node._M_data = (_Node*)malloc(sizeof(_Node));
        this->_M_node._M_data->_M_next = this->_M_node._M_data;
        this->_M_node._M_data->_M_prev = this->_M_node._M_data;
    }
    void PushBack(const _Tp& x)
    {
        _Node* n = (_Node*)malloc(sizeof(_Node));
        n->_M_data = x;
        _List_node_base* h = this->_M_node._M_data;
        n->_M_next = h;
        n->_M_prev = h->_M_prev;
        h->_M_prev->_M_next = n;
        h->_M_prev = n;
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

typedef _STL::vector<const CSteeringAgent*> TAgents;
class CNavigatorExternalInterface {
public:
    virtual ~CNavigatorExternalInterface() { }
    static _STL::list<TAgents*> AgentLists;
    static void __fastcall RemoveAgentListFromLists(TAgents* agentList);
};
_STL::list<TAgents*> CNavigatorExternalInterface::AgentLists;

void __fastcall CNavigatorExternalInterface::RemoveAgentListFromLists(TAgents* agentList)
{
    AgentLists.erase(_STL::find(AgentLists.begin(), AgentLists.end(), agentList));
    _STL::find(AgentLists.begin(), AgentLists.end(), agentList);
}

static int Count()
{
    int n = 0;
    for (_STL::list<TAgents*>::iterator it = CNavigatorExternalInterface::AgentLists.begin();
         it != CNavigatorExternalInterface::AgentLists.end(); ++it)
        ++n;
    return n;
}

int main()
{
    TAgents a, b, c;
    _STL::list<TAgents*>& lists = CNavigatorExternalInterface::AgentLists;
    lists.Init();
    lists.PushBack(&a);
    lists.PushBack(&b);
    lists.PushBack(&c);
    Check(Count() == 3);
    CNavigatorExternalInterface::RemoveAgentListFromLists(&b);
    Check(Count() == 2);
    _STL::list<TAgents*>::iterator it = lists.begin();
    Check(*it == &a);
    ++it;
    Check(*it == &c);
    ++it;
    Check(it == lists.end());
    Check(lists._M_node._M_data->_M_prev->_M_prev == lists._M_node._M_data->_M_next);
    CNavigatorExternalInterface::RemoveAgentListFromLists(&a);
    CNavigatorExternalInterface::RemoveAgentListFromLists(&c);
    Check(Count() == 0 && lists._M_node._M_data->_M_next == lists._M_node._M_data && lists._M_node._M_data->_M_prev == lists._M_node._M_data);
    free(lists._M_node._M_data);
    if (g_fail) {
        printf("00a47d80_TEST FAIL\n");
        return 1;
    }
    printf("00a47d80_TEST PASS\n");
    return 0;
}