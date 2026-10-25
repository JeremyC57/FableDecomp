// CTrollBattleDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (GroundPoundSpeedMultiplierStart, GroundPoundSpeedMultiplierEnd, AttackPosPredictiveness, PercentageChanceToUseSecondaryShotType, SecondaryShotType, MinLaughDistance, MaxTimeBetweenKnockdownAndBoast, FallDownExplosionIndex, DeathExplosionIndex)
// Leaf: ?Transfer@CTrollBattleDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3B82, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CTrollBattleDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CTrollBattleDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CTrollBattleDef's members and types exactly (retail offset = Ego_r offset - 4). Family G057 (seven def Transfers of nine fields).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs).
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / container instances in this TU
};
class CTrollBattleDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
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
void CTrollBattleDef::Transfer(CPersistContext& context)
{
    context.Transfer(GroundPoundSpeedMultiplierStart);
    context.Transfer(GroundPoundSpeedMultiplierEnd);
    context.Transfer(AttackPosPredictiveness);
    context.Transfer(PercentageChanceToUseSecondaryShotType);
    context.Transfer(SecondaryShotType);
    context.Transfer(MinLaughDistance);
    context.Transfer(MaxTimeBetweenKnockdownAndBoast);
    context.Transfer(FallDownExplosionIndex);
    context.Transfer(DeathExplosionIndex);
}