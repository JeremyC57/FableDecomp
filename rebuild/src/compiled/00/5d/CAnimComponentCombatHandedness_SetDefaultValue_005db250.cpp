// Leaf: ?SetDefaultValue@CAnimComponentCombatHandedness@@UAEXXZ
class CAnimComponentCombatHandedness { public: char pad[0x4]; int StartHandedness; int EndHandedness;
    virtual void SetDefaultValue();
};
void CAnimComponentCombatHandedness::SetDefaultValue()
{
    CAnimComponentCombatHandedness* component = this; component->StartHandedness = 2; component->EndHandedness = 0; }
// Flatten route (flatten_pilot v4, NF324): CAnimComponentCombatHandedness_SetDefaultValue -> CAnimComponentCombatHandedness::SetDefaultValue, the RTTI-placed PDB member; class model: the row's local class model.
