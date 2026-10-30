// CLookDef::operator=(const CLookDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CLookDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E05DF, 111 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CLookDef::Copy 0x004E05DA; the 15 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CLookDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M035, representative 0x00453FC9): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CLookDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0123AE14: slot 0 0x004D8108 (implicit inline dtor); real overrides from the def vtable scan
class CLookDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     EyesTurnRangeXY;                   // +0x28
    float                     EyesTurnRangeYZ;                   // +0x2c
    float                     HeadTurnRangeXY;                   // +0x30
    float                     HeadTurnRangeYZUp;                 // +0x34
    float                     HeadTurnRangeYZDown;               // +0x38
    float                     MaxTurnSpeed;                      // +0x3c
    bool                      AllowTurningViaAnimation;          // +0x40
    float                     AnimationTurnSpeedMultiplier;      // +0x44
    float                     CombatMaxTurnSpeed;                // +0x48
    bool                      CombatAllowTurningViaAnimation;    // +0x4c
    bool                      AllowOversteer;                    // +0x4d
    bool                      AllowUndersteer;                   // +0x4e
    bool                      DrawEyePrimitives;                 // +0x4f
    float                     CutOffNoLooking;                   // +0x50
    bool                      AlwaysUpdateHeadPos;               // +0x54
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DE28C
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D3938
};
void CLookDef::Copy(const CDefClassBase* other)
{
    *this = *(const CLookDef*)other;
}