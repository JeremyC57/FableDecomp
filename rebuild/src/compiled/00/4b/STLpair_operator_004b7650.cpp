// _STL::pair::operator=(const pair& other) -- copy assignment of pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>: the key, then the quest name and persist script
// Leaf: ??4?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@_STL@@QAEAAU01@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B7650, 49 bytes, __fastcall `ret 4`, returns `*this`.
// Identity: twin of the copy constructor 0x004B7490 (same 12-byte element of three CCharStrings: key, then
//   NQuestManager::CRuntimeQuestPersistence::CPersistInfo {QuestName, PersistScript}, Ego_r.pdb); the TU's swap 0x004B7800
//   and copy_backward 0x004B78D0 assign the same element. No direct callers (an emitted COMDAT).
// Callee: CCharString::operator=(const CCharString&) @ 0x0099EFB0 (three times).
// Shape: the compiler-generated pair assignment (STLport's pair declares none) with CPersistInfo's implicit assignment
//   inlined on its own `this` (ebx = &second); the address-of below makes VC7.1 emit the implicit `??4` out of line.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {
    public:
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

namespace _STL {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
}

typedef _STL::pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo> TQuestPersistPair;
TQuestPersistPair& (TQuestPersistPair::*g_pAssignQuestPersistPair)(const TQuestPersistPair&) = &TQuestPersistPair::operator=;