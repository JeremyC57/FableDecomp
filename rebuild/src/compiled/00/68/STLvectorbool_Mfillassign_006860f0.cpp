// _STL::vector<bool>::_M_fill_assign(size_type n, const bool& val) -- STLport fill assign of a byte vector: rebuild through a temporary when n exceeds the capacity, else overwrite in place and grow or erase the tail
// Leaf: ?_M_fill_assign@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006860F0, length 205 bytes (+ int3 padding), `ret 8` (thiscall: n, &val).
// Callers: 0x006821F9 (`assign(0x5C, true)` on the member vector at +0x3C) and the assign forwarder 0x006864E0 (`jmp`).
// Callees: vector(size_type n, const bool& val, const allocator_type& a) @ 0x00451583 (landed: the fill constructor of
//   the temporary), free @ 0x00BFEA14 (the temporary's destructor after the swap), memmove @ 0x00BFEAE6 (erase's
//   __copy_trivial).
// Label: catalog "std::vector_insert_value" is descriptive; the body is STLport 4.x `_M_fill_assign` verbatim:
//   `if (__n > capacity()) { vector __tmp(__n, __val, get_allocator()); __tmp.swap(*this); } else if (__n > size()) {
//   fill(begin(), end(), __val); _M_finish = uninitialized_fill_n(_M_finish, __n - size(), __val); } else
//   erase(fill_n(begin(), __n, __val), end());`
// Layout: retail STLport's vector<bool> is a plain byte vector (as the landed fill constructor 0x00451583 models it):
//   start, finish, and the end of storage in an _STLP_alloc_proxy over the empty allocator.
// Shape: the swap leaves only the old start live for the temporary's destructor (`if (_M_start) free(_M_start)`); the
//   fills re-read val through its reference every pass (a byte store may alias it); erase is `__copy_ptrs(last,
//   _M_finish, first, __true_type)` = `__copy_trivial` (`last == first ? result : memmove(...) + n`); the allocator
//   temporary handed to the constructor sits in the dead `&val` slot. The temporary's constructors (0x00451583 and its
//   _Vector_base 0x00450CCC) are same-TU noinline bodies: declaration-only, VC7.1 assumes they keep &__tmp and leaves
//   the three dead swap stores into the temporary (221v205).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void  __cdecl free(void* block);                                       // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size); // 0x00BFEAE6

namespace _STL {
struct __true_type { __true_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp> inline void swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result :
        ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) +
        ((const char*)__last - (const char*)__first);
}
template <class _Tp> inline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __true_type&)
{
    return (_Tp*)__copy_trivial(__first, __last, __result);
}

template <class _ForwardIter, class _Tp>
inline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for (; __first != __last; ++__first)
        *__first = __val;
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return fill_n(__first, __n, __x);                    // the POD path of __uninitialized_fill_n
}
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;  // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a);      // 0x00450CCC (same-TU noinline body below)
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef vector<_Tp, _Alloc> _Self;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    allocator_type get_allocator() const { return this->_M_end_of_storage; }
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    vector(size_type __n, const _Tp& __val, const allocator_type& __a);   // 0x00451583 (same-TU noinline body below)
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void swap(_Self& __x)
    {
        _STL::swap(this->_M_start, __x._M_start);
        _STL::swap(this->_M_finish, __x._M_finish);
        _STL::swap(this->_M_end_of_storage._M_data, __x._M_end_of_storage._M_data);
    }
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __true_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void _M_fill_assign(size_type __n, const _Tp& __val);
};

// The temporary's two constructors get their STLport bodies as same-TU noinline functions: with them visible VC7.1
// knows neither keeps the temporary's address, so the swap's stores into the temporary die (retail stores nothing back).
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
__declspec(noinline) _Vector_base<_Tp, _Alloc>::_Vector_base(unsigned int __n, const _Alloc& __a)
    : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
{                                                           // 0x00450CCC
    _M_start = _M_end_of_storage.allocate(__n);
    _M_finish = _M_start;
    _M_end_of_storage._M_data = _M_start + __n;
}
template <class _Tp, class _Alloc>
__declspec(noinline) vector<_Tp, _Alloc>::vector(size_type __n, const _Tp& __val, const allocator_type& __a)
    : _Vector_base<_Tp, _Alloc>(__n, __a)
{                                                           // 0x00451583
    this->_M_finish = fill_n(this->_M_start, __n, __val);
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_assign(size_type __n, const _Tp& __val)
{
    if (__n > capacity()) {
        vector<_Tp, _Alloc> __tmp(__n, __val, get_allocator());
        __tmp.swap(*this);
    } else if (__n > size()) {
        fill(begin(), end(), __val);
        this->_M_finish = uninitialized_fill_n(this->_M_finish, __n - size(), __val);
    } else
        erase(fill_n(begin(), __n, __val), end());
}
}

template void _STL::vector<bool>::_M_fill_assign(unsigned int, const bool&);