// _STL::__partial_sort(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* middle, CEngineSubPrimitiveRenderer::CSortElement* last, CSortElement*, less<CSortElement> comp) -- heap-select the smallest sort elements into [first, middle) and sort them
// Leaf: ??$__partial_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86450, 99 bytes. __fastcall (first ecx, middle edx), `ret 0Ch` (last,
//   the value-type tag pointer and the empty comparator on the stack).
// Callees: _STL::__make_heap<...> @ 0x00B855F0 (first, middle, comp, 0, 0), _STL::__adjust_heap<...> @ 0x00B852F0 (the
//   inlined __pop_heap), _STL::sort_heap<...> @ 0x00B85E10.
// Callers: the introsort loop 0x00B867A0 (call 0x00B86848, its depth-limit fallback) and 0x00B8668C.
// Identity: the manifest label "PartialSort" is descriptive; STLport `__partial_sort`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace _STL (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `make_heap(first, middle, comp); for (i = middle; i < last; ++i) if (comp(*i, *first))
//   __pop_heap(first, middle, i, T(*i), comp, DISTANCE_TYPE(first)); sort_heap(first, middle, comp);` with make_heap
//   and __pop_heap inline; last is re-read from its argument slot each pass.
inline void* operator new(unsigned int, void* where) { return where; }
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)

struct __true_type { __true_type() {} };
struct __false_type { __false_type() {} };

class CEngineSubPrimitiveBase;

class CEngineSubPrimitiveRenderer {
public:
    class CSortElement {                                // 8 bytes (Ego_r.pdb / FableWin.pdb)
    public:
        unsigned long SortValue;                        // +0x00
        CEngineSubPrimitiveBase* SubPrimitive;          // +0x04
        CSortElement() {}
        CSortElement(unsigned long sortValue, CEngineSubPrimitiveBase* subPrimitive)
            : SortValue(sortValue), SubPrimitive(subPrimitive) {}
        bool operator<(const CSortElement& other) const { return SortValue < other.SortValue; }
    };
};
typedef CEngineSubPrimitiveRenderer::CSortElement CSortElement;

namespace _STL {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace _STL {
template <class _RandomAccessIter, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIter __first, _Distance __holeIndex, _Distance __len, _Tp __val,
                              _Compare __comp);                                                    // 0x00B852F0
template <class _RandomAccessIter, class _Compare, class _Tp, class _Distance>
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*,
                            _Distance*);                                                           // 0x00B855F0
template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);  // 0x00B85E10

template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _Tp> inline int* __distance_type(_Tp* const&) { return (int*)0; }

template <class _RandomAccessIter, class _Compare>
inline void make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __make_heap(__first, __last, __comp, __value_type(__first), __distance_type(__first));
}
template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val,
                       _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __partial_sort(_RandomAccessIter __first, _RandomAccessIter __middle, _RandomAccessIter __last, _Tp*,
                               _Compare __comp)
{
    make_heap(__first, __middle, __comp);
    for (_RandomAccessIter __i = __middle; __i < __last; ++__i)
        if (__comp(*__i, *__first))
            __pop_heap(__first, __middle, __i, _Tp(*__i), __comp, __distance_type(__first));
    sort_heap(__first, __middle, __comp);
}

template void __fastcall __partial_sort<CSortElement*, CSortElement, less<CSortElement> >(CSortElement*, CSortElement*,
    CSortElement*, CSortElement*, less<CSortElement>);
}