// std::swap(pair<CCharString,CScriptInfo>& a, pair<CCharString,CScriptInfo>& b) -- exchange two script-registry entries (name key + CScriptInfo) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB6230, 181 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> edi, b in edx
//   -> esi), plain `ret`.
// Identity: the script registry's sort TU (CScriptInfoManager::Scripts, a CVectorMap<CCharString, CScriptInfo,
//   CKeyPairCompareLess> of 0x18-byte std::pair<CCharString, CScriptInfo>): the out-of-line iter_swap 0x00CB63F0 is a
//   bare `jmp` here and the landed __unguarded_partition 0x00CB67B0 calls it (its inlined iter_swap). The catalog's
//   "swap<CMusicManager::CRegisteredMusicEntry>" is propagated from a byte twin.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport 4.x `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }`. The pair has STLport's user copy constructor
//   (`first(o.first), second(o.second)`) and the implicit assignment; Ego_r.pdb CScriptInfo (0x14: AllocFunc +0,
//   MasterScriptDataAlloc +4, CCharString Name +8, long ID +0xc, bool MasterScript +0x10) copies member by member, so each
//   copy is first's string, the two function pointers, Name's string, ID and the flag; the temporary dies Name first.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

class CScriptInfo {                                     // Ego_r.pdb, 0x14 bytes
public:
    CScriptBase*     (__cdecl* AllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase*);   // +0x00
    CScriptDataBase* (__cdecl* MasterScriptDataAlloc)();                                  // +0x04
    CCharString      Name;                              // +0x08
    long             ID;                                // +0x0c
    bool             MasterScript;                      // +0x10
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<CCharString, CScriptInfo> >(pair<CCharString, CScriptInfo>&, pair<CCharString, CScriptInfo>&);
}