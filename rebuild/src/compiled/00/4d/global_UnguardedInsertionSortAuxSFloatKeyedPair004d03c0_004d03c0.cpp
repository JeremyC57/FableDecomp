// _global::UnguardedInsertionSortAux_SFloatKeyedPair_004d03c0(SFloatKeyedPair* first, SFloatKeyedPair* last, SFloatKeyedPair*, SFloatKeyPairLess comp) -- SGI __unguarded_insertion_sort_aux over 8-byte {float key, word} pairs: sink every element below its larger predecessors (no lower-bound test)
// Leaf: UnguardedInsertionSortAux_SFloatKeyedPair_004d03c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D03C0, length 87 bytes (+ int3 padding), `ret 8`.
// Caller: the __unguarded_insertion_sort(first, last, comp) front end 0x004D0A30 (`push comp ; push 0 ; call`), from the
// __final_insertion_sort of a float-keyed vector (the sibling __unguarded_linear_insert @ 0x004CFCB0 has the same element).
// Shape: SGI `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` with the step inlined:
// `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;` -- the value's
// key lives in the 8-byte frame (the x87 compare reads it from memory: `fld [esp+0Ch] ; fcomp [edi] ; test ah,5 ; jp`),
// its word in ebx, and the final store reloads the key through eax; each shift is two word moves through eax. That
// allocation needs the element's memberwise USER copy constructor (STLport pair's explicit copy): with the implicit copy
// VC7.1 keeps the key in ebx as well and spends ebp on the word (84 bytes). /Gr algorithm TU (__fastcall: first in ecx,
// last in edx, the unused value-type pointer and the empty comparator on the stack).
// Label correction: the catalog's `_Copy_impl<std::pair<EHeroMorphType,CParticleMorphs::CEntry>*,...>` is propagated;
// descriptive element names (as the sibling 0x004CFCB0).
struct SFloatKeyedPair {                         // 8 bytes
    float key;                                   // +0x00
    long  value;                                 // +0x04
    SFloatKeyedPair(const SFloatKeyedPair& o) : key(o.key), value(o.value) { }   // STLport pair's explicit copy
};
struct SFloatKeyPairLess {
    bool operator()(const SFloatKeyedPair& a, const SFloatKeyedPair& b) const { return a.key < b.key; }
};

inline void __unguarded_linear_insert(SFloatKeyedPair* last, SFloatKeyedPair val, SFloatKeyPairLess comp)
{
    SFloatKeyedPair* next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

void __fastcall UnguardedInsertionSortAux_SFloatKeyedPair_004d03c0(SFloatKeyedPair* first, SFloatKeyedPair* last, SFloatKeyedPair*, SFloatKeyPairLess comp)
{
    for (SFloatKeyedPair* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SFloatKeyedPair(*i), comp);
}