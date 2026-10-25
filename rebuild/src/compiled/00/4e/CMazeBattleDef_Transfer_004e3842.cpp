// CMazeBattleDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (ShieldUpTime, LightningDuration, MinThrowShotRange, EnflameRange, LightningRange, TutorialNumGetHits, MinTimeBetweenHeals, HealSpellLevel, TutorialTeleportTime)
// Leaf: ?Transfer@CMazeBattleDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3842, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CMazeBattleDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CMazeBattleDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CMazeBattleDef's members and types exactly (retail offset = Ego_r offset - 4). Family G057 (seven def Transfers of nine fields).
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
class CMazeBattleDef {                                              // retail fields from +0x3c
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                    // +0x04 def base body
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
void CMazeBattleDef::Transfer(CPersistContext& context)
{
    context.Transfer(ShieldUpTime);
    context.Transfer(LightningDuration);
    context.Transfer(MinThrowShotRange);
    context.Transfer(EnflameRange);
    context.Transfer(LightningRange);
    context.Transfer(TutorialNumGetHits);
    context.Transfer(MinTimeBetweenHeals);
    context.Transfer(HealSpellLevel);
    context.Transfer(TutorialTeleportTime);
}