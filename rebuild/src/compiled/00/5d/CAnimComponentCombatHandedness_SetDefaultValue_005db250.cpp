// Leaf: ?CAnimComponentCombatHandedness_SetDefaultValue@@YIXPAVCAnimComponentCombatHandedness@@@Z
class CAnimComponentCombatHandedness { public: char pad[8]; int StartHandedness; int EndHandedness; };
void __fastcall CAnimComponentCombatHandedness_SetDefaultValue(CAnimComponentCombatHandedness* component){ component->StartHandedness = 2; component->EndHandedness = 0; }