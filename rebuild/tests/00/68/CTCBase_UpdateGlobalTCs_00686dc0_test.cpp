// Behaviour test for 0x00686DC0 CTCBase::UpdateGlobalTCs: every listed TC not marked ToBeRemoved gets GlobalUpdate
// with the flag, in list order; a TC may unlink itself during its update because the iterator has already moved on.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace nav {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& __x) const { return _M_node != __x._M_node; }
};
template <class _Tp> class list {
public:
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator begin() { return _M_node->_M_next; }
    iterator end() { return _M_node; }
};
}

static int g_order[8];
static int g_calls = 0;
static bool g_flag = false;

class CTCBase {
public:
    virtual ~CTCBase() {}
    virtual void GlobalUpdate(bool paused) { g_order[g_calls++] = Id; g_flag = paused; }
    int  Id;
    bool ToBeRemoved;
    nav::_List_node<CTCBase*>* Node;
    static nav::list<CTCBase*> GlobalUpdateTCs;
    static void __fastcall UpdateGlobalTCs(bool paused);
};
nav::list<CTCBase*> CTCBase::GlobalUpdateTCs;

class CSelfRemovingTC : public CTCBase {
public:
    virtual void GlobalUpdate(bool paused)
    {
        CTCBase::GlobalUpdate(paused);
        Node->_M_prev->_M_next = Node->_M_next;               // unlink own node mid-walk
        Node->_M_next->_M_prev = Node->_M_prev;
    }
};

void __fastcall CTCBase::UpdateGlobalTCs(bool paused)
{
    for (nav::list<CTCBase*>::iterator it = GlobalUpdateTCs.begin(); it != GlobalUpdateTCs.end(); ) {
        CTCBase* tc = *it;
        ++it;
        if (!tc->ToBeRemoved)
            tc->GlobalUpdate(paused);
    }
}

int main()
{
    nav::_List_node_base sentinel;
    nav::_List_node<CTCBase*> nodes[4];
    CTCBase a, c, d; CSelfRemovingTC b;
    CTCBase* tcs[4] = { &a, &b, &c, &d };
    sentinel._M_next = sentinel._M_prev = &sentinel;
    for (int i = 0; i < 4; ++i) {
        tcs[i]->Id = i + 1; tcs[i]->ToBeRemoved = (i == 2); tcs[i]->Node = &nodes[i];
        nodes[i]._M_data = tcs[i];
        nodes[i]._M_prev = sentinel._M_prev; nodes[i]._M_next = &sentinel;
        sentinel._M_prev->_M_next = &nodes[i]; sentinel._M_prev = &nodes[i];
    }
    CTCBase::GlobalUpdateTCs._M_node = &sentinel;
    CTCBase::UpdateGlobalTCs(true);
    if (g_calls != 3 || g_order[0] != 1 || g_order[1] != 2 || g_order[2] != 4 || !g_flag || sentinel._M_next->_M_next != &nodes[2]) {
        std::printf("00686dc0_TEST FAIL\n"); return 1;
    }
    g_calls = 0;
    CTCBase::UpdateGlobalTCs(false);
    if (g_calls != 2 || g_order[0] != 1 || g_order[1] != 4 || g_flag) { std::printf("00686dc0_TEST FAIL\n"); return 1; }
    std::printf("00686dc0_TEST PASS\n");
    return 0;
}