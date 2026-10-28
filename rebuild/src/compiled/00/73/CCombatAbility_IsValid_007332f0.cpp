// CCombatAbility::IsValid(const CCombatAbilityData& data, unsigned long* randomSeed) const -- an active ability stays valid; an idle, usable one passes when the next value of the caller's random seed (mod 100) is below the data's percentage chance
// Leaf: ?IsValid@CCombatAbility@@QBE_NABVCCombatAbilityData@@PAK@Z
// Retail: Fable.exe @ 0x007332F0, 71 bytes, thiscall `ret 8`.
// Identity (proven): the manifest names the row CCombatAbility::IsValid with (CCombatAbilityData*, unsigned long*); both
//   PDBs have ?IsValid@CCombatAbility@@QBE_NABVCCombatAbilityData@@PAK@Z and Ego_r.pdb CCombatAbility is
//   {RepeatTimer +0, ActiveTimer +4}, which the body reads; CCombatAbilityData::PercentageChance is +4 (vptr first).
// Body: IsActive() (inline, ActiveTimer >= 0; FableWin.pdb ?IsActive@CCombatAbility@@QBE_NXZ) -> true; else when
//   IsUsable() (inline, RepeatTimer == -1; ?IsUsable@CCombatAbility@@QBE_NXZ): step the seed (seed*0x24A1 + 0x24DF),
//   store GFROR13(seed) (0x00497890, ?GFROR13@@YIJK@Z, __fastcall) back and compare it mod 100 unsigned (`div`, `jae`)
//   with the chance; otherwise false.
long __fastcall GFROR13(unsigned long value);           // 0x00497890
class CCombatAbilityData {
public:
    virtual void v00();
    long PercentageChance;                               // +0x04
};
class CCombatAbility {
public:
    long RepeatTimer;                                    // +0x00
    long ActiveTimer;                                    // +0x04
    bool IsActive() const { return ActiveTimer >= 0; }
    bool IsUsable() const { return RepeatTimer == -1; }
    bool IsValid(const CCombatAbilityData& data, unsigned long* randomSeed) const;
};
bool CCombatAbility::IsValid(const CCombatAbilityData& data, unsigned long* randomSeed) const
{
    if (IsActive())
        return true;
    if (IsUsable())
    {
        *randomSeed = *randomSeed * 0x24A1 + 0x24DF;
        *randomSeed = GFROR13(*randomSeed);
        if (*randomSeed % 100 < (unsigned long)data.PercentageChance)
            return true;
    }
    return false;
}