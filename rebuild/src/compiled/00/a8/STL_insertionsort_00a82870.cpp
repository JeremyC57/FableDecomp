// std::__insertion_sort(SKeyPairU64Long* first, SKeyPairU64Long* last, CKeyPairCompareLess comp) -- STLport insertion sort over 16-byte pair<unsigned __int64, long> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82870, 68 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00A81CC0 std::__linear_insert(first, last, value, comp) (manifest "InsertionSort_Callback_MeshInfo" is
// propagated; only this body calls it). That callee orders on a 64-bit unsigned key (`cmp hi ; ja/jb ; cmp lo ; jae`)
// and assigns `*first = value` as four dwords, so the element is a 16-byte, 8-aligned pair<unsigned __int64, long>.
// Label: catalogued as `_Insertion_sort1<NGraphicAppearance::CMeshInfo*, CSortPrimaryMeshesFirst>` (propagated); the
// code is STLport's `__insertion_sort`.
// Shape: STLport's pair has an explicit memberwise copy constructor, so the by-value `*i` is built in the argument area
// (`sub esp,10h ; mov eax,esp`) with three dword moves -- the 4 padding bytes after `second` are not copied.
struct SKeyPairU64Long {                  // std::pair<unsigned __int64, long>, 16 bytes (8-byte alignment)
    unsigned __int64 first;               // +0x00 sort key
    long second;                          // +0x08
    SKeyPairU64Long(const SKeyPairU64Long& o) : first(o.first), second(o.second) { }
};

struct CKeyPairCompareLess {              // 4-byte comparator functor passed by value
    void* m_context;
};

void __fastcall __linear_insert(SKeyPairU64Long* first, SKeyPairU64Long* last, SKeyPairU64Long value, CKeyPairCompareLess comp);   // 0x00A81CC0

void __fastcall __insertion_sort(SKeyPairU64Long* first, SKeyPairU64Long* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyPairU64Long* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}