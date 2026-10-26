// CTCNoiseDef::operator=(const CTCNoiseDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then Type, LifeTime, DistanceCanBeHeardFrom memberwise
// Leaf: ??4CTCNoiseDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004537E9, 39 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CTCNoiseDef::operator=(const CTCNoiseDef&) -- proven; evidence: its only callers are the 5-byte `jmp` 0x004537E4 in vtable
//   slot 19 (Copy) of vtable 0x01231E9C (RTTI .?AVCTCNoiseDef@@ : CSubDefClassBase : CDefClassBase : CDefPointeeBase : CResource),
//   i.e. `CTCNoiseDef::Copy(other) { *this = *static_cast<const CTCNoiseDef*>(other); }`, and 0x00463938; the copied offsets
//   +0x28/+0x2C/+0x30 are Ego_r.pdb CTCNoiseDef's three members (Type, LifeTime, DistanceCanBeHeardFrom; 52 bytes). Same
//   shape as the landed CPhysicsDef::operator= 0x004537A5 (family G058). The catalog's "CBalverineBattleDef::operator=" is
//   propagated (CBalverineBattleDef's is 0x004E485C, a CParentDefClassBase def with members at +0x3C).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit
//   CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the float); `return *this` in eax.
#pragma optimize("s", on)
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
enum ENoiseType { };
class CTCNoiseDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    ENoiseType Type;                         // +0x28
    float LifeTime;                          // +0x2c
    float DistanceCanBeHeardFrom;            // +0x30
};
void CTCNoiseDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CTCNoiseDef*>(other);
}
