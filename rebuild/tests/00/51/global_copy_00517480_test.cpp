// Behaviour test for 0x00517480 __copy(C2DLineF*, C2DLineF*, back_insert_iterator<list<C2DLineF> >, ...): every line of
// the range is appended, in order and with all four coordinates, to the end of the list behind its existing elements;
// the returned inserter still names the same list; an empty range appends nothing.
#include <cstdio>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif
class C2DVector {
public:
    C2DVector() {}
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z
};
class C2DLineF {
public:
    C2DLineF() {}
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

struct random_access_iterator_tag {};
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
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> class allocator {
public:
    allocator() {}
    T* allocate(unsigned int n) { return n != 0 ? (T*)std::malloc(n * sizeof(T)) : 0; }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<T> > _M_node;
};
template <class T> class list : public _List_base<T> {
public:
    typedef T value_type;
    typedef _List_node<T> _Node;
    _List_iterator end() { return _List_iterator(this->_M_node._M_data); }
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    _List_iterator insert(_List_iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_back(const T& x) { insert(end(), x); }
};
template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& x) : container(&x) {}
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& val)
    {
        container->push_back(val);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}


typedef list<C2DLineF> L;
typedef _List_node<C2DLineF> N;
class Peek : public back_insert_iterator<L> { public: Peek(const back_insert_iterator<L>& b) : back_insert_iterator<L>(b) {} L* c() const { return container; } };
int main()
{
    C2DLineF v[3];
    for (int i = 0; i < 3; ++i) { v[i].Point1.X = (float)i; v[i].Point1.Y = i + 0.5f; v[i].Point2.X = -(float)i; v[i].Point2.Y = 10.0f * i; }
    _List_node_base h; h._M_next = &h; h._M_prev = &h;
    N* pre = (N*)std::malloc(sizeof(N)); pre->_M_data.Point1.X = 99.0f;
    pre->_M_next = &h; pre->_M_prev = &h; h._M_next = pre; h._M_prev = pre;
    L l; l._M_node._M_data = (N*)&h;
    random_access_iterator_tag tag;
    back_insert_iterator<L> r = __copy(v, v, back_insert_iterator<L>(l), tag, (int*)0);
    int ok = Peek(r).c() == &l && h._M_next == pre && h._M_prev == pre;
    r = __copy(v, v + 3, back_insert_iterator<L>(l), tag, (int*)0);
    ok &= Peek(r).c() == &l && h._M_next == pre;
    _List_node_base* p = pre->_M_next;
    for (int i = 0; i < 3; ++i) {
        N* n = (N*)p;
        ok &= p != &h && n->_M_data.Point1.X == v[i].Point1.X && n->_M_data.Point1.Y == v[i].Point1.Y
              && n->_M_data.Point2.X == v[i].Point2.X && n->_M_data.Point2.Y == v[i].Point2.Y && p->_M_next->_M_prev == p;
        p = p->_M_next;
    }
    ok &= p == &h && h._M_prev != pre;
    if (!ok) { std::printf("00517480_TEST FAIL\n"); return 1; }
    std::printf("00517480_TEST PASS\n");
    return 0;
}