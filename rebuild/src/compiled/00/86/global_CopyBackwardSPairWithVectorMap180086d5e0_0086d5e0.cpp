// _global::CopyBackward_SPairWithVectorMap18_0086d5e0(SPairWithVectorMap18* first, SPairWithVectorMap18* last, SPairWithVectorMap18* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 24-byte {long, long, CVectorMap} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SPairWithVectorMap18_0086d5e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0086D5E0, length 88 bytes, `ret 0Ch`.
// Callees: the 12-byte vector's operator= @ 0x006741E0 (catalog "Vector_Assign").
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the two words, the CVectorMap (its vector through the out-of-line assignment at +8, the empty comparator byte +0x14 copies nothing, the Dirty flag +0x15 -- scheduled after the `dec ebx`).
// Label correction: the catalog's `_Copy_backward<CSharerParserOutputToDebugContextMapper::CEntry*>` is propagated; member names are descriptive.
struct SVector_006741e0 {                       // 12 bytes
    void* m_first;
    void* m_last;
    void* m_end;
    SVector_006741e0& operator=(const SVector_006741e0& other);   // 0x006741E0
};
struct CKeyPairCompareLess {
};
struct SVectorMap {                              // CVectorMap<...>: vector, comparator, dirty flag (the landed 0x006250B0 model)
    SVector_006741e0    Vector;                  // +0x00
    CKeyPairCompareLess Compare;                 // +0x0c
    bool                Dirty;                   // +0x0d
};
struct SPairWithVectorMap18 {                   // 24 bytes
    long m_first;                                // +0x00
    long m_second;                               // +0x04
    SVectorMap m_map;                            // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SPairWithVectorMap18* __fastcall CopyBackward_SPairWithVectorMap18_0086d5e0(SPairWithVectorMap18* first, SPairWithVectorMap18* last, SPairWithVectorMap18* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}