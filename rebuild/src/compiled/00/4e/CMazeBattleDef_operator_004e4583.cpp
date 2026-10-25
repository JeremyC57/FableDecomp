// CMazeBattleDef::operator=(const CMazeBattleDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then ShieldUpTime, LightningDuration, MinThrowShotRange, LightningRange, EnflameRange, TutorialNumGetHits, MinTimeBetweenHeals, HealSpellLevel, TutorialTeleportTime memberwise
// Leaf: ??4CMazeBattleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4583, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CMazeBattleDef::operator=(const CMazeBattleDef&) -- proven; evidence: its only caller is the landed CMazeBattleDef::Copy 0x004E457E
//   (a `jmp` to it: `*this = *static_cast<const CMazeBattleDef*>(other)`); RTTI of vtable 0x01242C9C gives CMazeBattleDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CMazeBattleDef's nine members (retail offset = Ego_r offset - 4: the TLC parent def base is 0x3c bytes); FableWin.pdb name set holds
//   ??4CMazeBattleDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CParentDefClassBase::operator=_9fields" is propagated).
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
class CMazeBattleDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    float ShieldUpTime;                      // +0x3c
    float LightningDuration;                 // +0x40
    float MinThrowShotRange;                 // +0x44
    float LightningRange;                    // +0x48
    float EnflameRange;                      // +0x4c
    long TutorialNumGetHits;                 // +0x50
    float MinTimeBetweenHeals;               // +0x54
    long HealSpellLevel;                     // +0x58
    float TutorialTeleportTime;              // +0x5c
};
void CMazeBattleDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CMazeBattleDef*>(other);
}