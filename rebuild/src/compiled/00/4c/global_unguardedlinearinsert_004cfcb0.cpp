// _global::__unguarded_linear_insert(SFloatKeyedPair* last, SFloatKeyedPair value, SFloatKeyPairLess comp) -- SGI insertion-sort step over 8-byte {float key, word} pairs: shift every larger key up one slot, drop `value` into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFCB0, length 49 bytes (+ int3 padding), `ret 0Ch` (/Gr algorithm TU:
// `last` in ecx, the 8-byte value and the empty comparator on the stack).
// Caller: 0x004D0F6F inside the insertion sort 0x004D0F10 (catalog "InsertionSort_MoveElementBackward").
// Shape: SGI `next = last; --next; while (comp(value, *next)) { *last = *next; last = next; --next; } *last = value;`,
// no callee-saved registers: `last` stays in ecx, `next` in edx, the comparator inlines to `fld value.key ;
// fcomp [next] ; test ah,5 ; jp` (value.key < next->key) and each assignment is two word moves through eax.
// The element type is not recovered beyond its size and float key (descriptive names).
// Label correction: the catalog's `_Copy_backward<CLandscapeBackgroundPatch::CEdgeToFanBaseMapperElement*>` is propagated.
struct SFloatKeyedPair {                          // 8 bytes
    float key;                                    // +0x00
    long  value;                                  // +0x04
};
struct SFloatKeyPairLess {
    bool operator()(const SFloatKeyedPair& a, const SFloatKeyedPair& b) const { return a.key < b.key; }
};

void __fastcall __unguarded_linear_insert(SFloatKeyedPair* last, SFloatKeyedPair value, SFloatKeyPairLess comp)
{
    SFloatKeyedPair* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}