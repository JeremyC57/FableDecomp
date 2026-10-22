#pragma optimize("s", on)
extern void __fastcall CMeleeCombatKnockdownEffects_Destruct(void* effects);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CMeleeCombatKnockdownEffects_ScalarDeletingDestructor(void* effects, int, unsigned int flags)
{
    CMeleeCombatKnockdownEffects_Destruct(effects);
    if (flags & 1)
        OperatorDelete(effects);
    return effects;
}