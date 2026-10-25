// CSummonDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (CreatureToSummon, CreatureFamilyToSummon, NumberToSummon, SummonRadius, MySummonLimit, MinimumSummon, TimeBetweenSummons, SummonEffect, SummoneeEffect)
// Leaf: ?Transfer@CSummonDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF264, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CSummonDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CSummonDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CSummonDef's members and types exactly (retail offset = Ego_r offset). Family G057 (seven def Transfers of nine fields).
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
class CSummonDef {                                              // retail fields from +0x28
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                    // +0x04 def base body
    long CreatureToSummon;                   // +0x28
    long CreatureFamilyToSummon;             // +0x2c
    long NumberToSummon;                     // +0x30
    float SummonRadius;                      // +0x34
    long MySummonLimit;                      // +0x38
    long MinimumSummon;                      // +0x3c
    float TimeBetweenSummons;                // +0x40
    long SummonEffect;                       // +0x44
    long SummoneeEffect;                     // +0x48
};
void CSummonDef::Transfer(CPersistContext& context)
{
    context.Transfer(CreatureToSummon);
    context.Transfer(CreatureFamilyToSummon);
    context.Transfer(NumberToSummon);
    context.Transfer(SummonRadius);
    context.Transfer(MySummonLimit);
    context.Transfer(MinimumSummon);
    context.Transfer(TimeBetweenSummons);
    context.Transfer(SummonEffect);
    context.Transfer(SummoneeEffect);
}