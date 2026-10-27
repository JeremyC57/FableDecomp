// std::__uninitialized_copy(_Elem* first, _Elem* last, _Elem* result, const __false_type&) -- (CNavigationAreaUpdate::CSwitchableBlockage) STLport placement copy-construct each 16-byte record: the 64-bit key, then the list (fresh self-linked header, insert the source range); return the advanced result
// Leaf: ??$__uninitialized_copy@PAVCSwitchableBlockage@CNavigationAreaUpdate@@PAV12@@std@@YIPAVCSwitchableBlockage@CNavigationAreaUpdate@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0051a810, 109 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): callers the landed std::vector<CNavigationAreaUpdate::CSwitchableBlockage>::_M_insert_overflow 0x0051C1F0 (head and tail copies, non-const pointers) and 0x00509A20; Ego_r.pdb CNavigationAreaUpdate::CSwitchableBlockage is {ulong64 UniqueID; list<C2DLineF> LineList} (16 bytes with the 4-byte STLport list), the element of CNavigationLayerUpdate::SwitchableBlockages; its copy calls list<C2DLineF>::_M_insert_dispatch.
// Callees: malloc 0x00BFEA0E (the 0x18-byte list header), std::list<C2DLineF>::_M_insert_dispatch<const_iterator>(iterator,
//   const_iterator, const_iterator, const __false_type&) 0x005152C0 (thiscall, `ret 10h`).
// Shape (family L005, model the landed 0x00A82730): the record's implicit copy constructor = two dword moves for the key and
//   STLport list(const list&) (`_List_base(a)` nulls the node word before the malloc, links the header to itself, then
//   `insert(begin(), x.begin(), x.end())`); `pos` is built in its argument slot (the iterator has a user copy constructor)
//   and the __false_type temporary is homed in the dead `last` slot; placement-new null test, result returned.
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

}
class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {                            // Ego_r.pdb; retail 16 bytes (4-byte STLport list)
    public:
        unsigned __int64 UniqueID;                        // +0x00
        std::list<C2DLineF> LineList;                     // +0x08
    };
};
typedef CNavigationAreaUpdate::CSwitchableBlockage _Elem;

template _Elem* __fastcall std::__uninitialized_copy<_Elem*, _Elem*>(_Elem*, _Elem*, _Elem*, const std::__false_type&);