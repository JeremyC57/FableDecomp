#pragma optimize("s", on)
extern void __fastcall CMeleeCombatAbilityDef_Destruct(void* def);
void* __fastcall CMeleeCombatAbilityDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CMeleeCombatAbilityDef_Destruct(def);
    if (flags & 1)
        operator delete(def);
    return def;
}