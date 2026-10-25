// std::__inplace_stable_sort(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, less<CSortElement> comp) -- stable sort without a buffer: insertion sort below 15 elements, else sort both halves recursively and merge them in place
// Leaf: ??$__inplace_stable_sort@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B86570, 99 bytes. __fastcall (first ecx, last edx), `ret 4` (the empty
//   comparator on the stack).
// Callees: std::__insertion_sort<...> @ 0x00B85B30, itself, std::__merge_without_buffer<...> @ 0x00B86140.
// Callers: __stable_sort_aux 0x00B86710 (when no temporary buffer could be allocated) and itself.
// Identity: the manifest label "std_introsort_recursive" is descriptive and wrong (no introsort); STLport
//   `__inplace_stable_sort`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace std (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `__inplace_stable_sort` verbatim (the landed stand-in of 0x00711B80): `(__last - __first) / 2` as
//   cdq ; sub ; sar, the two recursive calls, then __merge_without_buffer with both lengths recomputed.
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

namespace std {
template <class _Tp> struct less {                      // empty functor (a 4-byte argument slot)
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
}

namespace std {
template <class _RandomAccessIter, class _Compare>
void __fastcall __insertion_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp);   // 0x00B85B30
template <class _BidirectionalIter, class _Distance, class _Compare>
void __fastcall __merge_without_buffer(_BidirectionalIter __first, _BidirectionalIter __middle, _BidirectionalIter __last,
                                       _Distance __len1, _Distance __len2, _Compare __comp);         // 0x00B86140

template <class _RandomAccessIter, class _Compare>
void __fastcall __inplace_stable_sort(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp)
{
    if (__last - __first < 15) {
        __insertion_sort(__first, __last, __comp);
        return;
    }
    _RandomAccessIter __middle = __first + (__last - __first) / 2;
    __inplace_stable_sort(__first, __middle, __comp);
    __inplace_stable_sort(__middle, __last, __comp);
    __merge_without_buffer(__first, __middle, __last, __middle - __first, __last - __middle, __comp);
}

template void __fastcall __inplace_stable_sort<CSortElement*, less<CSortElement> >(CSortElement*, CSortElement*,
    less<CSortElement>);
}