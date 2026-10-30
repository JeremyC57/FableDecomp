// CSummonDef::operator=(const CSummonDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then CreatureToSummon, CreatureFamilyToSummon, NumberToSummon, SummonRadius, MySummonLimit, MinimumSummon, TimeBetweenSummons, SummonEffect, SummoneeEffect memberwise
// Leaf: ??4CSummonDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E17D2, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CSummonDef::operator=(const CSummonDef&) -- proven; evidence: its only caller is the landed CSummonDef::Copy 0x004E17CD
//   (a `jmp` to it: `*this = *static_cast<const CSummonDef*>(other)`); RTTI of vtable 0x0123DEB4 gives CSummonDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CSummonDef's nine members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CSummonDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CThingBuildingDef::operator=_9fields" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x0123DEB4: slot 0 0x004D940A (implicit inline dtor); real overrides from the def vtable scan
class CSummonDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    long CreatureToSummon;                   // +0x28
    long CreatureFamilyToSummon;             // +0x2c
    long NumberToSummon;                     // +0x30
    float SummonRadius;                      // +0x34
    long MySummonLimit;                      // +0x38
    long MinimumSummon;                      // +0x3c
    float TimeBetweenSummons;                // +0x40
    long SummonEffect;                       // +0x44
    long SummoneeEffect;                     // +0x48
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DF264
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D61A0
};
void CSummonDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CSummonDef*>(other);
}