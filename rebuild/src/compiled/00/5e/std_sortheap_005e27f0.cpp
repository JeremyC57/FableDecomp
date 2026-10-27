// std::sort_heap(CCharString* first, CCharString* last, std::less<CCharString> comp) -- (<CCharString*, std::less<CCharString>>) turn the heap [first, last) into a sorted range
// Leaf: ??$sort_heap@PAVCCharString@@U?$less@VCCharString@@@std@@@std@@YIXPAVCCharString@@0U?$less@VCCharString@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E27F0, 112 bytes, `__fastcall` (first ecx, last edx, the empty comparator's slot on the stack), `ret 4`.
// Identity (proven): its only caller is the partial_sort 0x005E2EB0 of this TU; it calls 0x005E1170, bound ??$__adjust_heap@PAVCCharString@@HV1@U?$less@VCCharString@@@std@@@std@@, and CCharString's copy/assign/destructor.
// Callees: CCharString copy constructor 0x0099EC30, operator= 0x0099EFB0, destructor 0x0099EAE0; std::__adjust_heap<CCharString*, int,
//   CCharString, std::less<CCharString>> 0x005E1170 (__fastcall: first ecx, hole index edx, then len, the value and the comparator by value).
// Shape (family M073, representative 0x00497570): STLport 4.6 _heap.h `while (__last - __first > 1) pop_heap(__first, __last--,
//   __comp);` with pop_heap -> __pop_heap_aux -> __pop_heap inline (`_Tp(*(__last - 1))` in the dead comparator slot,
//   `*__result = *__first`, `__adjust_heap(__first, 0, __last - __first, __val, __comp)`); speed TU.
class CCharString {                                       // 4 bytes (PStringData)
public:
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
    bool operator<(const CCharString& other) const;
    void* PStringData;
};
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
}
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x005E1170
template <class _RandomAccessIterator, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _RandomAccessIterator __result, _Tp __val, _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIterator, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
template <class _RandomAccessIterator, class _Compare>
inline void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, (CCharString*)0, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<CCharString*, std::less<CCharString> >(CCharString*, CCharString*, std::less<CCharString>);
}