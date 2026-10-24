// _global::CopyBackward_SStringRecord1C_00623390(SStringRecord1C* first, SStringRecord1C* last, SStringRecord1C* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 28-byte records of six CCharStrings and a word: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SStringRecord1C_00623390
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623390, length 138 bytes, `ret 0Ch`.
// Callees: CCharString::operator= @ 0x0099EFB0 (six times).
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: CCharStrings at +0 and +4, the word at +8, CCharStrings at +0xC..+0x18.
// Label correction: the catalog's `_Copy_backward<CSymbolMap::CSymbolInfo*>` is propagated; member names are descriptive.
struct CCharString {
    void* m_pData;                               // +0x00
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SStringRecord1C {                       // 28 bytes
    CCharString m_a;                             // +0x00
    CCharString m_b;                             // +0x04
    long        m_value;                         // +0x08
    CCharString m_c;                             // +0x0c
    CCharString m_d;                             // +0x10
    CCharString m_e;                             // +0x14
    CCharString m_f;                             // +0x18
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SStringRecord1C* __fastcall CopyBackward_SStringRecord1C_00623390(SStringRecord1C* first, SStringRecord1C* last, SStringRecord1C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}