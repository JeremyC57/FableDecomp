// Behaviour test for 0x00567FC0 std::list<CClanMember>::push_back(x).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
class CWideString { public:
    int m_value;
    explicit CWideString(int v) : m_value(v) { }
    CWideString(const CWideString& other) : m_value(other.m_value) { ++g_copies; }
};
struct CClanMember {
    CWideString Name;
    long        MemberID;
    CWideString Description;
    CClanMember(int n, long id, int d) : Name(n), MemberID(id), Description(d) { }
};

namespace stl {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

typedef stl::list<CClanMember> List;

int main()
{
    stl::_List_node_base header;
    header._M_next = header._M_prev = &header;
    List l;
    l._M_node = &header;
    l.push_back(CClanMember(1, 100, 2));
    l.push_back(CClanMember(3, 200, 4));
    l.push_back(CClanMember(5, 300, 6));
    int ok = 1;
    long expect = 100;
    int count = 0;
    for (stl::_List_node_base* p = header._M_next; p != &header; p = p->_M_next, expect += 100, ++count) {
        const CClanMember& m = ((List::_Node*)p)->_M_data;
        if (m.MemberID != expect || m.Name.m_value != (int)(expect / 50 - 1) || m.Description.m_value != (int)(expect / 50))
            ok = 0;
        if (p->_M_next->_M_prev != p) ok = 0;
    }
    if (count != 3 || header._M_prev->_M_next != &header || g_copies != 6) ok = 0;
    stl::_List_node_base* p = header._M_next;
    while (p != &header) { stl::_List_node_base* n = p->_M_next; std::free(p); p = n; }
    if (!ok) { std::printf("00567fc0_TEST FAIL\n"); return 1; }
    std::printf("00567fc0_TEST PASS\n");
    return 0;
}