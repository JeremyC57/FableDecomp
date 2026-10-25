// CAugmentationDef::Transfer(CPersistContext& context) -- persist the def's fields one by one (Name, Type, InventoryGraphic, DamageMultiplier, ExperienceMultiplier, FrequencyToAddHealthInSeconds, HealthToAddPerIncrease, FrequencyToAddStaminaInSeconds, StaminaToAddPerIncrease)
// Leaf: ?Transfer@CAugmentationDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00452CDD, 112 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CAugmentationDef::Transfer(CPersistContext&) -- proven; evidence: the catalogue row is this virtual (manifest
//   ?Transfer@CAugmentationDef@@UAEXAAVCPersistContext@@@Z, Ego_r.pdb name set); the nine transferred offsets and their callee
//   overloads match Ego_r.pdb CAugmentationDef's members and types exactly (retail offset = Ego_r offset). Family G057 (seven def Transfers of nine fields).
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020, Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(bool&) 0x0043314A (each bound under that name by landed defs); member-template instances Transfer<EObjectAugmentationType> 0x00452D4D.
// Shape: context in edi and this in esi for the whole body; the last field's address is `add esi, N` in place.
#pragma optimize("s", on)
enum EObjectAugmentationType { EObjectAugmentationType_Force32 = 0x7fffffff };
class CPersistContext {
public:
    void Transfer(unsigned long& value);                   // 0x00431020
    void Transfer(long& value);                            // 0x00431102
    void Transfer(float& value);                           // 0x00431061
    void Transfer(bool& value);                            // 0x0043314A
    template <class T> void Transfer(T& value);            // enum / container instances in this TU
};
class CAugmentationDef {                                              // retail fields from +0x28
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x24];                    // +0x04 def base body
    unsigned long Name;                      // +0x28
    EObjectAugmentationType Type;            // +0x2c
    unsigned long InventoryGraphic;          // +0x30
    float DamageMultiplier;                  // +0x34
    float ExperienceMultiplier;              // +0x38
    float FrequencyToAddHealthInSeconds;     // +0x3c
    long HealthToAddPerIncrease;             // +0x40
    float FrequencyToAddStaminaInSeconds;    // +0x44
    long StaminaToAddPerIncrease;            // +0x48
};
void CAugmentationDef::Transfer(CPersistContext& context)
{
    context.Transfer(Name);
    context.Transfer(Type);
    context.Transfer(InventoryGraphic);
    context.Transfer(DamageMultiplier);
    context.Transfer(ExperienceMultiplier);
    context.Transfer(FrequencyToAddHealthInSeconds);
    context.Transfer(HealthToAddPerIncrease);
    context.Transfer(FrequencyToAddStaminaInSeconds);
    context.Transfer(StaminaToAddPerIncrease);
}