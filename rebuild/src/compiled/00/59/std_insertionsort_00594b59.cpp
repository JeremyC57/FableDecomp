// std::__insertion_sort(std::pair<long, CDefPointer<const CHeroAbilityDef> >* first, std::pair<long, CDefPointer<const CHeroAbilityDef> >* last, SAbilityEntryLess comp) -- guarded insertion sort of {id, hero-ability def pointer} pairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594B59, length 52 bytes (unaligned entry: a size-optimised TU).
// Leaf: __insertion_sort
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. Its callers 0x00594C51 /
// 0x00594C6E sit in the `__final_insertion_sort` of the same TU, so this is STLport `__insertion_sort(first, last,
// comp)`: return on an empty range, then `__linear_insert(first, i, _Tp(*i), comp)` @ 0x00594946 for i = first+1 ..
// last (`ret 0Ch` = 8-byte value + comparator). The value's copy constructor is out of line @ 0x00592DFB (manifest:
// `_Pair_base<long, CDefPointer<CHeroAbilityDef const> >` copy constructor -- it copies the key and the def pointer and
// adds a reference at the def's +4), which names the element.
// Convention: LTCG register form -- first in ecx, last in edx, the 4-byte empty comparator on the stack (`ret 4`).
// Callees: pair copy constructor @ 0x00592DFB, std::__linear_insert @ 0x00594946.
// Pragma: optimize("s") -- `push dword ptr [esp+10h]`, `push ecx ; push ecx` for the 8-byte slot and the loop entered
// by a jump to its bottom test.
#pragma optimize("s", on)
struct CHeroAbilityDef;
template <class T> struct CDefPointer {
    T* m_pDef;                              // +0x00 (the def's reference count sits at its +4)
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other);                // 0x00592DFB (out of line in this TU)
};
}
typedef std::pair<long, CDefPointer<const CHeroAbilityDef> > SAbilityEntry;
struct SAbilityEntryLess { };               // empty comparator
namespace std {
void __fastcall __linear_insert(SAbilityEntry* first, SAbilityEntry* last, SAbilityEntry val, SAbilityEntryLess comp);   // 0x00594946
void __fastcall __insertion_sort(SAbilityEntry* first, SAbilityEntry* last, SAbilityEntryLess comp)
{
    if (first == last)
        return;
    for (SAbilityEntry* i = first + 1; i != last; ++i)
        __linear_insert(first, i, SAbilityEntry(*i), comp);
}
}