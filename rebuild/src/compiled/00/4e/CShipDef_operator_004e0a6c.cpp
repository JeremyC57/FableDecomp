// CShipDef::operator=(const CShipDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then SightingFrequency, SightingOffset, DaysVisible, Swell, Pitch, Roll, WaveLength memberwise
// Leaf: ??4CShipDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0A6C, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CShipDef::operator=(const CShipDef&) -- proven; evidence: its only caller is the landed CShipDef::Copy 0x004E0A67
//   (a `jmp` to it: `*this = *static_cast<const CShipDef*>(other)`); RTTI of vtable 0x0123C0A4 gives CShipDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CShipDef's seven members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CShipDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x0123C0A4: slot 0 0x004D87BD (implicit inline dtor); real overrides from the def vtable scan
class CShipDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    long SightingFrequency;                  // +0x28
    long SightingOffset;                     // +0x2c
    long DaysVisible;                        // +0x30
    float Swell;                             // +0x34
    float Pitch;                             // +0x38
    float Roll;                              // +0x3c
    float WaveLength;                        // +0x40
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE782
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D489D
};
void CShipDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CShipDef*>(other);
}