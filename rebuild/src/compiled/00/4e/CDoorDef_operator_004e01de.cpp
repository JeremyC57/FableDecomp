// CDoorDef::operator=(const CDoorDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then TriggerType, TriggerRadius, OpenCollisionMesh, ClosedCollisionMesh, PostVandalismCrimeIfDestroyed, LockPickable, PreventLockingByOwner memberwise
// Leaf: ??4CDoorDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E01DE, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CDoorDef::operator=(const CDoorDef&) -- proven; evidence: its only caller is the landed CDoorDef::Copy 0x004E01D9
//   (a `jmp` to it: `*this = *static_cast<const CDoorDef*>(other)`); RTTI of vtable 0x0123A714 gives CDoorDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CDoorDef's seven members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CDoorDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
enum EDoorTriggerType { EDoorTriggerType_Force32 = 0x7fffffff };
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#include "engine/CSubDefClassBase.h"                    // vtable 0x01231D54
// def vtable 0x0123A714: slot 0 0x004D7C0B (implicit inline dtor); real overrides from the def vtable scan
class CDoorDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    EDoorTriggerType TriggerType;            // +0x28
    float TriggerRadius;                     // +0x2c
    long OpenCollisionMesh;                  // +0x30
    long ClosedCollisionMesh;                // +0x34
    bool PostVandalismCrimeIfDestroyed;      // +0x38
    bool LockPickable;                       // +0x39
    bool PreventLockingByOwner;              // +0x3a
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004DDB76
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004D331C
};
void CDoorDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CDoorDef*>(other);
}