// _global::__pop_heap(SKeyedRecord24* first, SKeyedRecord24* last, SKeyedRecord24* result, SKeyedRecord24 value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of 28-byte (key, 24-byte record) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D0B50, length 117 bytes, `ret 28h` (result, the 28-byte value, the comparator
//   and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Label correction: the manifest's `_Pair_base<CDateAndTime const,CWideString>` is propagated (that pair is 0x20 bytes and
//   copy-constructs a CWideString; see 0x005921A4). The body is the out-of-line __pop_heap of a 28-byte element
//   (`imul 92492493h ; add edx,ecx ; sar edx,4` = divide by 28) whose callee 0x004D0660 is the matching __adjust_heap
//   (catalog "_Adjust_heap<CDebugBox*,int,CDebugBox>", propagated: CDebugBox is 0x54 bytes). Callers 0x004D0E0F, 0x004D116F,
//   0x004D1CE4 (the pop_heap / sort_heap front ends of the same TU).
// Callee: __adjust_heap @ 0x004D0660.
// Layout: a 4-byte key + a 24-byte record of dwords (modelled from the bytes; the element type is not identified).
// Shape: `*result = *first` is the pair's implicit (trivial) assignment of 28 bytes: `mov ecx,7 ; rep movsd`; the by-value
//   copy is built in the argument area by STLport's explicit pair copy constructor: the key, then the record's user copy
//   constructor written as an assignment (`{ *this = other; }`), which materialises the record's `this` (`add eax,4`).
struct SRecord24 {                                     // 24-byte record, user copy constructor through the implicit operator=
    long m_0, m_4, m_8, m_c, m_10, m_14;
    SRecord24(const SRecord24& other) { *this = other; }
};

struct SKeyedRecord24 {                                // std::pair<key, SRecord24>, 28 bytes
    long      first;                                   // +0x00
    SRecord24 second;                                  // +0x04
    SKeyedRecord24(const SKeyedRecord24& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // empty, passed by value
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(SKeyedRecord24* first, long holeIndex, long len, SKeyedRecord24 value,
                              CKeyPairCompareLess comp);   // 0x004D0660

void __fastcall __pop_heap(SKeyedRecord24* first, SKeyedRecord24* last, SKeyedRecord24* result, SKeyedRecord24 value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}