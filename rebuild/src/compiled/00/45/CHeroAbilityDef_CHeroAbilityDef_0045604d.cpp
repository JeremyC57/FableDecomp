// CHeroAbilityDef::CHeroAbilityDef() -- def default constructor: CParentDefClassBase base, vtable, the three per-level vectors emptied and the dummy-object def index cleared
// Leaf: ??0CHeroAbilityDef@@QAE@XZ
// Retail: Fable.exe @ 0x0045604D, 58 bytes (/Os def module: unaligned entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x01233974 is RTTI .?AVCHeroAbilityDef@@; PDB name set `??0CHeroAbilityDef@@QAE@XZ`; only
// caller the def factory thunk at 0x0045CBDE. The catalog label CEngineStateBlocks is propagated.
// Callee: CParentDefClassBase::CParentDefClassBase() 0x00430370.
// Layout: Ego_r.pdb CHeroAbilityDef, retail = Ego_r - 4 (the base's container) - 4 per vector before the member:
// LevDescription +0x60, StaminaCost +0x70, ExperienceCostsToUpgrade +0x80, DummyObject (CDefIndex, Val = 0) +0x98.
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { public:
    allocator() {}
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};
template <class _Tp, class _Alloc> class _Vector_base { public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, (_Tp*)0) {}
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) {}
};
}
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
enum EHeroAbility { };
enum EHeroExperienceStatCategory { };
class C2DVector { public:
    float X;
    float Y;
};
class CDefIndex { public:
    long Val;
    CDefIndex() : Val(0) { }
};
class CHeroAbilityDef : public CParentDefClassBase { public:
    CHeroAbilityDef();
    EHeroAbility                 Ability;                    // +0x3C
    EHeroExperienceStatCategory  Stat;                       // +0x40
    long                         IconGraphicIndex;           // +0x44
    long                         IconEffectIndex;            // +0x48
    long                         IconActiveEffectIndex;      // +0x4C
    C2DVector                    IconEffectOffset;           // +0x50
    long                         Name;                       // +0x58
    long                         Description;                // +0x5C
    std::vector<long>            LevDescription;             // +0x60
    long                         MaxedOutDescription;        // +0x6C
    std::vector<long>            StaminaCost;                // +0x70
    long                         StatExperienceGainedOnUse;  // +0x7C
    std::vector<long>            ExperienceCostsToUpgrade;   // +0x80
    long                         ABXYPriority;               // +0x8C
    float                        MoralityCostFactor;         // +0x90
    bool                         Aggressive;                 // +0x94
    CDefIndex                    DummyObject;                // +0x98
};
CHeroAbilityDef::CHeroAbilityDef()
{
}