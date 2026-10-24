// std::vector<unsigned char>::vector(size_type n, const unsigned char& val, const allocator_type& a) -- STLport fill constructor: allocate n bytes and memset them to val
// Leaf: ??0?$vector
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567560, 101 bytes, thiscall `ret 0Ch` (n, &val, &allocator). Callers: the
//   scoreboard/transaction code at 0x0055F0B8, 0x0055F167, 0x00560BE3 (byte buffers). The catalog label
//   "Vector_Fill_Byte" is descriptive.
// Callees: malloc @ 0x00BFEA0E (allocator<unsigned char>::allocate, null for n == 0).
// Shape: STLport 4.x verbatim: `_Vector_base(n, a)` zeroes start/finish/end (the end-of-storage word through the
//   _STLP_alloc_proxy), allocates and sets the three words; the body is `_M_finish = uninitialized_fill_n(
//   _M_start, n, val)`, which for the POD byte type goes to STLport's one-byte `fill_n` -> `fill` ->
//   `memset(first, tmp, last - first)` specialisation (the /Oi intrinsic: byte replicated into eax, `rep stosd`
//   + `rep stosb`), returning first + n (n re-read from its stack slot).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

namespace std {

struct __true_type { };
struct __false_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
{
    unsigned char __tmp = __val;
    memset(__first, __tmp, __last - __first);
}

template <class _Size>
inline unsigned char* fill_n(unsigned char* __first, _Size __n, const unsigned char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __true_type&)
{
    return fill_n(__first, __n, __x);
}

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __true_type());
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
    typedef _Vector_base<_Tp, _Alloc> _Base;
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    vector(size_type __n, const _Tp& __val, const allocator_type& __a = allocator_type())
        : _Vector_base<_Tp, _Alloc>(__n, __a)
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
};

template vector<unsigned char>::vector(unsigned int, const unsigned char&, const allocator<unsigned char>&);

}