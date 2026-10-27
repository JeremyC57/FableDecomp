// std::pair::pair(const first_type& a, const second_type& b) -- (pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>) STLport member-wise constructor: copy the key pointer, copy-construct the boast record
// Leaf: ??0?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@QAE@ABQAVCMenuEntry@NUISystem@@ABVCBoast@CQuestManager@@@Z
// Retail: Fable.exe @ 0x0056C5F0, 30 bytes, thiscall `ret 8` (&a, &b).
// Identity (probable): the value_type constructor of the boast map. Its second member is copied by
// CQuestManager::CBoast::CBoast(const CBoast&) 0x004B73A0 (bound ??0CBoast@CQuestManager@@QAE@ABV01@@Z; 32-byte
// record, Ego_r.pdb), and the only pair holding a CBoast in the PDB name sets is the value of
// map<NUISystem::CMenuEntry*, CQuestManager::CBoast> (pair<CMenuEntry* const, CBoast>). The landed 0x0056CB60 is the
// same pair's copy constructor (same 4-byte first + CBoast copy). No direct caller (the xref at 0x0042EF1C is not a
// code reference): an unreferenced COMDAT of the CQuestManager boast-menu TU.
// Callees: CQuestManager::CBoast::CBoast(const CBoast&) 0x004B73A0.
// Shape: STLport `pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}` -- both argument words loaded up
// front, the key stored, then the copy constructor on `lea ecx,[esi+4]` with `&b`.
class CCharString {
public:
    void* PStringData;
};
class CWideString {
public:
    void* PStringData;
};
namespace NUISystem {
class CMenuEntry;
}
class CQuestManager {
public:
    class CBoast {                                          // 32 bytes (Ego_r.pdb)
    public:
        long ID;                                            // +0x00
        long Cost;                                          // +0x04
        long Reward;                                        // +0x08
        CCharString ScriptName;                             // +0x0C
        CWideString Text;                                   // +0x10
        CWideString Title;                                  // +0x14
        bool Evil;                                          // +0x18
        bool Failed;                                        // +0x19
        bool Completed;                                     // +0x1A
        long GroupIndex;                                    // +0x1C
        CBoast(const CBoast& other);                        // 0x004B73A0
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
}

template struct std::pair<NUISystem::CMenuEntry* const, CQuestManager::CBoast>;