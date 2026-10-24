// _STL::sort_heap(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, less<CSortElement> comp) -- sort a heap of sort elements by popping its top to the back until one element is left
// Leaf: ??$sort_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B85E10, 92 bytes. __fastcall (first ecx, last edx), `ret 4` (the empty
//   comparator on the stack).
// Callees: _STL::__adjust_heap<CSortElement*, int, CSortElement, less<CSortElement> > @ 0x00B852F0.
// Callers: __partial_sort 0x00B86450 (call 0x00B864A7).
// Identity: the manifest label "_Sort_heap<long*>" is a propagated fold label; STLport `sort_heap` (Ego_r.pdb has the
//   Dinkumware twin `_Sort_heap<CSortElement*>`).
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace _STL (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `while (last - first > 1) pop_heap(first, last--, comp);` with pop_heap / __pop_heap_aux / __pop_heap
//   inline: `*(last - 1) = *first` after the by-value copy of *(last - 1), then __adjust_heap(first, 0, last - 1 - first,
//   val, comp). VC7.1 tests `last - first > 1` on the byte difference (`and ecx,-8 ; cmp ecx,8`).
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

template <class _RandomAccessIter, class _Tp, class _Compare, class _Distance>
inline void __pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _RandomAccessIter __result, _Tp __val,
                       _Compare __comp, _Distance*)
{
    *__result = *__first;
    __adjust_heap(__first, _Distance(0), _Distance(__last - __first), __val, __comp);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
inline void __pop_heap_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    __pop_heap(__first, __last - 1, __last - 1, _Tp(*(__last - 1)), __comp, (int*)0);
}
template <class _Tp> inline _Tp* __value_type(_Tp* const&) { return (_Tp*)0; }
template <class _RandomAccessIter, class _Compare>
inline void pop_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    __pop_heap_aux(__first, __last, __value_type(__first), __comp);
}

template <class _RandomAccessIter, class _Compare>
void __fastcall sort_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    while (__last - __first > 1)
        pop_heap(__first, __last--, __comp);
}

template void __fastcall sort_heap<CSortElement*, less<CSortElement> >(CSortElement*, CSortElement*, less<CSortElement>);
}