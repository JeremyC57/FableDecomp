// std::vector<CVertexGroupBlock>::reserve(unsigned int n) -- grow the optimiser's block vector's storage to n blocks: copy the blocks into new storage, destroy and free the old run
// Flags: /Gr
// Leaf: ?reserve@?$vector@VCVertexGroupBlock
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADCB90, 0xE2 bytes (+ int3 padding), thiscall, `ret 4`.
// Identity: 0x18-byte elements (`/ 24` by `imul 2AAAAAABh ; sar edx,2`) copied by __uninitialized_copy<CVertexGroupBlock*,
// CVertexGroupBlock*> 0x00ADC330 and destroyed by __destroy_aux<CVertexGroupBlock*> 0x00ADC190: Ego_r.pdb
// vector<CVertexGroupBlock>::reserve. Catalog "Vector_Reallocate_Sized" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocate(n)), __uninitialized_copy @ 0x00ADC330, __destroy_aux @ 0x00ADC190, free @
// 0x00BFEA14 (the old storage).
// Shape: STLport 4.5 `if (capacity() < n) { old_size = size(); if (_M_start) { tmp = _M_allocate_and_copy(n, _M_start,
// _M_finish); _M_clear(); } else tmp = allocate(n); _M_set(tmp, tmp + old_size, tmp + n); }` -- both empty tags
// (__false_type) are homed on the LIVE `n` argument slot (`lea edx,[esp+18h]`), which needs the two callees' real
// bodies out of line in the TU (same-TU stand-ins below, under auto_inline(off)).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}

template <class T> class allocator {                        // STLport allocator<T>: empty
public:
    allocator() { }
    allocator(const allocator&) { }
    T* allocate(unsigned int n) { return n != 0 ? static_cast<T*>(malloc(n * sizeof(T))) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};

template <class T> class _STLP_alloc_proxy : public allocator<T> {
public:
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};

#pragma auto_inline(off)
// Same-TU stand-ins for the out-of-line range helpers (retail 0x00ADC330 / 0x00ADC190): neither touches its tag, which
// lets VC7.1 home the caller's empty tags on the live `n` slot as retail does.
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIterator>
__declspec(noinline) void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
#pragma auto_inline(on)

template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}
template <> inline void _Destroy<unsigned long*>(unsigned long*, unsigned long*) { }   // trivial destructor

template <class T> class _Vector_base {
public:
    T*                    _M_start;                         // +0
    T*                    _M_finish;                        // +4
    _STLP_alloc_proxy<T>  _M_end_of_storage;                // +8
    _Vector_base(unsigned int n, const allocator<T>& a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class T> class vector : public _Vector_base<T> {
public:
    typedef T* pointer;
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    unsigned int capacity() const { return (unsigned int)(this->_M_end_of_storage._M_data - this->_M_start); }
    allocator<T> get_allocator() const { return allocator<T>(); }
    vector(const vector& x)                                 // instantiated for the POD group words only
        : _Vector_base<T>(x.size(), x.get_allocator())
    {
        this->_M_finish = (T*)__copy_trivial(x._M_start, x._M_finish, this->_M_start);
    }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }

    template <class _ForwardIterator>
    pointer _M_allocate_and_copy(unsigned int __n, _ForwardIterator __first, _ForwardIterator __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void reserve(unsigned int __n);
};

template <class T>
void vector<T>::reserve(unsigned int __n)
{
    if (capacity() < __n) {
        const unsigned int __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

struct _Rb_tree_node_base {
    bool                _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class V> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node> _M_header;                     // +0x00
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
};

template <class K, class V, class Compare> class _Rb_tree : public _Rb_tree_base<V> {
public:
    typedef _Rb_tree_node<V>    _Node;
    typedef _Node*              _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    unsigned int _M_node_count;                             // +0x04
    Compare      _M_key_compare;                            // +0x08

    _Rb_tree(const _Rb_tree& __x);                          // 0x00A96E40
    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }
    static _Link_type& _S_left(_Base_ptr x) { return (_Link_type&)x->_M_left; }
    static _Link_type& _S_right(_Base_ptr x) { return (_Link_type&)x->_M_right; }

    void _M_erase(_Link_type __x);                          // 0x004507A1
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
    ~_Rb_tree() { clear(); }
};

template <class K, class V, class Compare>
void _Rb_tree<K, V, Compare>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        this->_M_header.deallocate(__x, 1);
        __x = __y;
    }
}

template <class K, class T, class Compare = less<K> > class map {
public:
    typedef pair<const K, T> value_type;
    _Rb_tree<K, value_type, Compare> _M_t;
    map(const map& __x) : _M_t(__x._M_t) { }
};

} // namespace std

class CVertexGroupBlock {                                   // Ego_r.pdb, 0x18 bytes in retail
public:
    std::vector<unsigned long>    Groups;                  // +0x00
    std::map<unsigned long, long> GroupToIndexMap;         // +0x0C
};

namespace std {
template void vector<CVertexGroupBlock>::reserve(unsigned int);
} // namespace std