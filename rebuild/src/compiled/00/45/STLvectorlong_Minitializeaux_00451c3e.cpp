// _STL::vector<long>::_M_initialize_aux(int n, int value, const __true_type&) -- the integral-argument path of vector<long>'s range constructor: allocate exactly n longs and fill them all with `value`
// Leaf: ??$_M_initialize_aux@H@?$vector@J
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00451C3E, 69 bytes (size-optimised TU, no padding), thiscall, `ret 0Ch`.
// Identity: the only caller 0x00451C3E <- 0x00453673 is vector<long>::vector<int>(int first, int last, const allocator&)
//   @ 0x00453659 (catalog `vector<long,std::allocator<long>_><int>`): _Vector_base(a) zeroes the three pointers, then
//   `_M_initialize_aux(first, last, _Integral())` with STLport's `_Is_integer<int>::_Integral` = __true_type (pushed by
//   address). The catalog name `std::vector_Fill` is descriptive.
// Callees: malloc @ 0x00BFEA0E (Fable's STLport allocator; `n != 0 ? malloc(n * 4) : 0`).
// Shape: STLport 4.6 `_M_start = allocate(n); _M_end_of_storage._M_data = _M_start + n; _M_finish =
//   uninitialized_fill_n(_M_start, n, val);` -- the POD fill is `fill_n`'s `for (; n > 0; --n, ++first) *first = val;`,
//   which the /Os TU turns into `rep stosd` (the returned end pointer = start + n*4); `#pragma optimize("s", on)` as the
//   module's other landed bodies (no frame pointer here); under "s" the allocator's allocate, uninitialized_fill_n and
//   fill_n must be `__forceinline` (plain inline ones are called out of line).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

namespace _STL {
struct __true_type { };

template <class _Tp> class allocator {
public:
    allocator() { }
    __forceinline _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _OutputIter, class _Size, class _Tp>
__forceinline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return fill_n(__first, __n, __x);                       // __uninitialized_fill_n(..., __true_type): POD
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x0
    _Tp* _M_finish;                                         // +0x4
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x8
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    template <class _Integer>
    void _M_initialize_aux(_Integer __n, _Integer __val, const __true_type&)
    {
        this->_M_start = this->_M_end_of_storage.allocate(__n);
        this->_M_end_of_storage._M_data = this->_M_start + __n;
        this->_M_finish = uninitialized_fill_n(this->_M_start, __n, __val);
    }
};

template void vector<long>::_M_initialize_aux<int>(int, int, const __true_type&);
}