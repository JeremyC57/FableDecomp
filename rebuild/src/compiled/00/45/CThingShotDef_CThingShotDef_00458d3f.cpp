#pragma optimize("s", on)
// CThingShotDef::CThingShotDef() -- CParentDefClassBase(), vtable, ShotDefinitionClassName (-1) and two zeroed CFloatRange members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00458D3F, oracle length 42 bytes. Size-optimised TU
// (`#pragma optimize("s", on)`: the `or dword ptr [mem],-1` idiom and the fldz/fstp float zeroes are the
// /Os forms; the /Ot build stores immediates and DIFFERs).
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370 (landed). Vtable 0x01234E2C = CThingShotDef
// (rtti-vtables.tsv; the next function, 0x00458D69, is CThingShotDef::GetSizeofClass). The manifest label
// "C3DSkeletalMorph::C3DSkeletalMorph" is propagated and rejected.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CParentDefClassBase::CParentDefClassBase ; fldz ; mov [esi],offset vftable ;
//                or dword ptr [esi+60h],-1 ; fstp [esi+64h] ; fldz ; mov eax,esi ; fstp [esi+68h] ; fldz ; fstp [esi+6Ch] ;
//                fldz ; fstp [esi+70h] ; pop esi ; ret
// Shape: the def's own members start at +0x3C behind the 0x39-byte packed retail base (Ego_r.pdb offsets less 4:
// its CParentDefClassBase is 0x3D). The ctor builds only the members with inline ctors: ShotDefinitionClassName
// (CDefString, TablePos -1) at +0x60 and the two CFloatRange members at +0x64 / +0x6C; the vtable store precedes
// the `-1` store because that member is a sub-object with an inline constructor (a plain `long` in the init list
// sinks the vtable store below it, tried: DIFFER).
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CDefString { public:                        // a def-name table position, -1 = none (inline ctor)
    long TablePos;
    CDefString() : TablePos(-1) { }
};

class CFloatRange { public:                       // Ego_r.pdb CFloatRange (8 bytes), zeroed by its inline ctor
    float HighValue;
    float LowValue;
    CFloatRange() : HighValue(0.0f), LowValue(0.0f) { }
};

// Members from Ego_r.pdb CThingShotDef (its offsets less 4: the retail base is 0x39 bytes packed, the Ego_r one 0x3D);
// only the members up to the two ranges the ctor builds are declared (the def runs on past +0x74).
class CThingShotDef : public CParentDefClassBase { public:
    virtual void Transfer(CPersistContext& context);              // ?Transfer@CThingShotDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase* other);                // ?Copy@CThingShotDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                       // ?GetSizeofClass@CThingShotDef@@UAEKXZ
    float       Speed;                            // +0x3C
    long        PrimaryEffect;                    // +0x40
    long        PrimaryEffectSound;               // +0x44
    long        AttachedContinuousEffect;         // +0x48
    long        AttachedContinuousSound;          // +0x4C
    bool        UseEndTimeLineOnContinousEffect;  // +0x50
    float       HitThingDamage;                   // +0x54
    float       HitThingShieldDamage;             // +0x58
    bool        CauseDamage;                      // +0x5C
    CDefString  ShotDefinitionClassName;          // +0x60
    CFloatRange ShotOffsetAngleRange;             // +0x64
    CFloatRange ShotOffsetRadiusRange;            // +0x6C
    CThingShotDef();
};

CThingShotDef::CThingShotDef()
    : CParentDefClassBase()
{
}