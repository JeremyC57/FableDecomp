// _STL::__make_heap(CEngineSubPrimitiveRenderer::CSortElement* first, CEngineSubPrimitiveRenderer::CSortElement* last, less<CSortElement> comp, CSortElement*, int*) -- heapify the sort elements: sift every parent down from the last one to the root
// Leaf: ??$__make_heap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B855F0, 102 bytes (+ int3 padding). __fastcall (first ecx, last edx),
//   `ret 0Ch` (the empty comparator and the value-type / distance-type tag pointers on the stack).
// Callees: _STL::__adjust_heap<CSortElement*, int, CSortElement, less<CSortElement> > @ 0x00B852F0 (__fastcall, the
//   element by value in two argument dwords).
// Callers: make_heap inside __partial_sort 0x00B86450 (call 0x00B86461).
// Identity: the manifest label "MakeHeap" is descriptive; STLport `__make_heap`.
//   The element is CEngineSubPrimitiveRenderer::CSortElement (8 bytes: unsigned long SortValue, CEngineSubPrimitiveBase*
//   SubPrimitive; a user default constructor makes it non-POD for STLport's __type_traits) and the comparator is
//   less<CSortElement> (CSortElement::operator< compares SortValue unsigned). This is the STLport sort / stable_sort
//   chain of the CEngineSubPrimitiveRenderer TU (SortList 0x00B84990: stable_sort when the list RequiresStableSort, else
//   sort below 256 elements, else RadixSort 0x00B847D0); Ego_r.pdb / FableWin.pdb carry the Dinkumware twins
//   (_Stable_sort, _Buffered_merge_sort, _Make_heap, _Sort_heap, lower_bound, upper_bound ... over CSortElement*).
//   Modelled in namespace _STL (lane convention; retail RTTI spells std, `pair@...@std@@`); the templates are __fastcall.
// Shape: STLport `if (last - first < 2) return; len = last - first; parent = (len - 2) / 2; for (;;) {
//   __adjust_heap(first, parent, len, T(*(first + parent)), comp); if (parent == 0) return; parent--; }` -- the cursor
//   first + parent is strength-reduced into edi, the by-value element is pushed as two dwords (implicit copy ctor).
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
void __fastcall __make_heap(_RandomAccessIter __first, _RandomAccessIter __last, _Compare __comp, _Tp*, _Distance*)
{
    if (__last - __first < 2)
        return;
    _Distance __len = __last - __first;
    _Distance __parent = (__len - 2) / 2;

    for (;;) {
        __adjust_heap(__first, __parent, __len, _Tp(*(__first + __parent)), __comp);
        if (__parent == 0)
            return;
        __parent--;
    }
}

template void __fastcall __make_heap<CSortElement*, less<CSortElement>, CSortElement, int>(CSortElement*, CSortElement*,
    less<CSortElement>, CSortElement*, int*);
}