// _global::CopyBackward_SStringPair0C_00a3fe60(SStringPair0C* first, SStringPair0C* last, SStringPair0C* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 12-byte {CCharString, CCharString, long} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SStringPair0C_00a3fe60
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3FE60, length 84 bytes, `ret 0Ch`.
// Callees: CCharString::operator= @ 0x0099EFB0 (twice).
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the two CCharStrings, then the word at +8 (scheduled after the `dec ebx`).
// Label correction: the catalog's `_Copy_backward<CSymbolMap::CSymbolInfo*>` is propagated; member names are descriptive.
class CCharString { public:
    void* m_pData;                               // +0x00
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SStringPair0C {                         // 12 bytes
    CCharString m_key;                           // +0x00
    CCharString m_value;                         // +0x04
    long        m_data;                          // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SStringPair0C* __fastcall CopyBackward_SStringPair0C_00a3fe60(SStringPair0C* first, SStringPair0C* last, SStringPair0C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}