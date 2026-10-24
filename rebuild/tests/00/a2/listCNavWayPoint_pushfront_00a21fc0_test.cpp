// Behaviour test for list<CNavWayPoint>::push_front (0x00A21FC0) and the shared insert: copies are linked in front
// of the route in LIFO order, every field is copied, and the ring stays consistent both ways.
#include <cstdio>
#include <cstdlib>
#include <new>

class C2DVector { public: float X, Y; C2DVector() {} C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {} };

class CNavWayPoint {
public:
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
};

namespace _STL {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator begin() { return iterator(_M_node->_M_next); }
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
    void push_front(const _Tp& __x) { insert(begin(), __x); }
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int ok = 1;
    _STL::_List_node_base sentinel; sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel;
    _STL::list<CNavWayPoint> route; route._M_node = &sentinel;
    CNavWayPoint wp;
    for (int i = 0; i < 3; ++i) {
        wp.Position.X = 1.5f * i; wp.Position.Y = -2.0f * i; wp.NavLayer = (unsigned char)(10 + i); wp.Removable = (i == 1);
        route.push_front(wp);
    }
    // expected order: 2, 1, 0 (LIFO)
    int expect[3] = { 2, 1, 0 };
    _STL::_List_node_base* n = sentinel._M_next;
    for (int i = 0; i < 3; ++i) {
        if (n == &sentinel) { ok = 0; break; }
        _STL::_List_node<CNavWayPoint>* node = static_cast<_STL::_List_node<CNavWayPoint>*>(n);
        int k = expect[i];
        if (!nearf(node->_M_data.Position.X, 1.5f * k) || !nearf(node->_M_data.Position.Y, -2.0f * k)) ok = 0;
        if (node->_M_data.NavLayer != 10 + k || node->_M_data.Removable != (k == 1)) ok = 0;
        if (n->_M_next->_M_prev != n) ok = 0;
        n = n->_M_next;
    }
    if (n != &sentinel || sentinel._M_prev->_M_next != &sentinel) ok = 0;
    while (sentinel._M_next != &sentinel) { _STL::_List_node_base* d = sentinel._M_next; sentinel._M_next = d->_M_next; std::free(d); }

    if (!ok) { std::printf("00a21fc0_TEST FAIL\n"); return 1; }
    std::printf("00a21fc0_TEST PASS\n");
    return 0;
}