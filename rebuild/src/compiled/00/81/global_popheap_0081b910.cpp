// _global::__pop_heap(SKeyedRecord20* first, SKeyedRecord20* last, SKeyedRecord20* result, SKeyedRecord20 value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for a heap of 24-byte (key, 20-byte record) pairs: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0081B910, length 127 bytes, `ret 24h` (result, the 24-byte value, the comparator
//   and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is 24 bytes (`imul 2AAAAAABh ; sar edx,2`); the callee 0x0081AA50 carries the manifest name
//   `_Adjust_heap<pair<EHeroMorphType,CTextureMorphEntry>*,...,CKeyPairCompareLess<...>>`; the row's own label
//   (pair<unsigned long,CSoundPair>, an 8-byte record in Ego_r.pdb) does not fit the stride, so the record is modelled from
//   the bytes as five dwords behind a 4-byte key. Callers: 0x0081BC79, 0x0081BF79, 0x0081C6D3, 0x0081D4A7.
// Callee: __adjust_heap @ 0x0081AA50.
// Shape: `*result = *first` is the pair's implicit (trivial) assignment, six dwords through edi; the by-value copy for
//   __adjust_heap is built in the argument area by STLport's explicit pair copy constructor: the key, then the record's
//   user copy constructor written as an assignment (`{ *this = other; }`), which materialises the record's own `this`
//   (`add eax,4`, then [eax]..[eax+10h]); a memberwise initialiser-list copy folds the offsets ([eax+4]..[eax+14h]).
struct SRecord20 {                                     // 20-byte record, user copy constructor through the implicit operator=
    long m_0, m_4, m_8, m_c, m_10;
    SRecord20(const SRecord20& other) { *this = other; }
};

struct SKeyedRecord20 {                                // std::pair<key, SRecord20>, 24 bytes
    long      first;                                   // +0x00
    SRecord20 second;                                  // +0x04
    SKeyedRecord20(const SKeyedRecord20& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                           // empty, passed by value
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(SKeyedRecord20* first, long holeIndex, long len, SKeyedRecord20 value,
                              CKeyPairCompareLess comp);   // 0x0081AA50

void __fastcall __pop_heap(SKeyedRecord20* first, SKeyedRecord20* last, SKeyedRecord20* result, SKeyedRecord20 value,
                           CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}