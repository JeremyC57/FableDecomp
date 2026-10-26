// std::__insertion_sort(CSortElement* first, CSortElement* last, CSortLess comp) -- STLport insertion sort over CEngineSubPrimitiveRenderer::CSortElement (unsigned sort value, sub-primitive pointer): every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B85B30, 54 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00B85870 std::__linear_insert(first, last, value, comp) (manifest "Insertion_HeapShift" is propagated; its
// only caller is this body; it compares `value.SortValue < first->SortValue` unsigned, `cmp ; jae`).
// Label: catalogued as `_Insertion_sort1<pair<long,long>*, CKeyPairCompareLess<long,long> >` (propagated over the
// byte-identical 0x00AD5B50); the element is the PDB's 8-byte CEngineSubPrimitiveRenderer::CSortElement { ulong
// SortValue; CEngineSubPrimitiveBase* SubPrimitive; } -- its neighbour 0x00B85510 merges the same elements.
// Shape: the element is a POD of two dwords, so the by-value copy is two plain pushes (`push [i+4] ; push [i]`), the
// comparator dword is loaded once into ebp and pushed each iteration. The callee-saved pushes are shrink-wrapped behind
// the two early exits (VC7.1 RTM does this on its own for this loop).
class CEngineSubPrimitiveBase;

struct CSortElement {                     // CEngineSubPrimitiveRenderer::CSortElement (PDB, 8 bytes)
    unsigned long SortValue;              // +0x00
    CEngineSubPrimitiveBase* SubPrimitive;// +0x04
};

struct CSortLess {                        // 4-byte comparator functor passed by value
    void* m_context;
};

void __fastcall __linear_insert(CSortElement* first, CSortElement* last, CSortElement value, CSortLess comp);   // 0x00B85870

void __fastcall __insertion_sort(CSortElement* first, CSortElement* last, CSortLess comp)
{
    if (first == last)
        return;
    for (CSortElement* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}