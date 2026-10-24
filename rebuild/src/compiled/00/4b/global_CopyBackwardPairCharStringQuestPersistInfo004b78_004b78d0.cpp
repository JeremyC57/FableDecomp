// _global::CopyBackward_PairCharStringQuestPersistInfo_004b78d0(PairCharStringQuestPersistInfo*, PairCharStringQuestPersistInfo*, PairCharStringQuestPersistInfo*, SRandomAccessIteratorTag, long*) -- SGI __copy_backward over pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>, returns the moved-back result
// Leaf: CopyBackward_PairCharStringQuestPersistInfo_004b78d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B78D0, length 90 bytes.
// Callees: CCharString::operator= @ 0x0099EFB0 (three per element).
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`): `for (n = last - first; n > 0; --n)
// *--result = *--last;` with the signed count computed once; the moved-back result is returned.
// The manifest label is right: Ego_r.pdb CPersistInfo (8 bytes) = { CCharString QuestName; CCharString
// PersistScript }, so the 12-byte pair (`/ 12`) is three strings; the implicit operator= calls the out-of-line
// CCharString::operator= on +0, +4, +8, and the count is held in ebx.
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    struct CPersistInfo {                        // Ego_r.pdb: 8 bytes
        CCharString QuestName;                   // +0x00
        CCharString PersistScript;               // +0x04
    };
};
}

struct PairCharStringQuestPersistInfo {          // std::pair<CCharString, CRuntimeQuestPersistence::CPersistInfo>, 12 bytes
    CCharString                                            first;   // +0x00
    NQuestManager::CRuntimeQuestPersistence::CPersistInfo  second;  // +0x04
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

PairCharStringQuestPersistInfo* __fastcall CopyBackward_PairCharStringQuestPersistInfo_004b78d0(PairCharStringQuestPersistInfo* first, PairCharStringQuestPersistInfo* last, PairCharStringQuestPersistInfo* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}