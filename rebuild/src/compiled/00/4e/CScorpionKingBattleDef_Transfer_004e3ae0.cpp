// CScorpionKingBattleDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (NumChargesBeforeTailStrike, UnderGroundTime, TailPlungeTime, SpikeTime, SpikeDecisionTime, TailStrikePrepareTime, DamageToCauseTailStrike, ChargeDamage, ClawObstructionRadius)
// Leaf: ?Transfer@CScorpionKingBattleDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3AE0, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CScorpionKingBattleDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CScorpionKingBattleDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CScorpionKingBattleDef's members and types exactly (retail offset = Ego_r offset - 4). Family G057 (seven def Transfers of nine fields).
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
class CScorpionKingBattleDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
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
void CScorpionKingBattleDef::Transfer(CPersistContext& context)
{
    context.Transfer(NumChargesBeforeTailStrike);
    context.Transfer(UnderGroundTime);
    context.Transfer(TailPlungeTime);
    context.Transfer(SpikeTime);
    context.Transfer(SpikeDecisionTime);
    context.Transfer(TailStrikePrepareTime);
    context.Transfer(DamageToCauseTailStrike);
    context.Transfer(ChargeDamage);
    context.Transfer(ClawObstructionRadius);
}