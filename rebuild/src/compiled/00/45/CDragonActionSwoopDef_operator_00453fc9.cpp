// CDragonActionSwoopDef::operator=(const CDragonActionSwoopDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CDragonActionSwoopDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00453FC9, 111 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CDragonActionSwoopDef::Copy 0x00453FC4; the 15 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CDragonActionSwoopDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M035, representative 0x00453FC9): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CDragonActionSwoopDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x01232744: slot 0 0x0044FE5F (implicit inline dtor); real overrides from the def vtable scan
class CDragonActionSwoopDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     IntoDelay;                         // +0x28
    float                     OutOfDelay;                        // +0x2c
    float                     Distance;                          // +0x30
    float                     MidPointsControlOffset;            // +0x34
    float                     StartHeight;                       // +0x38
    float                     MidHeight;                         // +0x3c
    float                     EndHeight;                         // +0x40
    float                     FlightSpeed;                       // +0x44
    float                     MaxSplineProgressForSplineAdjustment; // +0x48
    float                     MaxSplineProgressForBurninationEffect; // +0x4c
    float                     MinSplineProgressForWingFlap;      // +0x50
    float                     MaxExplosionRange;                 // +0x54
    float                     ExplosionLineProjectionAngle;      // +0x58
    long                      FramesBetweenExplosions;           // +0x5c
    float                     WindSpeed;                         // +0x60
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00452BE2
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C5CF
};
void CDragonActionSwoopDef::Copy(const CDefClassBase* other)
{
    *this = *(const CDragonActionSwoopDef*)other;
}