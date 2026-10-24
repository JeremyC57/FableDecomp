// _STL::__final_insertion_sort(unsigned long* first, unsigned long* last, CBankFile::CompareDataOffset comp) -- STLport introsort tail over a bank's entry indices: more than 16 elements -> insertion-sort the first 16, then unguarded linear inserts for the rest; otherwise one insertion sort
// Leaf: __final_insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D3ED0, 75 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// one-pointer comparator by value on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callers: 0x009D06E4 and 0x009D498A (the CBankFile entry sort by data offset).
// Callees: 0x009D3B90 __insertion_sort (landed STL_insertionsort_009d3b90) and 0x009D2FC0 __unguarded_linear_insert
// (landed global_unguardedlinearinsert_009d2fc0), both instantiated for CBankFile::CompareDataOffset.
// Identity: catalogued as "CBankFile_IntroSort" (descriptive); the shape is STLport's __final_insertion_sort with
// __stl_threshold = 16 and __unguarded_insertion_sort(_aux) inlined (the `_Tp*` value-type tag vanishes).
// Shape: the element count test `last - first > 16` is strength-reduced to the byte difference `and eax,-4 ; cmp eax,40h`;
// the loop is a for (test before the first pass); the comparator is re-pushed from its own argument slot.
// Layout: the comparator holds the bank (retail CBankFile model of the landed siblings; only its pointer is passed here).
struct CBankFile_Retail;

struct CompareDataOffset {                         // CBankFile::CompareDataOffset: holds the bank
    const CBankFile_Retail* m_pBank;
};

void __fastcall __insertion_sort(unsigned long* first, unsigned long* last, CompareDataOffset comp);                // 0x009D3B90
void __fastcall __unguarded_linear_insert(unsigned long* last, unsigned long value, CompareDataOffset comp);   // 0x009D2FC0

static const int __stl_threshold = 16;

inline void __unguarded_insertion_sort_aux(unsigned long* first, unsigned long* last, unsigned long*, CompareDataOffset comp)
{
    for (unsigned long* i = first; i != last; ++i)
        __unguarded_linear_insert(i, *i, comp);
}

inline void __unguarded_insertion_sort(unsigned long* first, unsigned long* last, CompareDataOffset comp)
{
    __unguarded_insertion_sort_aux(first, last, (unsigned long*)0, comp);
}

void __fastcall __final_insertion_sort(unsigned long* first, unsigned long* last, CompareDataOffset comp)
{
    if (last - first > __stl_threshold) {
        __insertion_sort(first, first + __stl_threshold, comp);
        __unguarded_insertion_sort(first + __stl_threshold, last, comp);
    }
    else
        __insertion_sort(first, last, comp);
}