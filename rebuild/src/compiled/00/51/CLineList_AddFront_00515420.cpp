// CLineList::AddFront(const C2DLineF& line) -- push a copy of a 16-byte line record at the front of an STLport list and return the list (chainable)
// Leaf: ?AddFront@CLineList@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00515420, 73 bytes, `ret 4` (thiscall: the record reference; returns `this`).
// Identity: no caller or absolute reference survives in the image (a COMDAT left by an inlined use), so the owner and
//   the method name are descriptive. `this` IS the list (its sentinel pointer at +0 is read for begin()); the node is
//   0x18 bytes and the element a 16-byte POD copied word by word -- the C2DLineF list of the neighbouring bodies
//   0x005152C0 / 0x005155F0. The catalog's "StdList_InsertNode" is descriptive.
// Callee: malloc @ 0x00BFEA0E (node allocate(1) = malloc(0x18)).
// Shape: STLport `push_front(x) { insert(begin(), x); }` with insert / _M_create_node inlined (begin() = the sentinel's
//   _M_next read before the allocation, placement null test, four dword copies through ebx, `prev = pos->_M_prev`
//   before `node->_M_next = pos`), then `return *this` (`mov eax,edi`).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X, Y;
};

class C2DLineF {                                    // Ego_r.pdb, 16 bytes
public:
    C2DVector Point1;                               // +0x00
    C2DVector Point2;                               // +0x08
};

namespace std {

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
    _List_node_base* _M_next;                       // +0
    _List_node_base* _M_prev;                       // +4
};

template <class T> struct _List_node : public _List_node_base {
    T _M_data;                                      // +8
};

struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};

template <class T> struct _Nonconst_traits { typedef T& reference; };

template <class T, class Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    typedef _List_node<T> _Node;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
};

template <class T, class A> class _List_base {
public:
    typedef _List_node<T> _Node;
    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_node;    // +0
};

template <class T, class A = allocator<T> > class list : public _List_base<T, A> {
public:
    typedef _List_node<T> _Node;
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;

    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    protected:
    _Node* _M_create_node(const T& x)
    {
        _Node* p = this->_M_node.allocate(1);
        _Construct(&p->_M_data, x);
        return p;
    }
    public:
    iterator insert(iterator position, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = position._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
    void push_front(const T& x) { insert(begin(), x); }
};

}

class CLineList : public std::list<C2DLineF> {
public:
    CLineList& AddFront(const C2DLineF& line);
};

CLineList& CLineList::AddFront(const C2DLineF& line)
{
    push_front(line);
    return *this;
}