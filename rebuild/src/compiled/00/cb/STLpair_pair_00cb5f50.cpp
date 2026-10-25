// std::pair::pair(const pair& other) -- copy constructor of pair<CCharString, CScriptInfo>: the key string, then the script info memberwise
// Leaf: ??0?$pair@VCCharString@@VCScriptInfo@@@std@@QAE@ABU01@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB5F50, 57 bytes, __fastcall `ret 4`, returns `this`.
// Identity: the element of CScriptInfoManager::Scripts (Ego_r.pdb: CVectorMap<CCharString, CScriptInfo,
//   CKeyPairCompareLess<CCharString, CScriptInfo> >, a vector of pair<CCharString, CScriptInfo>); five callers in the
//   vector copy / insert helpers 0x00CB7430..0x00CB7624. Ego_r.pdb CScriptInfo (20 bytes): AllocFunc +0, MasterScriptDataAlloc
//   +4, Name +8 (CCharString), ID +0xC, MasterScript +0x10 (bool) -- so the pair's second sits at +4 and its string at +0xC.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30 (twice: first, second.Name).
// Shape: `first(p.first), second(p.second)` with CScriptInfo's implicit memberwise copy inlined (two pointers, the name's
//   copy ctor, the ID, the bool byte).
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo {                                    // a class in the PDB name sets (mangled V)
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                                                                 // +0x08
    long             ID;                                                                   // +0x0C
    bool             MasterScript;                                                         // +0x10
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const pair& p) : first(p.first), second(p.second) {}
};
template pair<CCharString, CScriptInfo>::pair(const pair<CCharString, CScriptInfo>&);
}