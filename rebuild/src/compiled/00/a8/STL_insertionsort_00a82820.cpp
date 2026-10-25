// std::__insertion_sort(SKeyPairU64Bool* first, SKeyPairU64Bool* last, CKeyPairCompareLess comp) -- STLport insertion sort over 16-byte pair<unsigned __int64, bool> records: every element after the first goes through the out-of-line __linear_insert(first, i, *i, comp)
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82820, 68 bytes, `ret 4` (__fastcall: first in ecx, last in edx, the
// 4-byte comparator on the stack -- the SGI/STLport algorithm TUs are /Gr).
// Callee: 0x00A81C50 std::__linear_insert(first, last, value, comp) (manifest "InsertionSort_Callback_PairLong" is
// propagated; only this body calls it). That callee orders on a 64-bit unsigned key (`cmp hi ; ja/jb ; cmp lo ; jae`)
// and re-copies the byte at +8 with `mov cl,[..] ; mov [eax+8],cl`, so the element is pair<unsigned __int64, bool>.
// Label: catalogued as `_Insertion_sort1<pair<long,long>*, CKeyPairCompareLess<long,long> >` (propagated); the code is
// STLport's `__insertion_sort`.
// Shape: STLport's pair has an explicit memberwise copy constructor, so the by-value `*i` is built in the argument area
// (`sub esp,10h ; mov eax,esp`): two dword moves for the key, one byte move for the flag, no padding copied.
struct SKeyPairU64Bool {                  // std::pair<unsigned __int64, bool>, 16 bytes (8-byte alignment)
    unsigned __int64 first;               // +0x00 sort key
    bool second;                          // +0x08
    SKeyPairU64Bool(const SKeyPairU64Bool& o) : first(o.first), second(o.second) { }
};

struct CKeyPairCompareLess {              // 4-byte comparator functor passed by value
    void* m_context;
};

void __fastcall __linear_insert(SKeyPairU64Bool* first, SKeyPairU64Bool* last, SKeyPairU64Bool value, CKeyPairCompareLess comp);   // 0x00A81C50

void __fastcall __insertion_sort(SKeyPairU64Bool* first, SKeyPairU64Bool* last, CKeyPairCompareLess comp)
{
    if (first == last)
        return;
    for (SKeyPairU64Bool* i = first + 1; i != last; ++i)
        __linear_insert(first, i, *i, comp);
}