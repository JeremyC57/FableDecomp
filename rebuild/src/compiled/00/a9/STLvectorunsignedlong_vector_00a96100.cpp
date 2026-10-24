// _STL::vector<unsigned long>::vector(unsigned int n) -- STLport sized constructor of a 32-bit word vector: allocate n dwords (none for zero) and zero-fill them
// Leaf: ??0?$vector@K
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A96100, 98 bytes, thiscall `ret 4`, returns this.
// Identity: 5 call sites (0x00A9027D, 0x00A96A28, 0x00B22F90, 0x00F1D160, 0x00F3ACB0) in the 3D primitive code; the element is
//   4 bytes (`lea eax,[edi*4]` allocation, `rep stosd` fill with no word tail) and POD -- modelled as unsigned long, the
//   bytes cannot tell it from any other 4-byte POD. The catalog's "Vector_ZeroInit" is descriptive. Twin of the 2-byte
//   vector(n) 0x00A8D750.
// Callees: CRT malloc 0x00BFEA0E (allocator::allocate, null for n == 0).
// Shape: STLport 4.x `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   uninitialized_fill_n(_M_start, n, _Tp()); }` -- the POD arm `__uninitialized_fill_n(.., __true_type)` is
//   `fill_n`, whose `for (; n > 0; --n, ++first) *first = val;` loop with val == 0 VC7.1 turns into `rep stosd`
//   behind the unsigned `n > 0` test (`jbe`).
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

namespace _STL {

struct __true_type { __true_type() { } };

template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
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

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n)
        : _Vector_base<_Tp, _Alloc>(__n, _Alloc())
    {
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, _Tp());
    }
};

template class vector<unsigned long>;
}