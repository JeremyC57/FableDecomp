// _global::Copy_StringStringVector_00769b90(SStringStringVector*, SStringStringVector*, SStringStringVector*, const random_access_iterator_tag&, int*) -- SGI __copy over 0x14-byte (string, string, vector) records
// Leaf: Copy_StringStringVector_00769b90
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00769B90, length 97 bytes (+ int3 padding).
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the advanced result is returned.
// Element: 0x14 bytes = two CCharStrings (+0, +4: the out-of-line CCharString::operator= @ 0x0099EFB0) and three
// plain words copied by offset (+8..+0x10; separate members -- a C3DVector struct member copies through
// pointer registers instead). The manifest's `_Copy_backward<pair<CCharString,
// C3DVector>*>` label is propagated (that pair is 16 bytes and this is a forward copy); the TU is a COMDAT-folded mix
// of STL helpers, so the record stays descriptive.
class CCharString { public:                         // one-word handle
    void* m_pData;                           // +0x00
    const CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};

struct SStringStringVector {                 // 0x14 bytes
    CCharString First;                       // +0x00
    CCharString Second;                      // +0x04
    float       X;                           // +0x08
    float       Y;                           // +0x0C
    float       Z;                           // +0x10
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SStringStringVector* __fastcall Copy_StringStringVector_00769b90(SStringStringVector* first, SStringStringVector* last, SStringStringVector* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}