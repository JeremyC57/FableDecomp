// std::__unguarded_insertion_sort_aux(std::pair<CCharString, CSymbolMap::CSymbolInfo>* first, std::pair<CCharString, CSymbolMap::CSymbolInfo>* last, std::pair<CCharString, CSymbolMap::CSymbolInfo>*, SSymbolEntryLess comp) -- unguarded insertion sort of symbol-map entries
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E6D10, length 83 bytes.
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is SGI/STLport
// `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)` calling `__unguarded_linear_insert(i, _Tp(*i), comp)`
// @ 0x009E66F0 (`ret 14h` = 16-byte value + comparator). Element: pair<CCharString, CSymbolMap::CSymbolInfo> (its
// caller 0x009E6F77 is labelled _Adjust_heap<pair<CCharString, CSymbolMap::CSymbolInfo>*>; Ego_r.pdb CSymbolInfo =
// GroupName +0, EntryName +4, Val +8 -> a 16-byte pair).
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the 4-byte empty
// comparator on the stack (`ret 8`).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, std::__unguarded_linear_insert @ 0x009E66F0.
// Shape: the value is built in the argument slot by the STLport memberwise pair copy constructor, CSymbolInfo's
// implicit copy constructor at `edi+4` (the Val dword copied last, after the source pointer reload); the comparator is
// loaded once into ebp before the loop.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
};
struct CSymbolMap {
    struct CSymbolInfo {                    // Ego_r.pdb, 12 bytes
        CCharString GroupName;              // +0x00
        CCharString EntryName;              // +0x04
        long        Val;                    // +0x08
    };
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}
typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> SSymbolEntry;
struct SSymbolEntryLess { };
namespace std {
void __fastcall __unguarded_linear_insert(SSymbolEntry* last, SSymbolEntry val, SSymbolEntryLess comp);   // 0x009E66F0
void __fastcall __unguarded_insertion_sort_aux(SSymbolEntry* first, SSymbolEntry* last, SSymbolEntry*, SSymbolEntryLess comp)
{
    for (SSymbolEntry* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SSymbolEntry(*i), comp);
}
}