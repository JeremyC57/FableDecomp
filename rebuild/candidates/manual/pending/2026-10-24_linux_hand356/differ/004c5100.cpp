// _STL::__insertion_sort(CMeshInfo* first, CMeshInfo* last, CSortPrimaryMeshesFirst comp) -- STLport insertion sort over NGraphicAppearance::CMeshInfo with __linear_insert inlined: a primary mesh info shifts the run up one slot (out-of-line __copy_backward) and goes first, a non-primary one goes through the out-of-line __unguarded_linear_insert
// Leaf: __insertion_sort
// Grade: DIFFER(135v135) -- only the copy_backward tag temporary: ours gives it its own frame dword (`sub esp,0Ch`, `lea edx,[esp+17h]`), retail homes it over the live comparator slot (`sub esp,8`, `lea edx,[esp+20h]`); next: define the real __copy_backward body (0x004C2070) out of line in the TU (`#pragma auto_inline(off)` + `__declspec(noinline)`), the fix that matched the sibling 004c5190
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C5100, 135 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callees: 0x004C2070 __copy_backward(first, last, result, const random_access_iterator_tag&, ptrdiff_t*) (landed as
// global_CopyBackwardSElement12004c2070), 0x004C1EC0 __unguarded_linear_insert(last, value, comp) (`ret 10h`).
// Callers: the landed __final_insertion_sort 0x004C5720 (twice). Sibling: 0x004C5190 (CSortPrimaryMeshesLast).
// Element (Ego_r.pdb): NGraphicAppearance::CMeshInfo { long Index; float SizeX; uchar PrimaryMesh:1; } -- 12-byte POD.
// Comparator: CSortPrimaryMeshesFirst tests only the left operand's bit (`test bl,1`), i.e. `a.PrimaryMesh`.
// Shape: the by-value `*i` is a POD block copy held in edi/ebp/ebx (`mov eax,esi` first); `*first = val` is three dword
// stores; tried: plain inline / __forceinline __linear_insert, a by-value tag (fixes the frame size but pushes a
// register instead of the slot address), an iterator_category() + __false_type chain, and STLport's ctor-less tag
// hierarchy -- all keep the tag in its own dword while __copy_backward is only declared.
struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct CMeshInfo {                        // NGraphicAppearance::CMeshInfo (Ego_r.pdb, 12 bytes, POD)
    long Index;                           // +0x00
    float SizeX;                          // +0x04
    unsigned char PrimaryMesh : 1;        // +0x08 bit 0
};

struct CSortPrimaryMeshesFirst {
    void* m_context;
    bool operator()(const CMeshInfo& a, const CMeshInfo& b) const { return a.PrimaryMesh; }
};

CMeshInfo* __fastcall __copy_backward(CMeshInfo* first, CMeshInfo* last, CMeshInfo* result,
                                      const random_access_iterator_tag& tag, int* distance);   // 0x004C2070
void __fastcall __unguarded_linear_insert(CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp);   // 0x004C1EC0

__forceinline CMeshInfo* copy_backward(CMeshInfo* first, CMeshInfo* last, CMeshInfo* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

__forceinline void __linear_insert(CMeshInfo* first, CMeshInfo* last, CMeshInfo value, CSortPrimaryMeshesFirst comp)
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
