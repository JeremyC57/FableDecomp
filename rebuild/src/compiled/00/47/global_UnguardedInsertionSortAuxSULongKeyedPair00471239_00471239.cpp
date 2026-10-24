// _global::UnguardedInsertionSortAux_SULongKeyedPair_00471239(SULongKeyedPair* first, SULongKeyedPair* last, SULongKeyedPair*, SULongKeyPairLess comp) -- SGI __unguarded_insertion_sort_aux over 8-byte {unsigned long key, word} pairs: sink every element below its larger predecessors
// Leaf: UnguardedInsertionSortAux_SULongKeyedPair_00471239
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00471239, length 65 bytes, `ret 8`; size-optimised TU (frame pointer + `leave`).
// Caller: the __final_insertion_sort 0x004711F5 (`lea esi,[ecx+80h]` = first + 16 elements ; `push comp ; push 0 ; call`).
// Shape: SGI `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` with the step inlined:
// `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;` -- the key in esi,
// the word in ebx (read as `[next+0Ch]` off the freshly formed `next = i - 1`), the compare `cmp esi,[edi] ; jb`
// (unsigned key), `last` kept in the 4-byte frame word. The element keeps STLport pair's memberwise copy constructor
// (as the float-keyed twin 0x004D03C0). Frame pointer with esi/edi free: optimize("y", off) + ("s", on); under "s" the
// inline step stays a call (the by-value pair pushed through `mov eax,esp`), so it is __forceinline.
// Label correction: the catalog's `_Copy_impl<std::pair<EHeroMorphType,CParticleMorphs::CEntry>*,...>` is propagated (an
// enum key would compare signed); descriptive element names.
#pragma optimize("y", off)
#pragma optimize("s", on)

struct SULongKeyedPair {                         // 8 bytes
    unsigned long key;                           // +0x00
    long          value;                         // +0x04
    SULongKeyedPair(const SULongKeyedPair& o) : key(o.key), value(o.value) { }   // STLport pair's explicit copy
};
struct SULongKeyPairLess {
    bool operator()(const SULongKeyedPair& a, const SULongKeyedPair& b) const { return a.key < b.key; }
};

__forceinline void __unguarded_linear_insert(SULongKeyedPair* last, SULongKeyedPair val, SULongKeyPairLess comp)
{
    SULongKeyedPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall UnguardedInsertionSortAux_SULongKeyedPair_00471239(SULongKeyedPair* first, SULongKeyedPair* last, SULongKeyedPair*, SULongKeyPairLess comp)
{
    for (SULongKeyedPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SULongKeyedPair(*i), comp);
}