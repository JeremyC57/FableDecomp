// CTrollBattleDef::operator=(const CTrollBattleDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then GroundPoundSpeedMultiplierStart, GroundPoundSpeedMultiplierEnd, AttackPosPredictiveness, PercentageChanceToUseSecondaryShotType, SecondaryShotType, MinLaughDistance, MaxTimeBetweenKnockdownAndBoast, DeathExplosionIndex, FallDownExplosionIndex memberwise
// Leaf: ??4CTrollBattleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E47E8, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CTrollBattleDef::operator=(const CTrollBattleDef&) -- proven; evidence: its only caller is the landed CTrollBattleDef::Copy 0x004E47E3
//   (a `jmp` to it: `*this = *static_cast<const CTrollBattleDef*>(other)`); RTTI of vtable 0x01242F0C gives CTrollBattleDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CTrollBattleDef's nine members (retail offset = Ego_r offset - 4: the TLC parent def base is 0x3c bytes); FableWin.pdb name set holds
//   ??4CTrollBattleDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CParentDefClassBase::operator=" is propagated).
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
class CTrollBattleDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    float GroundPoundSpeedMultiplierStart;   // +0x3c
    float GroundPoundSpeedMultiplierEnd;     // +0x40
    float AttackPosPredictiveness;           // +0x44
    float PercentageChanceToUseSecondaryShotType; // +0x48
    long SecondaryShotType;                  // +0x4c
    float MinLaughDistance;                  // +0x50
    float MaxTimeBetweenKnockdownAndBoast;   // +0x54
    long DeathExplosionIndex;                // +0x58
    long FallDownExplosionIndex;             // +0x5c
};
void CTrollBattleDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CTrollBattleDef*>(other);
}