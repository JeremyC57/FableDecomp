// CSpecialAbilitiesDrunkennessDef::operator=(const CSpecialAbilitiesDrunkennessDef& other) -- memberwise copy: the CParentDefClassBase part out of line, then the five members; returns *this
// Leaf: ??4CSpecialAbilitiesDrunkennessDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460589, 51 bytes, thiscall, `ret 4` (size-optimised def module).
// Identity: its only reference is CSpecialAbilitiesDrunkennessDef::Copy 0x00460584 (slot 19 of the class's RTTI vftable; RTTI placement +
//   both PDB name sets), a 5-byte `jmp` to it (`*this = *(const CSpecialAbilitiesDrunkennessDef*)other;`); ??4CSpecialAbilitiesDrunkennessDef
//   is in Ego_r.pdb and FableWin.pdb. The five copied dwords +0x3C..+0x4C are Ego_r.pdb CSpecialAbilitiesDrunkennessDef's members in order
//   (DrunkenGraphic, MaxAlpha, USpeed, VSpeed, RotSpeed at Ego_r +0x40..+0x50; retail = Ego_r - 4 after the 0x39-byte retail CParentDefClassBase).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0.
// NF337 (2026-11-24): respelled from the made-up CBase_00431ed0_Derived_00460589 / CBase_00431ed0 (manifest label propagated) onto the real
//   class and the shared retail def base, in the sibling operator= rows' form (0x00460185, 0x0046051D); the scalar floats copy as dword moves.
#pragma optimize("s", on)
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

class CSpecialAbilitiesDrunkennessDef : public CParentDefClassBase { public:   // no engine/CSpecialAbilitiesDrunkennessDef.h
    long  DrunkenGraphic;                   // +0x3c
    long  MaxAlpha;                         // +0x40
    float USpeed;                           // +0x44
    float VSpeed;                           // +0x48
    float RotSpeed;                         // +0x4c
    CSpecialAbilitiesDrunkennessDef& operator=(const CSpecialAbilitiesDrunkennessDef& other);
};

CSpecialAbilitiesDrunkennessDef& CSpecialAbilitiesDrunkennessDef::operator=(const CSpecialAbilitiesDrunkennessDef& other)
{
    CParentDefClassBase::operator=(other);
    DrunkenGraphic = other.DrunkenGraphic;
    MaxAlpha = other.MaxAlpha;
    USpeed = other.USpeed;
    VSpeed = other.VSpeed;
    RotSpeed = other.RotSpeed;
    return *this;
}
