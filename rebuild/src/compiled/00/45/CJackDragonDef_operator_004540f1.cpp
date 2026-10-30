// CJackDragonDef::operator=(const CJackDragonDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CJackDragonDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004540F1, 141 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CJackDragonDef::Copy 0x004540EC (catalogued CJackDragonDef::Copy); the member
//   copies (dword / byte at each offset) are Ego_r.pdb CJackDragonDef's members in order (20 members from +0x28).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M033, representative 0x00453C58): compiler-generated `operator=`, emitted here through Copy's
//   `*this = *(const CJackDragonDef*)other;`; `optimize("s", on)` keeps every copy in eax/al (speed rotates eax/ecx/edx).
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0123294C: slot 0 0x0044FFBD (implicit inline dtor); real overrides from the def vtable scan
class CJackDragonDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                 MinTimeBetweenSlams;            // +0x28
    long                  MaxHitsBeforeFlyAway;           // +0x2c
    float                 MaxFirebeamDistance;            // +0x30
    float                 MinSnapDistance;                // +0x34
    float                 MaxSnapDistance;                // +0x38
    float                 MaxTimeAtLongRangeBeforeFlyAway; // +0x3c
    float                 MaxSlamDistance;                // +0x40
    long                  MaxHoverLoops;                  // +0x44
    float                 PercentChanceOfFakeLandingSwoop; // +0x48
    float                 MaxTimeOnGround;                // +0x4c
    float                 DragonGroundHeightOffsetBodge;  // +0x50
    float                 HoverBackOffOffset;             // +0x54
    long                  FramesBetweenBurninateUpdate;   // +0x58
    long                  FramesBetweenFireBeamUpdate;    // +0x5c
    float                 HoverObstructionRadius;         // +0x60
    float                 StrafeTriggerDistance;          // +0x64
    float                 GetHitSnapBuildupTime;          // +0x68
    float                 NotHitSnapBuildupTime;          // +0x6c
    float                 MaxTimeBeforeSnap;              // +0x70
    float                 MinRoarDistance;                // +0x74
    virtual void Transfer(CPersistContext& context);         // slot 18 0x00452EA5
    virtual unsigned long GetSizeofClass();                  // slot 20 0x0044C63D
};
void CJackDragonDef::Copy(const CDefClassBase* other)
{
    *this = *(const CJackDragonDef*)other;
}