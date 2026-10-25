// CLightningDef::operator=(const CLightningDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then Range, LifeTime, MaxNumberOfBranches, NeedLOSToBranch, DamagePeriod, DamagePerCycle, NodeDefIndex memberwise
// Leaf: ??4CLightningDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00461ED8, 63 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CLightningDef::operator=(const CLightningDef&) -- proven; evidence: its only caller is the landed CLightningDef::Copy 0x00461ED3
//   (a `jmp` to it: `*this = *static_cast<const CLightningDef*>(other)`); RTTI of vtable 0x01234C24 gives CLightningDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CLightningDef's seven members (retail offset = Ego_r offset - 4: the TLC parent def base is 0x3c bytes); FableWin.pdb name set holds
//   ??4CLightningDef@@QAEAAV0@ABV0@@Z. Family G058 (seven def copy assignments; catalogue "CParentDefClassBase::operator=" is propagated).
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) 0x00431ED0 (bound under that name).
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CParentDefClassBase : public CDefClassBase {        // retail 0x3C bytes (InstantiationName, SubDefInfoMap, flag)
public:
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
    unsigned char _body2[0x3c - 0x28];
};
class CLightningDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    float Range;                             // +0x3c
    float LifeTime;                          // +0x40
    long MaxNumberOfBranches;                // +0x44
    bool NeedLOSToBranch;                    // +0x48
    float DamagePeriod;                      // +0x4c
    float DamagePerCycle;                    // +0x50
    long NodeDefIndex;                       // +0x54
};
void CLightningDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CLightningDef*>(other);
}