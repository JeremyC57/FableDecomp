// _global::CopyBackward_PairCharStringValue0C_0077e170(PairCharStringValue0C*, PairCharStringValue0C*, PairCharStringValue0C*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 16-byte (CCharString, 12-byte value) pairs, returns the moved-back result
// Leaf: CopyBackward_PairCharStringValue0C_0077e170
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0077E170, length 95 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in ecx,
// last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n) *--result = *--last;`
// with the signed count computed once; the moved-back result is returned.
// Element: 16 bytes (`sar 4`) = a CCharString key (out-of-line operator=) + a 12-byte trivially copyable value at +4,
// moved as an inline dword block through the strength-reduced src+4/dst+4 pointers (re-addressed in eax/ecx); the
// count is decremented in the dead `result` argument slot. Same {name, 12-byte value} pair as the landed
// __uninitialized_copy 0x0077E280 of this TU. The manifest label pair<pair<long,long>, CEngineTextureDecal> (28
// bytes in retail) is propagated.
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SValue0C {                                // 12-byte trivially copyable value
    long m_word0, m_word4, m_word8;
};

struct PairCharStringValue0C {                   // 16 bytes
    CCharString first;                           // +0x00
    SValue0C    second;                          // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairCharStringValue0C* __fastcall CopyBackward_PairCharStringValue0C_0077e170(PairCharStringValue0C* first, PairCharStringValue0C* last, PairCharStringValue0C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}