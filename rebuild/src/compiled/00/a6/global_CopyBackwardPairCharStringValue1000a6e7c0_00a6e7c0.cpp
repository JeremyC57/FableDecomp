// _global::CopyBackward_PairCharStringValue10_00a6e7c0(PairCharStringValue10*, PairCharStringValue10*, PairCharStringValue10*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over 20-byte (CCharString, 16-byte value) pairs, returns the moved-back result
// Leaf: CopyBackward_PairCharStringValue10_00a6e7c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A6E7C0, length 117 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in ecx,
// last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n) *--result = *--last;`
// with the signed count computed once; the moved-back result is returned.
// Element: 20 bytes (0x66666667 multiply + `sar 3` = / 20) = a CCharString key (out-of-line operator=) + a 16-byte
// trivially copyable value at +4, moved as an inline dword block through the strength-reduced src+4/dst+4 pointers
// (re-addressed in eax/ecx); the count is decremented in the dead `result` argument slot. The manifest label
// pair<pair<long,long>, CEngineTextureDecal> is propagated.
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SValue10 {                                // 16-byte trivially copyable value
    long m_word0, m_word4, m_word8, m_wordC;
};

struct PairCharStringValue10 {                   // 20 bytes
    CCharString first;                           // +0x00
    SValue10    second;                          // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairCharStringValue10* __fastcall CopyBackward_PairCharStringValue10_00a6e7c0(PairCharStringValue10* first, PairCharStringValue10* last, PairCharStringValue10* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}