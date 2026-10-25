// _STL::__pop_heap(CCharString* first, CCharString* last, CCharString* result, CCharString val, less<CCharString> comp, int*) -- STLport heap pop step of a CCharString sort: move the top string to *result, then sift val down from the root
// Leaf: ??$__pop_heap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E1B20, 63 bytes (+ int3), /Gr `__fastcall` (first in ecx -> edi, last in edx
//   -> esi; result, the 4-byte string by value, the comparator word and the distance pointer on the stack, `ret 10h`).
// Identity: STLport 4.6 `__pop_heap` of the `sort<CCharString*>` (less<CCharString>) instantiation of the animation TU:
//   it calls the landed __adjust_heap<CCharString*, int, CCharString, less<CCharString> > 0x005E1170; no direct callers
//   (an emitted COMDAT). The catalog's "swap<CTCBase::CRegisteredTCInfo>" is propagated from a byte twin.
// Callees: CCharString::operator= 0x0099EFB0, CCharString copy ctor 0x0099EC30 (val rebuilt in the argument area),
//   __adjust_heap 0x005E1170 (hole 0 in edx), ~CCharString 0x0099EAE0.
// Shape: the STLport body verbatim (byte twin of the CWideString instantiations 0x00497310 / 0x009B3A40).
class CCharString {
public:
    void* PStringData;                                  // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const;
};

namespace _STL {
template <class _Tp> struct less {                      // empty, passed by value (one stack word)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex,
                              _Distance __len, _Tp __val, _Compare __comp);   // 0x005E1170

template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __fastcall __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last,
                                  _RandomAccessIterator __result, _Tp __val, _Compare __comp,
                                  _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template void __fastcall __pop_heap<CCharString*, CCharString, less<CCharString>, int>(
    CCharString*, CCharString*, CCharString*, CCharString, less<CCharString>, int*);
}