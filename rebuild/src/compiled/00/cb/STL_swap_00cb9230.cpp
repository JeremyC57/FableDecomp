// std::swap(pair<unsigned __int64,NScript::CRuntimeScriptedEntityPersist::CPersistInfo>& a, pair<...>& b) -- exchange two scripted-entity persistence entries (64-bit key + UID + persist script) through a by-value temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CB9230, 138 bytes (+ int3 padding), /Gr `__fastcall` (a in ecx -> edi, b in edx
//   -> esi), plain `ret`; 0x18-byte frame temporary.
// Identity: the scripted-entity persistence vector map's sort TU (neighbours: the landed __push_heap 0x00CB93E0 and
//   __copy_backward 0x00CB94D0 of pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>); callers
//   are the out-of-line iter_swap 0x00CB9E30 (a bare `jmp`) and the partition step 0x00CBA5C4. Ego_r.pdb CPersistInfo
//   (16 bytes, 8-aligned): ulong64 UID +0, CCharString PersistScript +8, so the pair is 24 bytes (key +0, UID +8,
//   string +0x10, tail padding +0x14 never copied). The catalog's "swap<NParticleEngine::CRegisteredPSCInfo>" is
//   propagated.
// Callees: CCharString copy ctor 0x0099EC30, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0.
// Shape: STLport `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` over STLport's pair (user copy constructor, implicit
//   assignment): the key and UID move as four dwords, the script string through the CCharString members.
class CCharString {
public:
    void* PData;                                        // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NScript {
class CRuntimeScriptedEntityPersist {
public:
    class CPersistInfo {                                // Ego_r.pdb, 16 bytes
    public:
        unsigned __int64 UID;                           // +0x00
        CCharString      PersistScript;                 // +0x08
    };
};
}

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x08
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

typedef pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> TEntityPersistPair;
template void __fastcall swap<TEntityPersistPair>(TEntityPersistPair&, TEntityPersistPair&);
}