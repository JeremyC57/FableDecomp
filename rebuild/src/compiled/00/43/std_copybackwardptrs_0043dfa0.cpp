// std::__copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result, const __true_type&) -- STLport trivial copy_backward: one memmove of [first, last) to end at result
// Leaf: ??$__copy_backward_ptrs@PAUCUnprovenPod_0043dfa0@@PAU1@@std@@YIPAUCUnprovenPod_0043dfa0@@PAU1@00ABU__true_type@0@@Z
// Retail: Fable.exe @ 0x0043DFA0, 26 bytes, __fastcall (first in ecx, last in edx; result and the __true_type tag on the
// stack, `ret 8`): `sub edx,ecx ; test edx,edx ; jle` (no copy, result returned) else `memmove(result - n, first, n)`.
// Identity: STLport 4.6 `__copy_backward_ptrs(.., const __true_type&)` -> `__copy_trivial_backward` (the landed
// _M_fill_insert 0x005686C0 carries the same text), the out-of-line COMDAT a vector<POD>::insert keeps. No caller in
// retail (gap-scan COMDAT) and the body is element-size free, so the element type is unproven: CUnprovenPod_0043dfa0
// (descriptive).
// Callee: memmove @ 0x00BFEAE6 (MSVCR71 import thunk).
extern "C" void* __cdecl memmove(void* dst, const void* src, unsigned int size);   // 0x00BFEAE6

struct CUnprovenPod_0043dfa0 {
    long _M_unproven;
};

namespace std {
struct __true_type { __true_type() { } };

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                   _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template CUnprovenPod_0043dfa0* __fastcall __copy_backward_ptrs<CUnprovenPod_0043dfa0*, CUnprovenPod_0043dfa0*>(
    CUnprovenPod_0043dfa0*, CUnprovenPod_0043dfa0*, CUnprovenPod_0043dfa0*, const __true_type&);
}