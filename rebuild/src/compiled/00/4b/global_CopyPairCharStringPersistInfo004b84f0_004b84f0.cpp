// _global::Copy_PairCharStringPersistInfo_004b84f0(const SPairCharStringPersistInfo* first, const SPairCharStringPersistInfo* last, SPairCharStringPersistInfo* result, const random_access_iterator_tag&, int*) -- assign [first, last) of (quest key, persist info) pairs onto result (three string assignments), return the advanced result
// Leaf: Copy_PairCharStringPersistInfo_004b84f0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B84F0, length 90 bytes (`ret 0Ch`).
// Identity: `__copy<std::pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>*>` -- the catalog element is right (12 bytes),
//   matching the landed callers CQuestManager::ClearQuestPersistScript 0x004B39B0 and CRuntimeQuestPersistence::ClearAll
//   0x004B41F0 (both declare `__copy(first, last, result, random_access_iterator_tag, ptrdiff_t*)` @ 0x004B84F0).
// Shape: SGI `__copy(first, last, result, random_access_iterator_tag, Distance*)` (the counted form), `__fastcall`: first in
//   ecx, last in edx, result + the tag reference + the distance pointer on the stack (`ret 0Ch`); the signed count is
//   (last - first) / sizeof(T) (reciprocal multiply), counted down; an empty range returns the untouched result.
//   Three CCharString::operator= 0x0099EFB0 calls per element at [esi+N]/[edi+N].
struct random_access_iterator_tag { random_access_iterator_tag() { } };   // empty tag, passed by const reference

struct CCharString {                            // 4-byte counted string handle
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    struct CPersistInfo {                       // Ego_r.pdb, 8 bytes
        CCharString QuestName;                  // +0x00
        CCharString PersistScript;              // +0x04
    };
};
}

struct SPairCharStringPersistInfo {             // std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>, 12 bytes
    CCharString                                           first;    // +0x00
    NQuestManager::CRuntimeQuestPersistence::CPersistInfo second;   // +0x04
};

SPairCharStringPersistInfo* __fastcall Copy_PairCharStringPersistInfo_004b84f0(const SPairCharStringPersistInfo* first, const SPairCharStringPersistInfo* last, SPairCharStringPersistInfo* result, const random_access_iterator_tag&, int*)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}