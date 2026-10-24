// _global::CopyBackward_SSymbolInfo18_0052b640(SSymbolInfo18* first, SSymbolInfo18* last, SSymbolInfo18* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 24-byte {CCharString, 12-byte vector, long, byte} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SSymbolInfo18_0052b640
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0052B640, length 91 bytes (+ int3 padding), `ret 0Ch`.
// Callees: CCharString::operator= @ 0x0099EFB0, the vector's operator= @ 0x005295B0 (catalog "std::vector_assign_buffer").
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the CCharString, the vector at +4, then the word +0x10 and the byte +0x14 (scheduled after the `dec ebx`).
// Label correction: the catalog's `_Copy_backward<CSymbolMap::CSymbolInfo*>` is propagated; member names are descriptive.
struct CCharString {
    void* m_pData;                               // +0x00
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SVector_005295b0 {                       // 12 bytes
    void* m_first;
    void* m_last;
    void* m_end;
    SVector_005295b0& operator=(const SVector_005295b0& other);   // 0x005295B0
};
struct SSymbolInfo18 {                          // 24 bytes
    CCharString m_name;                          // +0x00
    SVector_005295b0 m_values;                   // +0x04
    long m_type;                                 // +0x10
    bool m_flag;                                 // +0x14
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SSymbolInfo18* __fastcall CopyBackward_SSymbolInfo18_0052b640(SSymbolInfo18* first, SSymbolInfo18* last, SSymbolInfo18* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}