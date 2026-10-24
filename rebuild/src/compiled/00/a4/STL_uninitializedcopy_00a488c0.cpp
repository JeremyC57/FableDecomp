// _STL::__uninitialized_copy(const CLipSyncPhoneme* first, const CLipSyncPhoneme* last, CLipSyncPhoneme* result, const __false_type&) -- placement copy-construct lip-sync phonemes into raw storage, return the advanced result
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A488C0, length 58 bytes, `ret 8`.
// Callers: 0x00A4898C, 0x00A49CB7, 0x00A4A58E and 0x00A4A63A (vector<CLipSyncPhoneme> copy / insert helpers). Callee: CCharString copy ctor 0x0099EC30.
// Identity: the catalog's `_Copy_backward<CBrainUpdateZone*,...>` is propagated (CBrainUpdateZone holds three longs and no
//   string). The 12-byte element {CCharString, dword, dword} is Ego_r.pdb CLipSyncPhoneme (Symbol +0, ID +4, Weight +8),
//   the element of the lip-sync TU's vectors around it; `ret 8` + the placement null test is STLport
//   `__uninitialized_copy(first, last, result, const __false_type&)`. Byte twin of 0x00A48670 (same source).
// Shape: /Gr TU (`__fastcall` template: first in ecx -> esi, last in edx -> ebx, result and the tag reference on the
//   stack); `_Construct` is a placement new (null test on the destination) running the implicit copy ctor (the
//   string's out-of-line copy ctor, then two dword moves); the /O2 loop-head pad is a `nop`.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class CCharString {
public:
    void* m_pData;                                   // +0x00
    CCharString(const CCharString& other);           // 0x0099EC30
};

class CLipSyncPhoneme {                              // 12 bytes
public:
    CCharString   Symbol;                            // +0x00
    unsigned long ID;                                // +0x04
    float         Weight;                            // +0x08
};

namespace _STL {
struct __false_type { __false_type() { } };          // STLport _IS_POD_ITER result for a class element

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CLipSyncPhoneme* __fastcall __uninitialized_copy(const CLipSyncPhoneme*, const CLipSyncPhoneme*, CLipSyncPhoneme*, const __false_type&);
}