// _STL::__insertion_sort(CMeshInfo* first, CMeshInfo* last, CSortPrimaryMeshesFirst comp) -- STLport insertion sort over NGraphicAppearance::CMeshInfo with __linear_insert inlined: a primary mesh info shifts the run up one slot (out-of-line __copy_backward) and goes first, a non-primary one goes through the out-of-line __unguarded_linear_insert
// Leaf: ?__insertion_sort@@YIXPAUCMeshInfo@@0UCSortPrimaryMeshesFirst@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5100, 135 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x004C2070 __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) (landed as
// global_CopyBackwardSElement12004c2070), 0x004C1EC0 __unguarded_linear_insert(last, value, comp) (`ret 10h`).
// Callers: the landed __final_insertion_sort 0x004C5720 (twice). Sibling: 0x004C5190 (CSortPrimaryMeshesLast, landed; this
// row is its recipe with the comparator's sense flipped -- retry pass agent901).
// Element (Ego_r.pdb): NGraphicAppearance::CMeshInfo { long Index; float SizeX; uchar PrimaryMesh:1; } -- 12-byte POD.
// Comparator: CSortPrimaryMeshesFirst tests only the left operand's bit (`test bl,1`), i.e. `a.PrimaryMesh`; the
// out-of-line unguarded insert tests the same single bit.
// Label: catalogued as `_Insertion_sort1<CMeshInfo*, CSortPrimaryMeshesFirst>` (Dinkumware name); the code is STLport's
// `__insertion_sort` with `__linear_insert(first, i, *i, comp)` inlined.
// Shape: the by-value `*i` is a POD block copy held in edi/ebp/ebx (`mov eax,esi` first); `*first = val` is three dword
// stores; copy_backward's empty iterator tag is a const-reference temporary that VC7.1 homes over the LIVE comparator
// slot (`lea eax,[esp+20h]`) -- only when the real __copy_backward body is defined out of line in the TU (GOTCHAS
// "empty tag over a LIVE argument slot"; an extern declaration gives the tag its own frame dword).
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct CMeshInfo {                        // NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes, POD)
    long Index;                           // +0x00
    float SizeX;                          // +0x04
    unsigned char PrimaryMesh : 1;        // +0x08 bit 0
};

struct CSortPrimaryMeshesFirst {           // 4-byte comparator slot passed by value
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

// The real __copy_backward body (0x004C2070), out of line in the TU.
#pragma auto_inline(off)
__declspec(noinline) CMeshInfo* __fastcall __copy_backward(CMeshInfo* first, CMeshInfo* last, CMeshInfo* result,
                                                           const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}
#pragma auto_inline(on)

void __fastcall __unguarded_linear_insert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp);   // 0x004C1EC0

inline CMeshInfo* copy_backward(CMeshInfo* first, CMeshInfo* last, CMeshInfo* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

inline void __linear_insert(CMeshInfo* first, CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}

void __fastcall __insertion_sort(CMeshInfo* first, CMeshInfo* last, CSortPrimaryMeshesFirst comp)
{
    if (first == last)
        return;
    for (CMeshInfo* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}