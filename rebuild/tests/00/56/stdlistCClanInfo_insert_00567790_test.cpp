// Behaviour test for 0x00567790 std::list<CClanInfo>::insert(pos, x).
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_copies = 0;
class CWideString { public:
    int m_value;
    explicit CWideString(int v) : m_value(v) { }
    CWideString(const CWideString& other) : m_value(other.m_value) { ++g_copies; }
};
class CCharString { public:
    int m_value;
    explicit CCharString(int v) : m_value(v) { }
    CCharString(const CCharString& other) : m_value(other.m_value) { ++g_copies; }
};
struct CClanInfo {
    CWideString Name;
    long        ClanID;
    CWideString Description;
    CCharString HomePageURL;
    int         NumMembers;
    CClanInfo(int n, long id, int d, int u, int m) : Name(n), ClanID(id), Description(d), HomePageURL(u), NumMembers(m) { }
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
};
}

typedef stl::list<CClanInfo> List;

int main()
{
    stl::_List_node_base header;
    header._M_next = header._M_prev = &header;
    List l;
    l._M_node = &header;
    List::iterator a = l.insert(List::iterator(&header), CClanInfo(1, 10, 2, 3, 4));       // push_back
    List::iterator b = l.insert(List::iterator(&header), CClanInfo(5, 20, 6, 7, 8));       // push_back
    List::iterator c = l.insert(a, CClanInfo(9, 30, 10, 11, 12));                          // before a
    int ok = 1;
    if (header._M_next != c._M_node || c._M_node->_M_next != a._M_node || a._M_node->_M_next != b._M_node
        || b._M_node->_M_next != &header) ok = 0;
    if (header._M_prev != b._M_node || b._M_node->_M_prev != a._M_node || a._M_node->_M_prev != c._M_node
        || c._M_node->_M_prev != &header) ok = 0;
    const CClanInfo& r = ((List::_Node*)a._M_node)->_M_data;
    if (r.Name.m_value != 1 || r.ClanID != 10 || r.Description.m_value != 2 || r.HomePageURL.m_value != 3
        || r.NumMembers != 4) ok = 0;
    if (((List::_Node*)c._M_node)->_M_data.ClanID != 30) ok = 0;
    if (g_copies != 9) ok = 0;                            // three string members copied per insert
    std::free(a._M_node); std::free(b._M_node); std::free(c._M_node);
    if (!ok) { std::printf("00567790_TEST FAIL\n"); return 1; }
    std::printf("00567790_TEST PASS\n");
    return 0;
}