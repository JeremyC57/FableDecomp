// std::sort_heap(SQuestPersistPair* first, SQuestPersistPair* last, CKeyPairCompareLess comp) -- SGI sort_heap over the quest manager's (CCharString, CRuntimeQuestPersistence::CPersistInfo) table: pop the root to the back until one element remains
// Leaf: sort_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BB570, length 143 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32>>*,...>` is
// propagated over the byte-identical skeleton. Stride 12 (0x2AAAAAAB / sar 1); the callee 0x004BABF0 (manifest
// `_Pop_heap_0<pair<CCharString,NQuestManager::CRuntimeQuestPersistence::CPersistInfo>*,...>`) is the SGI `__pop_heap`
// of that pair (three CCharString assignments / copies / destructors, `ret 18h`), feeding __adjust_heap 0x004BA180.
// Ego_r.pdb NQuestManager::CRuntimeQuestPersistence::CPersistInfo (8 bytes): CCharString QuestName, PersistScript.
// Callees: __pop_heap @ 0x004BABF0 (out of line, __fastcall), CCharString copy constructor @ 0x0099EC30 (x3).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline. The value copy in the argument area is the implicit pair copy: the key string, then the
// persist info's implicit copy re-based on its own address (`add edi,4` / `lea ebx,[esi+4]`, then `add ebx,4`).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
namespace NQuestManager {
struct CRuntimeQuestPersistence {
    class CPersistInfo { public:                   // Ego_r.pdb, 8 bytes
        CCharString QuestName;              // +0x00
        CCharString PersistScript;          // +0x04
    };
};
}
struct SQuestPersistPair {                  // std::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>
    CCharString                                           first;    // +0x00
    NQuestManager::CRuntimeQuestPersistence::CPersistInfo second;   // +0x04
};
struct CKeyPairCompareLess { };             // empty, passed by value

void __fastcall __pop_heap(SQuestPersistPair* first, SQuestPersistPair* last, SQuestPersistPair* result,
                           SQuestPersistPair value, CKeyPairCompareLess comp, int*);   // 0x004BABF0

inline void __pop_heap_aux(SQuestPersistPair* first, SQuestPersistPair* last, SQuestPersistPair*, CKeyPairCompareLess comp)
{
    __pop_heap(first, last - 1, last - 1, SQuestPersistPair(*(last - 1)), comp, (int*)0);
}

inline void pop_heap(SQuestPersistPair* first, SQuestPersistPair* last, CKeyPairCompareLess comp)
{
    __pop_heap_aux(first, last, (SQuestPersistPair*)0, comp);
}

void __fastcall sort_heap(SQuestPersistPair* first, SQuestPersistPair* last, CKeyPairCompareLess comp)
{
    while (last - first > 1)
        pop_heap(first, last--, comp);
}