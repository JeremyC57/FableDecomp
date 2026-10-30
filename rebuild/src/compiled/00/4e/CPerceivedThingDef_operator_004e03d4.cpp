// CPerceivedThingDef::operator=(const CPerceivedThingDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CPerceivedThingDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E03D4, 81 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CPerceivedThingDef::Copy 0x004E03CF; the 10 member operations read from
//   these bytes (byte/dword copies) are Ego_r.pdb CPerceivedThingDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10.
// Shape (family M099, representative 0x004E03D4): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CPerceivedThingDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
enum EPerceivedThingType { EPERCEIVEDTHINGTYPE_NONE = 0 };
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0123AA9C: slot 0 0x004D7EDA (implicit inline dtor); real overrides from the def vtable scan
class CPerceivedThingDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    float                     FieldOfView;                       // +0x28
    float                     GiveUpChaseRadius;                 // +0x2c
    float                     SightRadius;                       // +0x30
    float                     ExtendedSightRadius;               // +0x34
    float                     SoundRadius;                       // +0x38
    float                     SmellRadius;                       // +0x3c
    EPerceivedThingType       Type;                              // +0x40
    float                     AwarenessTimer;                    // +0x44
    float                     DayTimeVisibilityMultiplier;       // +0x48
    float                     NightTimeVisibilityMultiplier;     // +0x4c
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DDF9E
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D35DE
};
void CPerceivedThingDef::Copy(const CDefClassBase* other)
{
    *this = *(const CPerceivedThingDef*)other;
}