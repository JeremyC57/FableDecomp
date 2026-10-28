// _global::Copy_Record28_006237c0(const SRecord28* first, const SRecord28* last, SRecord28* result, const random_access_iterator_tag&, int*) -- assign [first, last) of 0x1c-byte {six strings, one word} records onto result, return the advanced result
// Leaf: Copy_Record28_006237c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006237C0, length 137 bytes (`ret 0Ch`).
// Identity: catalog `_Move<std::pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*>` is propagated
//   (that pair is 12 bytes; this body strides 0x1c with six CCharString::operator= 0x0099EFB0 calls and a word at +8).
//   Callers: the tree lower-bound helper 0x00623AF0 and the vector assign 0x00624A30; the element stays descriptive.
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   A flat record keeps every member at [esi+N]/[edi+N] (a nested {string, word, ...} second member adds an
//   induction pair at +0x0c: tried).
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

class CCharString { public:                            // 4-byte counted string handle
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct SRecord28 {                              // 0x1c bytes, flat (a nested struct would add an induction pair)
    CCharString m_0;                            // +0x00
    CCharString m_4;                            // +0x04
    long        m_8;                            // +0x08
    CCharString m_c;                            // +0x0c
    CCharString m_10;                           // +0x10
    CCharString m_14;                           // +0x14
    CCharString m_18;                           // +0x18
};

SRecord28* __fastcall Copy_Record28_006237c0(const SRecord28* first, const SRecord28* last, SRecord28* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}