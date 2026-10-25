// _STL::__pop_heap(CWideString* first, CWideString* last, CWideString* result, CWideString val, NGameComponentHelpers::CCaseInsensitiveStringCompare comp, int*) -- STLport heap pop step of a case-insensitive wide-string sort: move the top string to *result, then sift val down from the root
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00497310, 63 bytes (+ int3), /Gr `__fastcall` (first in ecx -> edi, last in edx
//   -> esi; result, the 4-byte string by value, the comparator word and the distance pointer on the stack, `ret 10h`).
// Identity: STLport 4.6 `__pop_heap(__first, __last, __result, __val, __comp, _Distance*)` of the wide-string sort whose
//   landed __adjust_heap 0x00497160 orders by NGameComponentHelpers::CCaseInsensitiveStringCompare (`_wcsicmp < 0`);
//   no direct callers (an emitted COMDAT of the TU's sort_heap/partial_sort). The catalog's
//   "swap<CTCBase::CRegisteredTCInfo>" is propagated from a byte twin.
// Callees: CWideString::operator= 0x0099B7D0 (`*result = *first`), CWideString copy ctor 0x0099B720 (val rebuilt in the
//   argument area), __adjust_heap 0x00497160 (`__fastcall` first, hole 0 in edx), ~CWideString 0x0099B510 (the by-value
//   val, owned by this frame).
// Shape: the STLport body verbatim: the by-value string argument is copy-built in its stack slot (`push ecx ; mov
//   ecx,esp`) after the comparator word is pushed.
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};

namespace NGameComponentHelpers {
class CCaseInsensitiveStringCompare {                   // empty, passed by value (one stack word)
public:
    bool operator()(const CWideString& a, const CWideString& b) const;
};
}

namespace _STL {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
                              _Distance __len, _Tp __val, _Compare __comp);   // 0x00497160

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp,
                                  _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap<CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare, int>(
    CWideString*, CWideString*, CWideString*, CWideString, NGameComponentHelpers::CCaseInsensitiveStringCompare, int*);
}