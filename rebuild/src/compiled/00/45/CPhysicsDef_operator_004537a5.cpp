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
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
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
};
void CPhysicsDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CPhysicsDef*>(other);
}