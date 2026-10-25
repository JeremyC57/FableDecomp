// CAugmentationDef::operator=(const CAugmentationDef& other) -- compiler-generated copy assignment: CSubDefClassBase::operator=, then Name, Type, InventoryGraphic, DamageMultiplier, ExperienceMultiplier, FrequencyToAddHealthInSeconds, HealthToAddPerIncrease, FrequencyToAddStaminaInSeconds, StaminaToAddPerIncrease memberwise
// Leaf: ??4CAugmentationDef@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454088, 75 bytes (size-optimised def TU, odd entry), thiscall, `ret 4`.
// Identity: CAugmentationDef::operator=(const CAugmentationDef&) -- proven; evidence: its only caller is the landed CAugmentationDef::Copy 0x00454083
//   (a `jmp` to it: `*this = *static_cast<const CAugmentationDef*>(other)`); RTTI of vtable 0x0123287C gives CAugmentationDef : CSubDefClassBase : CDefClassBase : CDefPointeeBase; the
//   copied offsets and widths are Ego_r.pdb CAugmentationDef's nine members (retail offset = Ego_r offset); FableWin.pdb name set holds
//   ??4CAugmentationDef@@QAEAAV0@ABV0@@Z. Family G059 (seven def copy assignments of nine members; catalogue "CThingBuildingDef::operator=" is propagated).
// Callees: CDefClassBase::operator=(const CDefClassBase&) 0x00431F10 (bound under that name), reached through the inline implicit CSubDefClassBase::operator=.
// Shape: implicit operator= copies each scalar member with integer moves (no x87 for the floats); `return *this` in eax.
#pragma optimize("s", on)
enum EObjectAugmentationType { EObjectAugmentationType_Force32 = 0x7fffffff };
class CDefClassBase {                                      // retail 0x25 bytes (+ tail padding)
public:
    virtual ~CDefClassBase();
    CDefClassBase& operator=(const CDefClassBase& other);  // 0x00431F10
    unsigned char _body[0x25 - 4];
};
class CSubDefClassBase : public CDefClassBase {           // no members: its implicit operator= inlines to the base call
};
class CAugmentationDef : public CSubDefClassBase {
public:
    virtual void Copy(const CDefClassBase* other);
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
void CAugmentationDef::Copy(const CDefClassBase* other)
{
    *this = *static_cast<const CAugmentationDef*>(other);
}