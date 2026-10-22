#pragma optimize("s", on)
extern void __fastcall CMeleeCombatAbilityDef_Destruct(void* def);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CMeleeCombatAbilityDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CMeleeCombatAbilityDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}