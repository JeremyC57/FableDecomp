// pair<unsigned __int64, list<C2DLineF> >::pair(const pair& o) -- STLport pair copy constructor (out of line) of CNavigationLayer's CVectorMap<unsigned __int64, list<C2DLineF> > element: copy the 64-bit key, then copy-construct the line list (fresh sentinel + insert(begin(), o.begin(), o.end()))
// Leaf: ??0?$pair@_KV?$list@VC2DLineF@@
//
// Retail 0x005199E0, 79 bytes, thiscall `ret 4`.  Callees: 0x00BFEA0E malloc (the list's sentinel node; 0x18-byte
//   nodes = links + a 16-byte C2DLineF), 0x005152C0 list<C2DLineF>::_M_insert_dispatch(iterator pos, const_iterator
//   first, const_iterator last, const __false_type&) (`ret 10h`).
// Callers: the vector's push_back 0x0051C850 (landed), _M_insert_overflow 0x0051C1F0, 0x00509CC3 and 0x0051A805.
// Identity: Ego_r.pdb CNavigationLayer +0x20 = CVectorMap<unsigned __int64, list<C2DLineF>, CKeyPairCompareLess<..> >
//   (C2DLineF = { C2DVector Point1, Point2 }, 16 bytes), the one 64-bit-key list map in the PDB; the landed 0x00A84470
//   sort_heap documents the same (unsigned __int64, list) record (+0xC is padding after the list).
// Shape: as 0x00517590 -- STLport `pair(const pair& o) : first(o.first), second(o.second) {}` with the list copy
//   constructor inline and a same-TU __declspec(noinline) _M_insert_dispatch body, so the empty `__false_type()`
//   temporary overlays the dead argument slot (`lea edx,[esp+10h]`).
// Label: catalog "std_vector_from_vector" (propagated).
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
    __declspec(noinline) void _M_insert_dispatch(_List_iterator pos, _List_const_iterator first, _List_const_iterator last,
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

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) {}
};
}

template std::pair<unsigned __int64, std::list<C2DLineF> >::pair(const std::pair<unsigned __int64, std::list<C2DLineF> >&);