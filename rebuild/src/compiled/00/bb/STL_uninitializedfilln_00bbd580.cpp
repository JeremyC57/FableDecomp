// _STL::uninitialized_fill_n(SPairByteLong* first, unsigned int n, const SPairByteLong& x) -- placement copy-construct n copies of an 8-byte {byte, long} pair at first, return the advanced pointer
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BBD580, 40 bytes, __fastcall (first ecx, n edx), `ret 4` (the value reference).
// Caller: the vector<pair> `_M_fill_insert` helper 0x00BBD950 (the "fewer elements after the position" arm), whose
//   neighbours are the landed __uninitialized_copy 0x00BBD520 and fill 0x00BBD380 over the same 8-byte pair.
// Identity: catalog `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` is a propagated Dinkumware spelling; the
//   8-byte element copied as one byte (+0) and one dword (+4) is the pair<bool, long> of 0x00BBD520 (descriptive name).
// Shape: STLport 4.x `uninitialized_fill_n(first, n, x)` with the `__false_type` overload of `__uninitialized_fill_n`
//   inlined: `for (; n > 0; --n, ++cur) _Construct(&*cur, x)` (unsigned n: `test edx,edx ; jbe`), placement-new null
//   test, the advanced pointer returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }

struct SPairByteLong {                       // _STL::pair<bool, long>, 8 bytes
    bool first;                              // +0x00
    long second;                             // +0x04
    SPairByteLong(const SPairByteLong& o) : first(o.first), second(o.second) { }
};

namespace _STL {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template SPairByteLong* __fastcall uninitialized_fill_n<SPairByteLong*, unsigned int, SPairByteLong>(SPairByteLong*, unsigned int, const SPairByteLong&);

} // namespace _STL