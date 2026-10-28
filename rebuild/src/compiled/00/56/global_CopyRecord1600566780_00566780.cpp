// _global::Copy_Record16_00566780(const SRecord16* first, const SRecord16* last, SRecord16* result, const random_access_iterator_tag&, int*) -- assign [first, last) of 16-byte {three strings, two flags} records onto result, return the advanced result
// Leaf: Copy_Record16_00566780
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00566780, length 89 bytes (`ret 0Ch`).
// Identity: catalog `_Move<std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*>` is propagated (that
//   pair is 12 bytes; this body strides 16 with three string assignments and two flag bytes). Callers: the tree find
//   0x00566FE0, the tree erase 0x005689C0 (both landed) and the vector resize 0x00569D90; the element stays descriptive.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   Flat record: [esi+N]/[edi+N] throughout (a nested second member adds an induction pair: tried).
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

class CCharString { public:                            // 4-byte counted string handle
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SRecord16 {                              // 16 bytes, flat
    CCharString m_0;                            // +0x00
    CCharString m_4;                            // +0x04
    CCharString m_8;                            // +0x08
    bool        m_flagC;                        // +0x0c
    bool        m_flagD;                        // +0x0d
};

SRecord16* __fastcall Copy_Record16_00566780(const SRecord16* first, const SRecord16* last, SRecord16* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}