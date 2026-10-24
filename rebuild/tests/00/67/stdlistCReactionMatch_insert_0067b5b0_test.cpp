// Behaviour test for 0x0067B5B0 std::list<CReactionMatch*>::insert: new nodes carry the pointer and are
// linked in front of the given position (the sentinel for push_back), keeping the ring consistent both ways.
#include <cstdio>
#include <cstdlib>
#include <new>

class CReactionMatch { public: int Id; };

namespace nav {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : _List_node_base { _Tp _M_data; };
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
    iterator end() { return _M_node; }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return __tmp;
    }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

typedef nav::list<CReactionMatch*> MatchList;

static CReactionMatch* At(MatchList& l, int i)
{
    nav::_List_node_base* n = l._M_node->_M_next;
    while (i--) n = n->_M_next;
    return ((MatchList::_Node*)n)->_M_data;
}

int main()
{
    nav::_List_node_base sentinel;
    sentinel._M_next = sentinel._M_prev = &sentinel;
    MatchList l;
    l._M_node = &sentinel;
    CReactionMatch a, b, c;
    a.Id = 1; b.Id = 2; c.Id = 3;
    CReactionMatch* pa = &a; CReactionMatch* pb = &b; CReactionMatch* pc = &c;
    MatchList::iterator ia = l.insert(l.end(), pa);           // [a]
    MatchList::iterator ic = l.insert(l.end(), pc);           // [a c]
    MatchList::iterator ib = l.insert(ic, pb);                // [a b c]
    if (ib._M_node == ia._M_node || ((MatchList::_Node*)ib._M_node)->_M_data != pb) { std::printf("0067b5b0_TEST FAIL\n"); return 1; }
    if (At(l, 0) != pa || At(l, 1) != pb || At(l, 2) != pc || sentinel._M_prev->_M_prev->_M_prev->_M_prev != &sentinel ||
        sentinel._M_next->_M_next->_M_next->_M_next != &sentinel) {
        std::printf("0067b5b0_TEST FAIL\n"); return 1;
    }
    nav::_List_node_base* n = sentinel._M_next;
    while (n != &sentinel) { nav::_List_node_base* next = n->_M_next; std::free(n); n = next; }
    std::printf("0067b5b0_TEST PASS\n");
    return 0;
}