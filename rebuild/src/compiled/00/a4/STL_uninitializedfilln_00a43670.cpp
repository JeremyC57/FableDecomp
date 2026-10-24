// _STL::uninitialized_fill_n(NGameText::CDataBankTextEntry* first, unsigned int n, const NGameText::CDataBankTextEntry& x) -- STLport three-argument front end: placement copy-construct n text-bank entries from x (add-ref each text entry), return first + n
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43670, length 47 bytes (+1 int3 pad), `ret 4`.
// Caller: 0x00A43D71 (the vector<NGameText::CDataBankTextEntry> fill-insert helper; `push &x ; mov edx,n ; mov ecx,first`).
// Identity: the catalog label `_Fill_n<unsigned long*, unsigned int, unsigned long>` is propagated. Byte twin of 0x00A43500
//   (the four-argument `__uninitialized_fill_n(..., const __false_type&)`, `ret 8`) except for the `ret 4`: this is the
//   top-level `uninitialized_fill_n(first, n, x)` with `__uninitialized_fill_n(first, n, x, _IS_POD_ITER(...))` inlined.
//   Element NGameText::CDataBankTextEntry (8 bytes): PText CIVCountedPointer<NGameText::CTextEntry> +0x00 (intrusive
//   count at +0x04 of the pointee), NarratorIndex long +0x04 (landed neighbours 0x00A43630 / 0x00A43720 / 0x00A43890).
// Shape: /Gr STLport TU (`__fastcall` template: first in ecx -> eax, n in edx -> esi counted down, &x on the stack); the
//   inner helper must be `__forceinline` (GOTCHAS: 3-argument uninitialized_fill_n front ends, 0x00440180 / 0x007688B0);
//   `__n > 0` on the unsigned count is the `jbe` entry test; placement-new null test; CIVCountedPointer's user copy ctor.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

namespace _STL {
struct __false_type { __false_type() { } };     // STLport _IS_POD_ITER result for a class element
}

namespace NGameText {
struct CTextEntry {                             // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};
}

template <class T>
struct CIVCountedPointer {                      // 4 bytes
    T* P;                                       // +0x00
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};

namespace NGameText {
struct CDataBankTextEntry {                     // 8 bytes
    CIVCountedPointer<CTextEntry> PText;        // +0x00
    long                          NarratorIndex; // +0x04
};
}

namespace _STL {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
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

template NGameText::CDataBankTextEntry* __fastcall uninitialized_fill_n(NGameText::CDataBankTextEntry*, unsigned int,
    const NGameText::CDataBankTextEntry&);
}