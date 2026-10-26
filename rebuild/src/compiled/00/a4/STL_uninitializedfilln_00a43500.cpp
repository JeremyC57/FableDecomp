// std::__uninitialized_fill_n(NGameText::CDataBankTextEntry* first, unsigned int n, const NGameText::CDataBankTextEntry& x, const __false_type&) -- placement copy-construct n text-bank entries from x (add-ref each text entry), return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A43500, length 47 bytes (+1 int3 pad), `ret 8`.
// Caller: 0x00A43B20 vector<NGameText::CDataBankTextEntry>::_M_insert_overflow (the fill_len != 1 arm:
//   `push &tag ; push &x ; mov edx,n ; mov ecx,newFinish`).
// Identity: the catalog label `_Fill_n<unsigned long*, unsigned int, unsigned long>` is propagated. The 8-byte element is
//   NGameText::CDataBankTextEntry (the landed neighbours 0x00A43630 __uninitialized_copy, 0x00A43720 __copy_backward,
//   0x00A43890 fill model it): PText CIVCountedPointer<NGameText::CTextEntry> +0x00 (the intrusive count lives at +0x04
//   of the pointee), NarratorIndex long +0x04. The four-argument STLport helper with the `__false_type` tag (`ret 8`);
//   its byte twin 0x00A43670 is the three-argument front end (`ret 4`).
// Shape: /Gr STLport TU (`__fastcall` template: first in ecx -> eax as `cur`, n in edx -> esi counted down, &x and the
//   tag on the stack); `__n > 0` on an unsigned count is the `jbe` entry test; `_Construct` is a placement new (its null
//   test on the destination) running the entry's implicit copy constructor (CIVCountedPointer's user copy ctor
//   `P(x.P) { if (P) ++P->RefCount; }`, then the narrator index); the /O2 loop-head pad `lea ecx,[ecx]`.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

namespace std {
struct __false_type { __false_type() { } };     // STLport _IS_POD_ITER result for a class element
}

namespace NGameText {
class CTextEntry { public:                             // intrusively counted pointee
    void* __vftable;                            // +0x00
    long  RefCount;                             // +0x04
};
}

template <class T>
class CIVCountedPointer { public:                      // 4 bytes
    T* P;                                       // +0x00
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};

namespace NGameText {
class CDataBankTextEntry { public:                     // 8 bytes
    CIVCountedPointer<CTextEntry> PText;        // +0x00
    long                          NarratorIndex; // +0x04
};
}

namespace std {
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template NGameText::CDataBankTextEntry* __fastcall __uninitialized_fill_n(NGameText::CDataBankTextEntry*, unsigned int,
    const NGameText::CDataBankTextEntry&, const __false_type&);
}