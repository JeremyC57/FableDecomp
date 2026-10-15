// CQuestManager::ClearQuestPersistScript(const CCharString& questName) -- drop the quest's runtime persistence entries and clear the thing manager's persistence data for that section
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004B39B0, length 106 bytes (`ret 4`).
// Callees: CVectorMap<CCharString, CPersistInfo>::LowerBound @ 0x004B9750 / UpperBound @ 0x004B9850, STLport
// __copy(first, last, result, random_access_iterator_tag, ptrdiff_t*) @ 0x004B84F0 (`__fastcall`; catalog "_Move"),
// __destroy_aux(first, last, __false_type) @ 0x004B93E0 (`__fastcall`; catalog "vector::_Destroy"),
// CWorld::GetThingManager (slot 3, +0xc), CThingManager::ClearPersistenceDataForSection(const CCharString&)
// @ 0x00520C70 (an `add ecx,0A0h ; jmp` forwarder to CEntityRuntimePersistence).
// Layout: retail CQuestManager: RuntimeQuestPersistence +0xc whose first member is the CVectorMap (STLport vector
// begin +0, finish +4) of 12-byte pair<CCharString, CPersistInfo>; World +0x7c (PDB +0xd8).
// Shape: the range erase of [LowerBound(key), UpperBound(key)) written out over a map reference: end() is taken once
// into a local right after LowerBound (it is both the emptiness test and __copy's `last`; ebx), UpperBound goes into
// a local before the __copy pushes, _Destroy re-reads the finish; both empty tags sit in the dead name-argument
// slot; `this` lives in the `push ecx` frame word; the name is pushed before the GetThingManager call. The plain
// `erase(it, UpperBound(key))` member form re-reads the finish for __copy and pushes before calling UpperBound.
// Leaf: ClearQuestPersistScript
#include <stddef.h>

struct __false_type { __false_type() {} };
struct random_access_iterator_tag { random_access_iterator_tag() {} };

struct CCharString { void* PStringData; };

namespace NQuestManager {
struct CPersistInfo { CCharString a; CCharString b; };
}
struct CPersistPair {                               // pair<CCharString, CPersistInfo>, 12 bytes
    CCharString                 first;
    NQuestManager::CPersistInfo second;
};

CPersistPair* __fastcall CopyPairs(CPersistPair* first, CPersistPair* last, CPersistPair* result,
                                   const random_access_iterator_tag& tag, ptrdiff_t* distance);   // 0x004B84F0
void __fastcall DestroyPairs(CPersistPair* first, CPersistPair* last, const __false_type& tag);    // 0x004B93E0

struct CPersistMap {                                // CVectorMap<CCharString, CPersistInfo>
    CPersistPair* _M_start;
    CPersistPair* _M_finish;
    CPersistPair* _M_end_of_storage;
    CPersistPair* LowerBound(const CCharString& key);   // 0x004B9750
    CPersistPair* UpperBound(const CCharString& key);   // 0x004B9850
    CPersistPair* end() { return _M_finish; }
};

struct CThingManager {
    void ClearPersistenceDataForSection(const CCharString& section);   // 0x00520C70
};

struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager& GetThingManager();       // slot 3 (+0xc)
};

struct CQuestManager {
    unsigned char _pad00[0xc];
    CPersistMap   RuntimeQuestPersistence;          // +0x0c (its quest map)
    unsigned char _pad18[0x64];
    CWorld*       World;                            // +0x7c
    void ClearQuestPersistScript(const CCharString& questName);
};

void CQuestManager::ClearQuestPersistScript(const CCharString& questName)
{
    CPersistMap& map = RuntimeQuestPersistence;
    CPersistPair* it = map.LowerBound(questName);
    CPersistPair* end = map.end();
    if (it != end) {
        CPersistPair* last = map.UpperBound(questName);
        CPersistPair* i = CopyPairs(last, end, it, random_access_iterator_tag(), (ptrdiff_t*)0);
        DestroyPairs(i, map._M_finish, __false_type());
        map._M_finish = i;
    }
    World->GetThingManager().ClearPersistenceDataForSection(questName);
}