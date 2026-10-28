// std::pair::pair(const pair& other) -- copy constructor of pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>: the UID key, then the persist info's UID and script string
// Leaf: ??0?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB90F0, 47 bytes, __fastcall `ret 4`, returns `this`.
// Identity (label correction): the catalog's "pair<pair<long,long>,CEngineTextureDecal>" is propagated. The five callers
//   0x00CBCBD2..0x00CBCDC4 are the scripting library's 24-byte introsort loops building `_Tp(__median(...))` for
//   __unguarded_partition 0x00CBA580 (the landed __median 0x00CB9D30 names the element): std::pair<unsigned __int64,
//   NScript::CRuntimeScriptedEntityPersist::CPersistInfo> (0x18 bytes, 8-aligned: key +0, CPersistInfo::UID +8,
//   PersistScript +0x10); FableWin.pdb has ??0?$pair@_KVCPersistInfo@CRuntimeScriptedEntityPersist@NScript@@@std@@QAE@ABU01@@Z.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport's memberwise pair copy constructor with CPersistInfo's implicit copy (four dword moves, then the string).
class CCharString {
public:
    void* m_pStringData;                                // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {                                // Ego_r.pdb: 16 bytes (8-aligned)
    public:
        unsigned __int64 UID;                           // +0x00
        CCharString      PersistScript;                 // +0x08
    };
};
}

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>::pair(
    const pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>&);
}