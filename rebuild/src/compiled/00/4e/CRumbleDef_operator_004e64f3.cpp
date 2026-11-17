// CRumbleDef::operator=(const CRumbleDef& other) -- the implicit copy assignment: CDefClassBase's, then every member in order
// Leaf: ??4CRumbleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E64F3, 45 bytes, thiscall, `ret 4` (size-optimised module).
// Identity (proven): its only reference is the tail jump of CRumbleDef::Copy 0x004E64EE; the 2 member operations read from
//   these bytes (byte/dword copies and the container operator= calls) are Ego_r.pdb CRumbleDef's members in order (names and types are Ego_r.pdb's).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10, _Rb_tree<EQuakeStrength, ...>::operator= 0x004E6520, _Rb_tree<EQuakeLength, ...>::operator= 0x004E6645.
// Shape (family M043, representative 0x0045D2FC): compiler-generated `operator=`, emitted through Copy's
//   `*this = *(const CRumbleDef*)other;`; `optimize("s", on)` keeps every scalar copy in eax/al.
// Header debt: the def base is modelled by its retail size only (its operator= is out of line).
#pragma optimize("s", on)
// NF70 (round 7, land1117fu q10784 / q10785): the row's own `namespace std` map / vector models and its two stand-in enums are gone; the
//   maps are STLport's (stlport/fable_map.h) and the enums engine/NCreatureActionEvents.h's. map's operator= is implicit and inline, so each
//   member assignment calls the instance's out-of-line _Rb_tree::operator= (0x004E6520 / 0x004E6645), as retail does.
#include "stlport/fable_map.h"
#include "engine/NCreatureActionEvents.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
// def vtable 0x0124273C: slot 0 0x004E32A6 (implicit inline dtor); real overrides from the def vtable scan
class CRumbleDef : public CDefClassBase {
public:
    void Copy(const CDefClassBase* other);
    std::map<EQuakeStrength, float> QuakeIntensities;                  // +0x28
    std::map<EQuakeLength, float> QuakeDurations;                    // +0x34
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E676A
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004E1744
};
void CRumbleDef::Copy(const CDefClassBase* other)
{
    *this = *(const CRumbleDef*)other;
}