// CCombatTypeDef::CCombatTypeDef() -- def default constructor: CParentDefClassBase base, vtable, the three vectors emptied, then the attack-pattern map built
// Leaf: ??0CCombatTypeDef@@QAE@XZ
// Retail: Fable.exe @ 0x00455DA3, 58 bytes (/Os def module: unaligned entry), thiscall `ret`, returns this.
// Identity: the stored vtable 0x01233744 is RTTI .?AVCCombatTypeDef@@; PDB name set `??0CCombatTypeDef@@QAE@XZ`; only
// caller the def factory thunk at 0x0045C505. The catalog label C3DMeshMaterial is propagated.
// Callees: CParentDefClassBase::CParentDefClassBase() 0x00430370; map<ECombatCreatureType,long>::map() 0x0045338B
// (STLport map default ctor, out of line: it calls the _Rb_tree(compare, allocator) ctor 0x00451876).
// Layout: Ego_r.pdb CCombatTypeDef, each offset retail = Ego_r - 4 (the base's container) - 4 per vector before it:
// DefenderCombatRingOccupationNumbers +0x3C, CombatSequence +0x50, CombatSequenceDef +0x5C, AttackPatterns +0x84.
// engine/CCombatTypeDef.h is data-only (flattened vptr, Dinkumware sizes), so the class is modelled locally.
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
namespace std {
template <class _Tp> struct less { };
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map { public:                                      // STLport 4.6 map: one _Rb_tree (header, node count, compare), 12 bytes
    map();                                               // out of line
    void*        _M_header;
    unsigned int _M_node_count;
    _Compare     _M_key_compare;
};
}
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
class CCharString { public:
    CCharString();                                       // 0x0099E4B0
    ~CCharString();
    void* PStringData;
};
enum ECombatCreatureType { };
class CCombatTypeDef : public CParentDefClassBase { public:
    CCombatTypeDef();
    std::vector<long>                       DefenderCombatRingOccupationNumbers;   // +0x3C
    long                                    DefenderWheelOuterRingStartIndex;      // +0x48
    bool                                    DefenderWheelAllowAttackerStrafing;    // +0x4C
    std::vector<CCharString>                CombatSequence;                        // +0x50
    std::vector<long>                       CombatSequenceDef;                     // +0x5C
    bool                                    PreferOuterRing;                       // +0x68
    float                                   FrontFarLimit;                         // +0x6C
    float                                   FrontMiddleLimit;                      // +0x70
    float                                   FrontNearLimit;                        // +0x74
    float                                   SideLimit;                             // +0x78
    float                                   RearLimit;                             // +0x7C
    float                                   PreferredCombatDistance;               // +0x80
    std::map<ECombatCreatureType, long>     AttackPatterns;                        // +0x84
};
CCombatTypeDef::CCombatTypeDef()
{
}