// CScorpionKingBattleDef::operator=(const CScorpionKingBattleDef& other) -- compiler-generated copy assignment: CParentDefClassBase::operator=, then NumChargesBeforeTailStrike, UnderGroundTime, TailPlungeTime, SpikeTime, SpikeDecisionTime, TailStrikePrepareTime, DamageToCauseTailStrike, ChargeDamage, ClawObstructionRadius memberwise
// Leaf: ??4CScorpionKingBattleDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E4774, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CScorpionKingBattleDef::operator=(const CScorpionKingBattleDef&) -- proven; evidence: its only caller is the landed CScorpionKingBattleDef::Copy 0x004E476F
//   (a `jmp` to it: `*this = *static_cast<const CScorpionKingBattleDef*>(other)`); RTTI of vtable 0x01242EA4 gives CScorpionKingBattleDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CScorpionKingBattleDef's nine members (retail offset = Ego_r offset - 4: the TLC parent def base is 0x3c bytes); FableWin.pdb name set holds
//   ??4CScorpionKingBattleDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CParentDefClassBase::operator=_9fields_alt" is propagated).
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
class CScorpionKingBattleDef : public CParentDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
    long NumChargesBeforeTailStrike;         // +0x3c
    float UnderGroundTime;                   // +0x40
    float TailPlungeTime;                    // +0x44
    float SpikeTime;                         // +0x48
    float SpikeDecisionTime;                 // +0x4c
    float TailStrikePrepareTime;             // +0x50
    float DamageToCauseTailStrike;           // +0x54
    float ChargeDamage;                      // +0x58
    float ClawObstructionRadius;             // +0x5c
};
void CScorpionKingBattleDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CScorpionKingBattleDef*>(other);
}