// CNymphDef::operator=(const CNymphDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CNymphDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E176E, 81 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CNymphDef::Copy 0x004E1769; the 10 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CNymphDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M099, representative 0x004E03D4): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CNymphDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0123DE3C: slot 0 0x004D93C4 (implicit inline dtor); real overrides from the def vtable scan
class CNymphDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     MinAttackRange;                    // +0x28
    float                     MaxHeight;                         // +0x2c
    float                     MinHeight;                         // +0x30
    float                     CircleRange;                       // +0x34
    long                      FlyAroundTime;                     // +0x38
    long                      ShotTrapType;                      // +0x3c
    long                      ShotsBeforeIdle;                   // +0x40
    float                     IdleTimeBetweenShots;              // +0x44
    float                     FadeInTime;                        // +0x48
    float                     FadeOutTime;                       // +0x4c
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DF1E9
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D6177
};
void CNymphDef::Copy(const CDefClassBase* other)
{
    *this = *(const CNymphDef*)other;
}