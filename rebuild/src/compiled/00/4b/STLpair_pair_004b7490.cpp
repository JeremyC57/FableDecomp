// _STL::pair::pair(const pair& other) -- copy constructor of pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>: the key, then the quest name and persist script
// Leaf: ??0?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@_STL@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004B7490, 49 bytes, __fastcall `ret 4`, returns `this`.
// Identity: a 12-byte element of three CCharStrings whose second member is a two-string record copied on its own `this`
//   (ebx = &second): Ego_r.pdb NQuestManager::CRuntimeQuestPersistence::CPersistInfo (8 bytes: QuestName +0,
//   PersistScript +4); FableWin.pdb lists ??0?$pair@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@std@@QAE@ABU01@@Z.
//   The same TU holds the element's swap (0x004B7800: copy, two assignments) and copy_backward (0x004B78D0, stride 12,
//   three 0x0099EFB0 calls) and the twin assignment 0x004B7650. No direct callers (an emitted COMDAT).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (three times).
// Shape: STLport's memberwise `pair(const pair& __o) : first(__o.first), second(__o.second)` with CPersistInfo's
//   implicit copy inlined; edi steps through the source (`add edi,4` twice) because nothing after the last call reads it.
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
template pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>::pair(
    const pair<CCharString, NQuestManager::CRuntimeQuestPersistence::CPersistInfo>&);
}