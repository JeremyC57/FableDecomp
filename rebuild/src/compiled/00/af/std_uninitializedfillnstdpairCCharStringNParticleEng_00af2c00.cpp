// std::__uninitialized_fill_n<std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,unsigned int,std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo> >(first, n, x, const __false_type&) -- copy-construct n copies of one (name, registered particle-system-component info) pair into raw storage and return first + n
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@IU12@@std@@YIPAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2C00, length 72 bytes, __fastcall (first ecx, n edx), `ret 8` (x and
//   the empty __false_type tag by reference), returns first + n.
// Identity: STLport's `__uninitialized_fill_n(first, n, x, __false_type)` (returns the advanced pointer, pops 8; VC7.1
//   Dinkumware _Uninit_fill_n returns void and pops 12), the fill path of
//   vector<pair<CCharString, CRegisteredPSCInfo> >::_M_insert_overflow at 0x00AF37F0 -- the vector inside CPSCManager's
//   CVectorMap<CCharString, NParticleEngine::CRegisteredPSCInfo> RegisteredPSCs (Ego_r.pdb). The catalog label
//   "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is propagated across byte-identical bodies. Retyped from the
//   non-template stand-in element `SElem { CCharString; SRec { long, long, CCharString } }`.
// Element: Ego_r.pdb CRegisteredPSCInfo (0xC): AllocFunc +0, ComponentType EPSCType +4, GUIName CCharString +8; FableWin.pdb
//   mangles it `VCRegisteredPSCInfo@NParticleEngine` with a copy constructor `??0CRegisteredPSCInfo@NParticleEngine@@QAE@ABV01@@Z`
//   and the field constructor `(CPSCBase* (__fastcall*)(), EPSCType, const CCharString&)`.
// Shape: the pair has STLport's user copy constructor and holds CRegisteredPSCInfo, which has its own user copy
//   constructor: VC7.1 then keeps a second induction register (`lea esi,[eax+N]`) pointing INTO the info record and
//   stores its words relative to it; the placement new keeps the `test eax,eax` null check.
// Callees: CCharString copy constructor 0x0099EC30 (twice per element).
#include <new>

class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);                // 0x0099EC30
};

namespace NParticleEngine {

enum EPSCType { PSC_TYPE_FIRST = 0 };

class CPSCBase;

class CRegisteredPSCInfo {
public:
    CPSCBase* (__fastcall* AllocFunc)();                  // +0
    EPSCType    ComponentType;                            // +4
    CCharString GUIName;                                  // +8
    CRegisteredPSCInfo(const CRegisteredPSCInfo& o) : AllocFunc(o.AllocFunc), ComponentType(o.ComponentType), GUIName(o.GUIName) { }
};

}

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

typedef pair<CCharString, NParticleEngine::CRegisteredPSCInfo> _RegisteredPSCPair;
template _RegisteredPSCPair* __fastcall __uninitialized_fill_n<_RegisteredPSCPair*, unsigned int, _RegisteredPSCPair>(_RegisteredPSCPair*, unsigned int, const _RegisteredPSCPair&, const __false_type&);

}
