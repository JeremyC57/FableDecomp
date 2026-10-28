// std::sort_heap(CWideString* first, CWideString* last, std::less<CWideString> comp) -- (<CWideString*, std::less<CWideString>>) turn the heap [first, last) into a sorted range
// Leaf: ??$sort_heap@PAVCWideString@@U?$less@VCWideString@@@std@@@std@@YIXPAVCWideString@@0U?$less@VCWideString@@@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B5A80, 112 bytes, `__fastcall` (first ecx, last edx, the empty comparator's slot on the stack), `ret 4`.
// Identity (proven): its only caller is the partial_sort 0x009B6A40 of this TU; it calls 0x009B2EA0, bound ??$__adjust_heap@PAVCWideString@@HV1@U?$less@VCWideString@@@std@@@std@@ (landed), and CWideString's copy/assign/destructor.
// Callees: CWideString copy constructor 0x0099B720, operator= 0x0099B7D0, destructor 0x0099B510; std::__adjust_heap<CWideString*, int,
//   CWideString, std::less<CWideString>> 0x009B2EA0 (__fastcall: first ecx, hole index edx, then len, the value and the comparator by value).
// Shape (family M073, representative 0x00497570): STLport 4.6 _heap.h `while (__last - __first > 1) pop_heap(__first, __last--,
//   __comp);` with pop_heap -> __pop_heap_aux -> __pop_heap inline (`_Tp(*(__last - 1))` in the dead comparator slot,
//   `*__result = *__first`, `__adjust_heap(__first, 0, __last - __first, __val, __comp)`); speed TU.
class CWideString {                                       // 4 bytes (PStringData)
public:
    CWideString(const CWideString& other);                // 0x0099B720
    ~CWideString();                                       // 0x0099B510
    const CWideString& operator=(const CWideString& other);     // 0x0099B7D0
    bool operator<(const CWideString& other) const;
    void* PStringData;
};
namespace std {
template <class _Tp> struct less { bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; } };
}
namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val, _Compare __comp);   // 0x009B2EA0
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
    __pop_heap_aux(__first, __last, (CWideString*)0, __comp);
}
template <class _RandomAccessIterator, class _Compare>
void __fastcall sort_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}
template void __fastcall sort_heap<CWideString*, std::less<CWideString> >(CWideString*, CWideString*, std::less<CWideString>);
}