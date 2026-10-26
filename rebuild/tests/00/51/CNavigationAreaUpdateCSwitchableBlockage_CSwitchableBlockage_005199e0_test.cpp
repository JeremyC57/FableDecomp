// Behaviour test for 0x005199E0 CNavigationAreaUpdate::CSwitchableBlockage copy constructor: the 64-bit UniqueID is copied and
// the line list is deep-copied in order (all four floats) into fresh nodes behind a fresh sentinel; empty copies empty.
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc;
#ifdef _MSC_VER
#define K64(x) x##ui64
#else
#define __int64 long long
#define K64(x) x##ULL
#endif
class C2DVector {
public:
    float X;                                   // +0x0
    float Y;                                   // +0x4
};
class C2DLineF {
public:
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

struct __false_type { __false_type() {} };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
};
struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) {}
};
struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_const_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) {}
};
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) {}
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
    _List_base(const allocator<_List_node<T> >& a) : _M_node(a, (_List_node<T>*)0)
    {
        _List_node<T>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class T> class list : public _List_base<T> {
public:
    typedef _List_node<T> _Node;
    allocator<_Node> get_allocator() const { return allocator<_Node>(); }
    _List_iterator begin() { return _List_iterator(this->_M_node._M_data->_M_next); }
    _List_const_iterator begin() const { return _List_const_iterator(this->_M_node._M_data->_M_next); }
    _List_const_iterator end() const { return _List_const_iterator(this->_M_node._M_data); }
    void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
                                                 const __false_type&)   // 0x005152C0 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _Node* n = this->_M_node.allocate(1);
            new (&n->_M_data) T(((_Node*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(_List_iterator pos, _List_const_iterator first, _List_const_iterator last)
    {
        _M_insert_dispatch(pos, first, last, __false_type());
    }
    list(const list<T>& x) : _List_base<T>(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

struct CSwitchableBlockage {                    // CNavigationAreaUpdate::CSwitchableBlockage
    unsigned __int64 UniqueID;
    list<C2DLineF> LineList;
    CSwitchableBlockage(const CSwitchableBlockage& o) : UniqueID(o.UniqueID), LineList(o.LineList) {}
};


typedef CSwitchableBlockage PairT;
typedef list<C2DLineF>::_Node N;
static int same(const C2DLineF& a, const C2DLineF& b)
{ return a.Point1.X == b.Point1.X && a.Point1.Y == b.Point1.Y && a.Point2.X == b.Point2.X && a.Point2.Y == b.Point2.Y; }
static int check(const list<C2DLineF>& l, const C2DLineF* v, int n, const list<C2DLineF>& src)
{
    _List_node_base* h = l._M_node._M_data;
    if (h == 0 || h == src._M_node._M_data) return 0;
    _List_node_base* p = h->_M_next;
    for (int i = 0; i < n; ++i) {
        if (p == h || !same(((N*)p)->_M_data, v[i]) || p->_M_next->_M_prev != p) return 0;
        for (_List_node_base* q = src._M_node._M_data->_M_next; q != src._M_node._M_data; q = q->_M_next) if (q == p) return 0;
        p = p->_M_next;
    }
    return p == h && h->_M_prev->_M_next == h;
}
int main()
{
    C2DLineF v[2];
    v[0].Point1.X = 1.5f; v[0].Point1.Y = -2.0f; v[0].Point2.X = 3.25f; v[0].Point2.Y = 4.0f;
    v[1].Point1.X = -7.0f; v[1].Point1.Y = 8.5f; v[1].Point2.X = 0.0f; v[1].Point2.Y = 10.0f;
    N* h = (N*)malloc(sizeof(N)); h->_M_next = h; h->_M_prev = h;
    for (int i = 0; i < 2; ++i) {
        N* n = (N*)malloc(sizeof(N)); n->_M_data = v[i];
        n->_M_next = h; n->_M_prev = h->_M_prev; h->_M_prev->_M_next = n; h->_M_prev = n;
    }
    char raw[sizeof(PairT)];
    PairT* src = (PairT*)raw;
    src->UniqueID = K64(0xFFFFFE0000001234);
    src->LineList._M_node._M_data = h;
    PairT copy(*src);
    int ok = copy.UniqueID == K64(0xFFFFFE0000001234) && check(copy.LineList, v, 2, src->LineList);
    N* e = (N*)malloc(sizeof(N)); e->_M_next = e; e->_M_prev = e;
    src->LineList._M_node._M_data = e; src->UniqueID = 0;
    PairT copy2(*src);
    ok &= copy2.UniqueID == 0 && check(copy2.LineList, v, 0, src->LineList);
    if (!ok) { std::printf("005199e0_TEST FAIL\n"); return 1; }
    std::printf("005199e0_TEST PASS\n");
    return 0;
}