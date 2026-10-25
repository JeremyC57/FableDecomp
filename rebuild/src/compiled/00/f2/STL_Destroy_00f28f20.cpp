// _STL::_Destroy(CFeatDef* first, CFeatDef* last) -- run the virtual destructor of every 100-byte feat def in [first, last)
// Leaf: ??$_Destroy@PAVCFeatDef@@@_STL@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F28F20, 34 bytes, __fastcall (first in ecx, last in edx), plain `ret`.
// Identity: the only caller is vector<CFeatDef>::operator= (0x00F2D010, the shrinking branch at 0x00F2D0EF), which
//   CScriptDef::operator= (0x00F2A250) calls for its Feats member (+0x1038; Ego_r.pdb CScriptDef::Feats is
//   vector<CFeatDef>). Stride 0x64 = 100 = Ego_r sizeof(CFeatDef) (a 40-byte def base and scalar/def-string members,
//   no STL members). Catalog "vector<CIntelligentPointer<NParticleEngine::CParticleEmitter>>::_Destroy" is propagated.
// Callees: the element's scalar deleting destructor through vtable slot 0 (`push 0 ; call [eax]`).
// Shape: twin of 0x00CDB820 -- STLport 4.6 `_Destroy(first, last)` from a /Gr TU with `__destroy` / `__destroy_aux`
//   and the explicit virtual destructor call inlined (`push 0 ; mov ecx,esi ; call [eax]`, stride 100).
class CDefString {
public:
    long Index;
};

class CGameDefinitionBase {                   // 40-byte def base (vtable + def bookkeeping)
public:
    virtual ~CGameDefinitionBase();
    unsigned char _defData[0x24];
};

class CFeatDef : public CGameDefinitionBase { // 100 bytes
public:
    CDefString FeatName;                      // +0x28
    CDefString Verb;                          // +0x2C
    float TimeLimit;                          // +0x30
    long TargetNumber;                        // +0x34
    long GoldReward;                          // +0x38
    long XPReward;                            // +0x3C
    CDefString ItemReward;                    // +0x40
    bool NoBlocking;                          // +0x44
    long KN_AttackType;                       // +0x48  EFeatAttackType
    bool KN_Perfect;                          // +0x4C
    long KN_CreatureType;                     // +0x50
    CDefString GF_FromRegion;                 // +0x54
    CDefString GF_ToRegion;                   // +0x58
    bool GF_NoTeleporting;                    // +0x5C
    CDefString CO_ItemName;                   // +0x60
    virtual ~CFeatDef();
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _Tp>
inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator, class _Tp>
inline void __fastcall __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _ForwardIterator>
inline void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy(__first, __last, (CFeatDef*)0);
}

template void __fastcall _Destroy<CFeatDef*>(CFeatDef*, CFeatDef*);
}