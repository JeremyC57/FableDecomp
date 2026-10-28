// _global::__pop_heap(PairCharStringPersistInfo* first, PairCharStringPersistInfo* last, PairCharStringPersistInfo* result, PairCharStringPersistInfo value, CKeyPairCompareLess comp, long*) -- SGI pop_heap core for the quest persistence (name, persist info) heap: *result = *first, then sift `value` down from the root
// Leaf: __pop_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BABF0, length 158 bytes (+ int3 padding), `ret 18h` (result, the 0xc-byte
//   value, the comparator and the distance pointer on the stack; first/last in ecx/edx -- the SGI algorithm TUs are /Gr).
// Identity: the element is pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> (0xc bytes,
//   `imul 2AAAAAABh ; sar edx,1`); the callee 0x004BA180 carries the matching `_Adjust_heap<...CPersistInfo...>` manifest name.
//   The row's `_Pop_heap_0` label is the aux front end's name; this body is the out-of-line core (callers 0x004BAD4C,
//   0x004BB06C, 0x004BB5D3, 0x004BB87E).
// Callees: CCharString::operator= @ 0x0099EFB0 (x3), CCharString::CCharString(const CCharString&) @ 0x0099EC30 (x3),
//   __adjust_heap @ 0x004BA180, CCharString::~CCharString @ 0x0099EAE0 (x3).
// Layout: Ego_r.pdb NQuestManager::CRuntimeQuestPersistence::CPersistInfo (8): CCharString QuestName +0, PersistScript +4.
// Shape: the pair's IMPLICIT assignment is inlined (three string operator= calls through the record's own pointer pair), the
//   by-value copy for __adjust_heap is built in the argument area by STLport's explicit pair copy constructor and the
//   record's implicit one, and the by-value `value` parameter is destroyed by this callee at the end (in reverse).
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
struct CRuntimeQuestPersistence {
    class CPersistInfo { public:                               // Ego_r.pdb, 8 bytes
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

struct PairCharStringPersistInfo {                      // std::pair<CCharString, CPersistInfo>, 0xc bytes
    CCharString                                            first;    // +0x00
    NQuestManager::CRuntimeQuestPersistence::CPersistInfo  second;   // +0x04
    PairCharStringPersistInfo(const PairCharStringPersistInfo& other) : first(other.first), second(other.second) { }
};

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<CCharString, CPersistInfo>, empty
    CKeyPairCompareLess() { }
};

void __fastcall __adjust_heap(PairCharStringPersistInfo* first, long holeIndex, long len, PairCharStringPersistInfo value,
                              CKeyPairCompareLess comp);   // 0x004BA180

void __fastcall __pop_heap(PairCharStringPersistInfo* first, PairCharStringPersistInfo* last, PairCharStringPersistInfo* result,
                           PairCharStringPersistInfo value, CKeyPairCompareLess comp, long*)
{
    *result = *first;
    __adjust_heap(first, 0, long(last - first), value, comp);
}