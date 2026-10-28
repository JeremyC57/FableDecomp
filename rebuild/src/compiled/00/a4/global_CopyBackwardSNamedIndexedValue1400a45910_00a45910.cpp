// _global::CopyBackward_SNamedIndexedValue14_00a45910(SNamedIndexedValue14*, SNamedIndexedValue14*, SNamedIndexedValue14*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 20-byte {CCharString, long, 12-byte value} records, returns the moved-back result
// Leaf: CopyBackward_SNamedIndexedValue14_00a45910
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A45910, length 122 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in ecx,
// last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n) *--result = *--last;`
// with the signed count computed once; the moved-back result is returned.
// Element: 20 bytes (0x66666667 multiply + `sar 3` = / 20): the CCharString at +0 (out-of-line operator=), a long
// at +4 and a 12-byte trivially copyable value at +8 moved as an inline dword block (re-addressed in ecx/edx) --
// the CTargetPoint shape (landed 0x00806EA0) with a string in place of the intelligent pointer. The count lives
// in the dead `result` argument slot. The manifest label pair<pair<long,long>, CEngineTextureDecal> is propagated.
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SValue0C {                                // 12-byte trivially copyable value
    long m_word0, m_word4, m_word8;
};

struct SNamedIndexedValue14 {                    // 20 bytes
    CCharString m_name;                          // +0x00
    long        m_index;                         // +0x04
    SValue0C    m_value;                         // +0x08
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SNamedIndexedValue14* __fastcall CopyBackward_SNamedIndexedValue14_00a45910(SNamedIndexedValue14* first, SNamedIndexedValue14* last, SNamedIndexedValue14* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}