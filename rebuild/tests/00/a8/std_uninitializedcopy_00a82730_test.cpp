// Behaviour test for 0x00A82730 std::__uninitialized_copy<const list<C2DLineF>*, list<C2DLineF>*>: copy-construct STLport
// lists into raw storage (fresh self-linked header, then insert the source range before begin()).
#include <cstdio>
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class C2DVector {
public:
    float X, Y;
};

class C2DLineF {
public:
    C2DVector Point1;                           // +0x00
    C2DVector Point2;                           // +0x08
};

namespace std {

struct __false_type { __false_type() { } };     // STLport empty tag; passed by const reference (one stack slot)

template <class T1, class T2>
inline void _Construct(T1* p, const T2& value)
{
    new (p) T1(value);
}

template <class T> class allocator {
public:
    allocator() { }
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};

struct _List_node_base {
    _List_node_base* _M_next;                   // +0
    _List_node_base* _M_prev;                   // +4
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                  // +8 (0x18-byte node for C2DLineF)
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _Nonconst_traits { typedef T& reference; };
template <class T> struct _Const_traits { typedef const T& reference; };

template <class T, class Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_node<T> _Node;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }   // iterator: user copy constructor
};

template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;    // +0 (4-byte STLport list)
    _List_base(const allocator<_Node>& a) : _M_node(a, (_Node*)0)
    {
        _Node* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};

template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_iterator<T, _Const_traits<T> > const_iterator;
    allocator<_Node> get_allocator() const { return allocator<_Node>(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    template <class InputIter>
    void _M_insert_dispatch(iterator position, InputIter first, InputIter last, const __false_type&)
    {
        for (_List_node_base* s = first._M_node; s != last._M_node; s = s->_M_next) {
            _Node* n = (_Node*)malloc(sizeof(_Node));
            n->_M_data = ((_Node*)s)->_M_data;
            n->_M_next = position._M_node;
            n->_M_prev = position._M_node->_M_prev;
            position._M_node->_M_prev->_M_next = n;
            position._M_node->_M_prev = n;
        }
    }
    list() : _List_base<T, A>(allocator<_Node>()) { }
    void push_back(float v)
    {
        _List_node_base* h = this->_M_node._M_data;
        _Node* n = (_Node*)malloc(sizeof(_Node));
        n->_M_data.Point1.X = v; n->_M_data.Point1.Y = v + 1; n->_M_data.Point2.X = v + 2; n->_M_data.Point2.Y = v + 3;
        n->_M_next = h; n->_M_prev = h->_M_prev; h->_M_prev->_M_next = n; h->_M_prev = n;
    }
    template <class InputIter>
    void insert(iterator position, InputIter first, InputIter last) { _M_insert_dispatch(position, first, last, __false_type()); }
    list(const list<T, A>& x) : _List_base<T, A>(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

}
typedef std::list<C2DLineF> L;
static int Count(const L& l, float* sum)
{
    int n = 0;
    *sum = 0;
    for (std::_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next) {
        ++n;
        *sum += ((L::_Node*)p)->_M_data.Point1.X + ((L::_Node*)p)->_M_data.Point2.Y;
    }
    return n;
}
int main()
{
    L src[3];
    src[0].push_back(1);
    src[0].push_back(5);
    src[2].push_back(9);
    static L* storage[3];
    L* dst = (L*)storage;
    std::__false_type tag;
    L* r = std::__uninitialized_copy((const L*)src, (const L*)src + 3, dst, tag);
    bool ok = r == dst + 3 && sizeof(L) == 4 && sizeof(L::_Node) == 0x18;
    float s0 = 0, s1 = 0, s2 = 0;
    if (Count(dst[0], &s0) != 2 || s0 != (1 + 4) + (5 + 8)) ok = false;
    if (Count(dst[1], &s1) != 0 || s1 != 0) ok = false;
    if (Count(dst[2], &s2) != 1 || s2 != 9 + 12) ok = false;
    if (dst[0]._M_node._M_data == src[0]._M_node._M_data || dst[0]._M_node._M_data->_M_next == src[0]._M_node._M_data->_M_next) ok = false;
    if (std::__uninitialized_copy((const L*)src, (const L*)src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00a82730_TEST FAIL\n"); return 1; }
    std::printf("00a82730_TEST PASS\n");
    return 0;
}
