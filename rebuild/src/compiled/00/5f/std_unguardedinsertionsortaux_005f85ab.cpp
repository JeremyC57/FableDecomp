// std::__unguarded_insertion_sort_aux(SKeyEntryA* first, SKeyEntryA* last, SKeyEntryA*, SKeyEntryALess comp) -- unguarded insertion sort of 8-byte {long key, word} pairs ordered by key
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005F85AB, length 45 bytes (unaligned entry: a size-optimised TU).
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<std::pair<unsigned long, CAnimationEntry>>>" is
// propagated (CAnimationEntry is 16 bytes; these elements are 8). Its caller 0x005FA068 is STLport
// `__final_insertion_sort` (`and eax,-8 ; cmp eax,80h` = 16 elements of 8 bytes; it pushes the null value-type pointer
// `push 0` and the comparator), so this is `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)`: for every element
// `__unguarded_linear_insert(i, _Tp(*i), comp)` @ 0x005F7CD0 (it shifts while `val.first < prev.first`, signed, `ret 0Ch`).
// Byte-identical twin of 0x005F85F1 (another instantiation of the same template); the element and comparator classes
// are not identified, so they are modelled as a pair<long, long> and an empty key comparator.
// Convention: LTCG register form -- first in ecx, last in edx; the value-type pointer and the 4-byte empty comparator on
// the stack (`ret 8`).
// Callees: std::__unguarded_linear_insert @ 0x005F7CD0.
// Pragma: optimize("s") -- the loop is entered by a jump to its bottom test, the comparator is pushed straight from
// memory and the 8-byte argument slot is made by `push ecx ; push ecx`; the STLport pair's memberwise user copy
// constructor builds the value in that slot through eax.
#pragma optimize("s", on)
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}
typedef std::pair<long, long> SKeyEntryA;
struct SKeyEntryALess { };              // empty comparator: orders by `first`
namespace std {
void __fastcall __unguarded_linear_insert(SKeyEntryA* last, SKeyEntryA val, SKeyEntryALess comp);   // 0x005F7CD0
void __fastcall __unguarded_insertion_sort_aux(SKeyEntryA* first, SKeyEntryA* last, SKeyEntryA*, SKeyEntryALess comp)
{
    for (SKeyEntryA* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SKeyEntryA(*i), comp);
}
}