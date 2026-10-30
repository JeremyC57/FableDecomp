// CPhysicsDef::operator=(const CPhysicsDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then Diameter, Friction, Elasticity, AirResistance, Mass, InteractionFlags, PushableByHero memberwise
// Leaf: ??4CPhysicsDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004537A5, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CPhysicsDef::operator=(const CPhysicsDef&) -- proven; evidence: its only caller is the landed CPhysicsDef::Copy 0x004537A0
//   (a `jmp` to it: `*this = *static_cast<const CPhysicsDef*>(other)`); RTTI of vtable 0x01231E34 gives CPhysicsDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CPhysicsDef's seven members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CPhysicsDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x01231E34: slot 0 0x0044F6BA (implicit inline dtor); real overrides from the def vtable scan
class CPhysicsDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    float Diameter;                          // +0x28
    float Friction;                          // +0x2c
    float Elasticity;                        // +0x30
    float AirResistance;                     // +0x34
    float Mass;                              // +0x38
    long InteractionFlags;                   // +0x3c
    bool PushableByHero;                     // +0x40
    virtual void Transfer(CPersistContext& context);         // slot 18 0x0045212A
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C158
};
void CPhysicsDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CPhysicsDef*>(other);
}