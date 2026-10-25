// std::__uninitialized_copy(const list<C2DLineF>* first, const list<C2DLineF>* last, list<C2DLineF>* result, const __false_type&) -- STLport placement copy-construct each line list (fresh self-linked header, then insert the source range), return the advanced result
// Leaf: ??$__uninitialized_copy@PBV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@std@@PAV12@@std@@YIPAV?$list@VC2DLineF@@V?$allocator@VC2DLineF@@@std@@@0@PBV10@0PAV10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82730, length 87 bytes, `ret 8`.
// Callers: 0x00A82AA9 (0x00A829E0), 0x00A831B7 (vector copy constructor 0x00A83170), 0x00A83DC7 / 0x00A83E66 (0x00A83D70),
//   0x00A854DD (the landed record __uninitialized_copy 0x00A85420) and 0x00A8561C (0x00A85560): the vector<list<C2DLineF> >
//   helpers of the navigation quad-tree TU (Ego_r struct_layouts CNavigationLayer::LineLists = vector<list<C2DLineF> >).
// Callees: malloc @ 0x00BFEA0E (the 0x18-byte header node), list<C2DLineF>::_M_insert_dispatch<const_iterator>(iterator,
//   const_iterator, const_iterator, const __false_type&) @ 0x005152C0 (`ret 10h`; landed under that name, its element is
//   C2DLineF {C2DVector Point1, Point2}).
// Identity: was landed as the non-template stand-in UninitCopy_List16_00a82730 over a "SValue16" record; the value is
//   C2DLineF (FINDINGS chunk 422; the callee 0x005152C0 is the list<C2DLineF> range insert). The catalog label
//   `_Uninit_copy<_Vector_const_iterator<..unsigned int..>, ...>` is propagated. Byte twin of 0x00A831D0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)`, `__fastcall` (first in ecx -> edi,
//   last in edx -> ebx, result + the tag reference on the stack -> esi). list's copy constructor is `_List_base(a)` -- the
//   alloc proxy nulls the node word BEFORE the malloc, the header is linked to itself and stored -- then
//   `insert(begin(), x.begin(), x.end())` forwarding to the out-of-line `_M_insert_dispatch(pos, first, last, _Integral())`:
//   the iterator has a user copy constructor so `pos` is built in its argument slot (`push ecx ; mov ecx,esp ; mov [ecx],eax`),
//   the const_iterators are pushed as plain words, and the empty __false_type temporary is homed in the dead `result`
//   argument slot (`lea ebp,[esp+14h]`). Placement-new null test; the advanced result returned from esi.
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
    void _M_insert_dispatch(iterator position, InputIter first, InputIter last, const __false_type&);   // 0x005152C0
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

template list<C2DLineF>* __fastcall __uninitialized_copy<const list<C2DLineF>*, list<C2DLineF>*>(
    const list<C2DLineF>*, const list<C2DLineF>*, list<C2DLineF>*, const __false_type&);

}
