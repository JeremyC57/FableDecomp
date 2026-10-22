#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseAbility_Destruct(void* ability);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseAbility_ScalarDeletingDestructor(void* ability, int, unsigned int flags)
{
    CTCCarriedActionUseAbility_Destruct(ability);
    if (flags & 1)
        OperatorDelete(ability);
    return ability;
}