// Behaviour test for 00a830a0 (_STL::__uninitialized_fill_n over pair<unsigned __int64, list<C2DLineF> >): n copies of x are copy-constructed into raw storage,
// the returned cursor is first + n, n == 0 constructs nothing, and every copy is balanced by its destructor.
#include <cstdio>
#include <new>
#include <cstdlib>
static int g_live = 0;
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#else
typedef unsigned long long u64;
#endif
struct C2DVector { float X; float Y; };
struct C2DLineF { C2DVector Point1; C2DVector Point2; };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct list {              // STLport list: a malloc'd self-linked header, insert(begin(), x.begin(), x.end())
    _List_node<_Tp>* _M_node;
    list() { _M_node = (_List_node<_Tp>*)std::malloc(sizeof(_List_node<_Tp>)); _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node; ++g_live; }
    list(const list& x)
    {
        _M_node = (_List_node<_Tp>*)std::malloc(sizeof(_List_node<_Tp>));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
        _M_insert_dispatch(_M_node->_M_next, x._M_node->_M_next, x._M_node);   // stands in for 0x005152C0
        ++g_live;
    }
    void _M_insert_dispatch(_List_node_base* pos, _List_node_base* first, _List_node_base* last)
    {
        for (; first != last; first = first->_M_next) {
            _List_node<_Tp>* n = (_List_node<_Tp>*)std::malloc(sizeof(_List_node<_Tp>));
            n->_M_data = ((_List_node<_Tp>*)first)->_M_data;
            n->_M_next = pos;
            n->_M_prev = pos->_M_prev;
            pos->_M_prev->_M_next = n;
            pos->_M_prev = n;
        }
    }
    void push_back(const _Tp& v) { _List_node<_Tp>* n = (_List_node<_Tp>*)std::malloc(sizeof(_List_node<_Tp>)); n->_M_data = v; n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev; _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n; }
    ~list()
    {
        _List_node_base* c = _M_node->_M_next;
        while (c != _M_node) { _List_node_base* nx = c->_M_next; std::free(c); c = nx; }
        std::free(_M_node);
        --g_live;
    }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a) : first(a), second() { }
    pair(const pair& o) : first(o.first), second(o.second) { }
};
static bool SameLines(const list<C2DLineF>& a, const list<C2DLineF>& b)
{
    if (a._M_node == b._M_node) return false;   // a deep copy owns its own header
    _List_node_base* p = a._M_node->_M_next;
    _List_node_base* q = b._M_node->_M_next;
    for (; p != a._M_node && q != b._M_node; p = p->_M_next, q = q->_M_next) {
        const C2DLineF& l = ((_List_node<C2DLineF>*)p)->_M_data;
        const C2DLineF& m = ((_List_node<C2DLineF>*)q)->_M_data;
        if (l.Point1.X != m.Point1.X || l.Point1.Y != m.Point1.Y || l.Point2.X != m.Point2.X || l.Point2.Y != m.Point2.Y) return false;
    }
    return p == a._M_node && q == b._M_node;
}
typedef pair<u64, list<C2DLineF> > T;
static bool Same(const T& a, const T& b)
{
    return a.first == b.first && SameLines(a.second, b.second);
}
struct __false_type { __false_type() { } };
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        new (&*__cur) _Tp(__x);
    return __cur;
}
union UStorage { double d; void* p; char bytes[sizeof(T) * 6]; };
int main()
{
    T x(((u64)0x12345678 << 4) | 9);
    for (int k = 0; k < 3; ++k) {
        C2DLineF l;
        l.Point1.X = (float)k; l.Point1.Y = 1.0f; l.Point2.X = 2.0f; l.Point2.Y = (float)(k * 3);
        x.second.push_back(l);
    }
    const int base = g_live;
    UStorage storage;
    T* items = reinterpret_cast<T*>(storage.bytes);
    T* r = __uninitialized_fill_n(items, 0u, x, __false_type());
    if (r != items || g_live != base) { std::printf("00a830a0_TEST FAIL\n"); return 1; }
    r = __uninitialized_fill_n(items, 5u, x, __false_type());
    if (r != items + 5) { std::printf("00a830a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (!Same(items[i], x)) { std::printf("00a830a0_TEST FAIL\n"); return 1; }
    if (g_live != base + 5 * 1) { std::printf("00a830a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        items[i].~T();
    if (g_live != base) { std::printf("00a830a0_TEST FAIL\n"); return 1; }
    std::printf("00a830a0_TEST PASS\n");
    return 0;
}