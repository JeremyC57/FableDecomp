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
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
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
};
void CDoorDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CDoorDef*>(other);
}