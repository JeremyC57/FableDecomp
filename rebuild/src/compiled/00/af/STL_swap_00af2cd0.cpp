// std::swap(pair<CCharString,NParticleEngine::CRegisteredPSCInfo>& a, pair<CCharString,NParticleEngine::CRegisteredPSCInfo>& b) -- exchange two registered particle-system-component entries (GUI key + alloc function, type, GUI name) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF2CD0, 141 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> edi, b in edx
//   -> esi), plain `ret`.
// Identity: the PSC registry sort TU (neighbours: the landed _Construct / __copy_backward / __push_heap of
//   pair<CCharString, NParticleEngine::CRegisteredPSCInfo> at 0x00AF2B70, 0x00AF2D60, 0x00AF2F10); callers are the
//   out-of-line iter_swap 0x00AF2E60 (a bare `jmp`) and the partition step 0x00AF3213. Ego_r.pdb CRegisteredPSCInfo
//   (12 bytes): AllocFunc +0, ComponentType +4, CCharString GUIName +8 -- so the 16-byte pair is {string, ptr, enum,
//   string}. The catalog's "swap<CSymbolMap::CSymbolInfo>" is propagated from a byte twin.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor, implicit
//   assignment) with CRegisteredPSCInfo's implicit members: key string, the two words, GUI name string per copy.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NParticleEngine {
class CPSCBase;
enum EPSCType { PSC_TYPE_NONE = 0 };
class CRegisteredPSCInfo {                              // Ego_r.pdb, 12 bytes
public:
    CPSCBase*   (__cdecl* AllocFunc)();                 // +0x00
    EPSCType    ComponentType;                          // +0x04
    CCharString GUIName;                                // +0x08
};
}

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
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

template void __fastcall swap<pair<CCharString, NParticleEngine::CRegisteredPSCInfo> >(pair<CCharString, NParticleEngine::CRegisteredPSCInfo>&, pair<CCharString, NParticleEngine::CRegisteredPSCInfo>&);
}