// std::vector<unsigned short>::vector(size_type n) -- STLport sized constructor: allocate n 16-bit slots through the _Vector_base constructor and fill them with zero
// Leaf: ??0?$vector@G
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AADD80, length 97 bytes, `ret 4`.
// Callees: malloc @ 0x00BFEA0E (allocator<unsigned short>::allocate, null for n == 0).
// Identity: the element size (`lea eax,[edi+edi]`, 16-bit `rep stosw` tail) and STLport's `_Vector_base(n, a)` shape (three
//   zero stores, allocate, start/finish/end) make this vector<unsigned short>'s `explicit vector(size_type n)`. The
//   manifest's "Vector_Construct_uint16_v2" is descriptive.
// Shape: `_Vector_base(n, allocator_type())` inlined (zero stores, allocate, finish = start, end = start + n), then
//   `_M_finish = uninitialized_fill_n(_M_start, n, _Tp())`: the POD (__true_type) path goes to STLport's generic
//   `fill_n` loop, which VC7.1 turns into the `rep stosd ; adc ecx,ecx ; rep stosw` fill idiom (n > 0 unsigned = `jbe`).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

struct __true_type { __true_type() { } };

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

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
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
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    explicit vector(size_type __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};

template vector<unsigned short>::vector(unsigned int);

}   // namespace std