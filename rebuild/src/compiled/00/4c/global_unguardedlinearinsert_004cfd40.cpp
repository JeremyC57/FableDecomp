// _global::__unguarded_linear_insert(SFloatKeyedRecord* last, SFloatKeyedRecord value, SFloatKeyLess comp) -- SGI insertion-sort step over 28-byte records keyed by a leading float: shift every larger key up one slot, drop `value` into the hole
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CFD40, length 56 bytes (+ int3 padding), `ret 20h` (/Gr algorithm TU:
// `last` in ecx, the 28-byte value and the empty comparator on the stack).
// Caller: the landed __linear_insert 0x004D1000, which declares this body with exactly these types (SFloatKeyedRecord,
// SFloatKeyLess); the model is reused unchanged so the symbol it references is the one defined here.
// Shape: SGI `next = last; --next; while (comp(value, *next)) { *last = *next; last = next; --next; } *last = value;`
// with the comparator inlined to `fld value.key ; fcomp [next] ; test ah,5 ; jp` (value.key < next->key). The implicit
// assignment is a 7-dword `rep movsd` (edi = last, esi = next), the final store copies from the argument slot. The loop
// head is aligned by the speed TU (`lea esp,[esp]` filler).
// Label correction: the catalog's `_Copy_backward<CEngineCamera::CDebugScriptElement*>` is propagated.
struct SPayload {                                 // 24 bytes, POD
    long m_words[6];
};
struct SFloatKeyedRecord {                        // 28 bytes
    float    key;                                 // +0x00
    SPayload payload;                             // +0x04
    SFloatKeyedRecord(const SFloatKeyedRecord& o) : key(o.key), payload(o.payload) { }
};
struct SFloatKeyLess {
    bool operator()(const SFloatKeyedRecord& a, const SFloatKeyedRecord& b) const { return a.key < b.key; }
};

void __fastcall __unguarded_linear_insert(SFloatKeyedRecord* last, SFloatKeyedRecord value, SFloatKeyLess comp)
{
    SFloatKeyedRecord* next = last;
    --next;
    while (comp(value, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = value;
}