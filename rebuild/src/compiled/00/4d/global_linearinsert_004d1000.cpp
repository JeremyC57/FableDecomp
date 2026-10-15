// _global::__linear_insert(SFloatKeyedRecord* first, SFloatKeyedRecord* last, SFloatKeyedRecord value, SFloatKeyLess comp) -- STLport insertion-sort step over 28-byte records keyed by a leading float: a value sorting before the first element shifts [first, last) up one slot and goes first, otherwise it sinks from `last` with __unguarded_linear_insert
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D1000, length 123 bytes (`ret 20h`: the 28-byte value and the comparator on
// the stack; first/last in ecx/edx -- a /Gr algorithm TU).
// Label correction: the catalog's "CJoystick::EventScanStart" is propagated and wrong (no CJoystick member is touched; the
// body is the 0x0081BDA0 __linear_insert skeleton).
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, Distance*) @ 0x004CF5C0 (`rep movsd` x7 per
// element; manifest "_Copy_backward<pair<CCharString,C3DDummyObject>*>" is propagated), __unguarded_linear_insert(last,
// value, comp) @ 0x004CFD40 (float keys, `fcomp ; test ah,5 ; jp`).
// Element: a float key followed by a 24-byte POD payload whose memberwise user copy constructor rebuilds the record in
// the argument area word by word (the key, then the payload through a re-based pointer); the implicit assignment is the
// 7-dword `rep movsd`. The comparator is an empty functor `a.key < b.key`.
// Shape: as the landed 0x0081BDA0 (the empty iterator tag a const-ref temporary homed in the dead comparator slot).
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
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SFloatKeyedRecord* __fastcall __copy_backward(SFloatKeyedRecord* first, SFloatKeyedRecord* last, SFloatKeyedRecord* result,
                                              const SRandomAccessIteratorTag& category, long* distance);   // 0x004CF5C0
void __fastcall __unguarded_linear_insert(SFloatKeyedRecord* last, SFloatKeyedRecord value, SFloatKeyLess comp);   // 0x004CFD40

static inline SFloatKeyedRecord* copy_backward(SFloatKeyedRecord* first, SFloatKeyedRecord* last, SFloatKeyedRecord* result)
{
    return __copy_backward(first, last, result, SRandomAccessIteratorTag(), (long*)0);
}

void __fastcall __linear_insert(SFloatKeyedRecord* first, SFloatKeyedRecord* last, SFloatKeyedRecord value, SFloatKeyLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    }
    else
        __unguarded_linear_insert(last, value, comp);
}