// _STL::__pop_heap(CWideString* first, CWideString* last, CWideString* result, CWideString val, less<CWideString> comp, int*) -- STLport heap pop step of a wide-string sort: move the top string to *result, then sift val down from the root
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B3A40, 63 bytes, /Gr `__fastcall` (first in ecx -> edi, last in edx -> esi;
//   result, the 4-byte string by value, the comparator word and the distance pointer on the stack, `ret 10h`).
// Identity: STLport 4.6 `__pop_heap` of the `sort<CWideString*>` (less<CWideString>) TU: it calls the landed
//   __adjust_heap<CWideString*, int, CWideString, less<CWideString> > 0x009B2EA0 (whose caller list starts with this
//   body's 0x009B3A6C); no direct callers (an emitted COMDAT). The catalog's "swap<CTCBase::CRegisteredTCInfo>" is
//   propagated from a byte twin.
// Callees: CWideString::operator= 0x0099B7D0, CWideString copy ctor 0x0099B720 (val rebuilt in the argument area),
//   __adjust_heap 0x009B2EA0 (hole 0 in edx), ~CWideString 0x0099B510.
// Shape: the STLport body verbatim (byte twin of the CCaseInsensitiveStringCompare instantiation 0x00497310).
class CWideString {
public:
    void* PStringData;                                  // +0x00
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    bool operator<(const CWideString& other) const;     // 0x0099AF70
};

namespace _STL {
template <class _Tp> struct less {                      // empty, passed by value (one stack word)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
                              _Distance __len, _Tp __val, _Compare __comp);   // 0x009B2EA0

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp,
                                  _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap<CWideString*, CWideString, less<CWideString>, int>(
    CWideString*, CWideString*, CWideString*, CWideString, less<CWideString>, int*);
}