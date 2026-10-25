// std::merge(CSortElement* first1, CSortElement* last1, CSortElement* first2, CSortElement* last2, CSortElement* result, CSortLess comp) -- SGI merge of two sorted runs of CEngineSubPrimitiveRenderer::CSortElement by the unsigned sort value (the second run's element goes first only when strictly smaller), then both tails copied with the counted copy loop
// Leaf: merge
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B85510, 127 bytes, `ret 10h` (__fastcall: first1 in ecx, last1 in edx,
// first2, last2, result and the 4-byte comparator slot on the stack -- the SGI/STLport algorithm TUs are /Gr). No callees.
// Element: Ego_r.pdb CEngineSubPrimitiveRenderer::CSortElement (8 bytes) { ulong SortValue; CEngineSubPrimitiveBase*
// SubPrimitive; }; the comparator orders by SortValue unsigned (`cmp ebx,[ecx] ; jae`). Same element and comparator as
// the matched insertion sort 0x00B85B30 (chunk 356) beside it.
// Shape: `while (first1 != last1 && first2 != last2) { if (comp(*first2, *first1)) *result = *first2++; else *result =
// *first1++; ++result; } return copy(first2, last2, copy(first1, last1, result));` -- copy() of a non-trivial-traits
// element is STLport's `__copy(first, last, result, random_access_iterator_tag, Distance*)` counted loop, both inlined
// (VC7.1 folds source and destination into one `[ecx+eax]` walk). The SubPrimitive word is copied through ebx after
// the branch join.
// Label: catalogued as Dinkumware `_Merge<CSortElement*, CSortElement*, CSortElement*>`.
struct CEngineSubPrimitiveBase;

struct CSortElement {                     // CEngineSubPrimitiveRenderer::CSortElement (PDB, 8 bytes)
    unsigned long SortValue;              // +0x00
    CEngineSubPrimitiveBase* SubPrimitive;// +0x04
};

struct CSortLess {                        // comparator functor passed in a 4-byte slot
    bool operator()(const CSortElement& a, const CSortElement& b) const { return a.SortValue < b.SortValue; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

inline CSortElement* __copy(CSortElement* first, CSortElement* last, CSortElement* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

inline CSortElement* copy(CSortElement* first, CSortElement* last, CSortElement* result)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

CSortElement* __fastcall merge(CSortElement* first1, CSortElement* last1, CSortElement* first2, CSortElement* last2,
                               CSortElement* result, CSortLess comp)
{
    while (first1 != last1 && first2 != last2) {
        if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}