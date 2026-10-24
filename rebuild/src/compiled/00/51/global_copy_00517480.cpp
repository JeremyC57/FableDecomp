// __copy(C2DLineF* first, C2DLineF* last, back_insert_iterator<list<C2DLineF> > result, const random_access_iterator_tag&, int*) -- STLport random-access copy into a back inserter: push every navigation line of [first, last) onto the end of the list
// Leaf: ??$__copy@PAVC2DLineF@@V?$back_insert_iterator
//
// Retail 0x00517480, 122 bytes (+ int3 padding to 128), `ret 10h`: /Gr __fastcall with the hidden back_insert_iterator result in ecx, first in
//   edx; last, the iterator (the list pointer), the category tag and the _Distance* tag on the stack.
// Callee: 0x00BFEA0E malloc (0x18-byte list nodes: links + a 16-byte C2DLineF).
// Callers: copy() 0x00518FB0 (`push 0` = the _Distance* tag), and the navigation setup 0x00509A20 / 0x005198B0 bodies.
// Shape: STLport `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`
//   with back_insert_iterator::operator= -> list::push_back -> insert(end(), x) inline: _M_create_node (malloc +
//   placement copy of the four dwords, null test on node+8) and the link with both neighbours cached (as 0x00517130).
//   `last - first` is the `sar 4` byte difference counted down in ebp; the hidden result pointer is spilled to the
//   `push ecx` slot.  The retail TU is /Gr (explicit __fastcall here).
// Lever: C2DVector keeps its user-written memberwise copy constructor (FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z); with the
//   implicit one VC copies the line through a second pointer register, takes ebp for the result pointer and spills
//   the count to the `last` slot (138 v 122 bytes).
// Label: catalog "LinkedList_InsertRangeFromArray" (propagated).
extern "C" void* __cdecl malloc(unsigned int size);
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C2DVector {
public:
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z
};
class C2DLineF {
public:
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

namespace std {
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
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
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
}

template std::back_insert_iterator<std::list<C2DLineF> > __fastcall
std::__copy<C2DLineF*, std::back_insert_iterator<std::list<C2DLineF> >, int>(
    C2DLineF*, C2DLineF*, std::back_insert_iterator<std::list<C2DLineF> >, const std::random_access_iterator_tag&, int*);