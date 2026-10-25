// std::__destroy_aux(std::set<long>* first, std::set<long>* last, const __false_type&) -- STLport range destroy for an array of index sets: run each set's destructor (erase every node, reset the header, free the header node)
// Leaf: ??$__destroy_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE76E0, 114 bytes, `ret 4`. /Gr algorithm TU written as explicit __fastcall: first in
//   ecx, last in edx (spilled to the pushed-ecx frame word and reloaded after the inner loop), the tag reference on the stack.
// Callers: the particle-program adjacency builder 0x00AE2E20's vector<set<long> > teardown (0x00AE3311) and the vector's
//   _M_clear / erase paths (0x00AE7A66, 0x00AE7BD6, 0x00AE7D9E, 0x00AE7E9E).
// Callees: the recursive _Rb_tree::_M_erase 0x004CF810 (landed as a descriptive "FreeTree"), free 0x00BFEA14.
// Layout: STLport set = _Rb_tree {header-node proxy, _M_node_count, empty _M_key_compare} = 12 bytes; node = colour, parent,
//   left, right + a 4-byte value (the vector<set<long> > of the sized constructor 0x00AE7880).
// Label: catalog "LTextTreeWalkThrough_DestroyRange_Range" is descriptive (0x004CF810 carries a propagated LTextTreeWalkThrough
//   label).
// Shape: STLport 4.6 `for (; first != last; ++first) _Destroy(&*first);` with `~_Rb_tree() { clear(); }` and
//   `~_Rb_tree_base() { deallocate(header, 1); }` inline. _M_erase's body is in the TU, so VC7.1 inlines its outer
//   `while (x != 0) { _M_erase(right(x)); y = left(x); deallocate(x, 1); x = y; }` level (the free without a null test)
//   and calls itself out of line for the right subtrees.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E (not called here)
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14

namespace std {
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
    void _M_erase(_Link_type __x);                                                             // 0x004CF810 (body below)
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

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    // erase without rebalancing
    while (__x != 0) {
        _M_erase((_Link_type)__x->_M_right);
        _Base_ptr __y = __x->_M_left;
        this->_M_header.deallocate(__x, 1);
        __x = (_Link_type)__y;
    }
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

template void __fastcall std::__destroy_aux<std::set<long>*>(std::set<long>*, std::set<long>*, const std::__false_type&);