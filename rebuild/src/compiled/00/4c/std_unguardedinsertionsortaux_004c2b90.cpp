// std::__unguarded_insertion_sort_aux(NGraphicAppearance::CMeshInfo* first, NGraphicAppearance::CMeshInfo* last, NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesFirst comp) -- STLport: unguarded linear insert of every element of [first, last)
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2B90, length 105 bytes, `ret 8` (__fastcall /Gr algorithm TU: first in
// ecx, last in edx, the value-type tag pointer and the 4-byte comparator on the stack).
// Callers: the two __final_insertion_sort<CMeshInfo*, CSortPrimaryMeshesFirst> tails (0x004C38E7, 0x004C5755).
// Label: the catalog "_Copy_impl<CNavigationPosition*,CNavigationPosition*>" is propagated; the body is STLport 4.x
// `for (i = first; i != last; ++i) __unguarded_linear_insert(i, _Tp(*i), comp);` with the insert inlined (its out-of-line
// copy for this comparator is 0x004C1EC0, this chunk).
// Element (Ego_r.pdb): NGraphicAppearance::CMeshInfo { long Index; float SizeX; uchar PrimaryMesh:1; } -- 12-byte POD.
// Comparator: CSortPrimaryMeshesFirst reads only its left operand (`a.PrimaryMesh`): a primary value enters the
// unconditional (exit-less) backward copy loop, any other value is stored back into its own slot.
// Shape: last is spilled to the frame (`mov [esp+4],edx`), the hoisted predicate bit to a dead frame byte.
struct CMeshInfo {                        // NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes, POD)
    long Index;                           // +0x00
    float SizeX;                          // +0x04
    unsigned char PrimaryMesh : 1;        // +0x08 bit 0
};

struct CSortPrimaryMeshesFirst {          // 4-byte comparator slot passed by value
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

inline void __unguarded_linear_insert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)   // out of line: 0x004C1EC0
{
    CMeshInfo* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}

void __fastcall __unguarded_insertion_sort_aux(CMeshInfo* first, CMeshInfo* last, CMeshInfo*, CSortPrimaryMeshesFirst comp)
{
    for (CMeshInfo* i = first; i != last; ++i)
        __unguarded_linear_insert(i, CMeshInfo(*i), comp);
}