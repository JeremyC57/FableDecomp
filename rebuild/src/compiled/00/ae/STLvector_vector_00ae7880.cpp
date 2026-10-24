// _STL::vector::vector(unsigned int n) -- STLport sized vector constructor (T = _STL::set<long>) for an array of index sets: allocate n 12-byte sets and copy-construct each from one empty set temporary, then destroy the temporary
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE7880, 214 bytes, __fastcall `ret 4`, returns this.
// Callers: the particle-program adjacency builder 0x00AE2E20 (one set per particle, n = the particle count at +0x20; each
//   16-byte link {a, b} inserts b into set[a] and a into set[b] through the set insert 0x00514C10) and 0x00AE7E78.
// Callees: malloc 0x00BFEA0E (the element block and the temporary's 0x14-byte header node), the _Rb_tree copy constructor
//   0x00AD3A10, the recursive _Rb_tree::_M_erase 0x004CF810 (landed as a descriptive "FreeTree"; the temporary's clear()),
//   free 0x00BFEA14 (the temporary's header).
// Layout: STLport set = _Rb_tree {header-node proxy, _M_node_count, empty _M_key_compare} = 12 bytes; node = colour, parent,
//   left, right + a 4-byte value = 0x14. The value type is not visible in the bytes (4-byte particle indices): `long`.
// Label: catalog "Vector_Construct_Reserve" is descriptive.
// Shape: STLport 4.6 verbatim -- `vector(n) : _Vector_base(n, allocator_type()) { _M_finish = uninitialized_fill_n(_M_start,
//   n, T()); }` with _Vector_base's `_M_start(0), _M_finish(0), _M_end_of_storage(a, 0)` zero stores ahead of the
//   allocation, the fill loop inline (placement new null test, `jbe` from `n > 0`), the temporary set's
//   `_Rb_tree() { _M_empty_initialize(); }` built in the frame and its `~_Rb_tree() { clear(); }` + header deallocate after.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                     // +0x00 (false = red)
    _Base_ptr _M_parent;                                    // +0x04
    _Base_ptr _M_left;                                      // +0x08
    _Base_ptr _M_right;                                     // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                                  // +0x10
};

template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(__a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef _Alloc allocator_type;
    unsigned int _M_node_count;
    _Compare _M_key_compare;

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    void _M_erase(_Link_type __x);                                                             // 0x004CF810
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
    _Rb_tree(const _Compare& __comp, const allocator_type& __a)
        : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp)
    {
        _M_empty_initialize();
    }
    _Rb_tree(const _Rb_tree& __x);                                                             // 0x00AD3A10
    ~_Rb_tree() { clear(); }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
};

template <class _Key, class _Compare = less<_Key>, class _Alloc = allocator<_Key> >
class set {
public:
    typedef _Rb_tree<_Key, _Key, _Identity<_Key>, _Compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
    set() : _M_t(_Compare(), _Alloc()) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(size_type __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};
}

template _STL::vector<_STL::set<long> >::vector(unsigned int);