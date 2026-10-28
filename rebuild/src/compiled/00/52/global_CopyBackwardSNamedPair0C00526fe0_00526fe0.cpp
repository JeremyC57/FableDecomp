// _global::CopyBackward_SNamedPair0C_00526fe0(SNamedPair0C* first, SNamedPair0C* last, SNamedPair0C* result, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 12-byte {long, long, CCharString} records: counted backward copy, returns the moved-back result
// Leaf: CopyBackward_SNamedPair0C_00526fe0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00526FE0, length 81 bytes, `ret 0Ch`.
// Callees: CCharString::operator= @ 0x0099EFB0.
// Shape: SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)` -- `for (n = last - first;
// n > 0; --n) *--result = *--last;`, the moved-back result returned (landed 0x00517430 family). /Gr TU (__fastcall: first
// in ecx, last in edx, result + the two tags on the stack, `ret 0Ch`). The element's implicit operator= is inlined
// memberwise: the two words through eax/ecx, then the CCharString assignment (`lea` of both +8 members).
// Label correction: the catalog's `_Copy_backward<CSharerParserOutputToDebugContextMapper::CEntry*>` is propagated; member names are descriptive.
class CCharString { public:
    void* m_pData;                               // +0x00
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SNamedPair0C {                           // 12 bytes
    long m_first;                                // +0x00
    long m_second;                               // +0x04
    CCharString m_name;                          // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SNamedPair0C* __fastcall CopyBackward_SNamedPair0C_00526fe0(SNamedPair0C* first, SNamedPair0C* last, SNamedPair0C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}