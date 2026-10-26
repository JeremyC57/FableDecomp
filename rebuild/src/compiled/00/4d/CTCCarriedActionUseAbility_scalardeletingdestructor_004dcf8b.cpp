#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseAbility_Destruct(void* ability);
void* __fastcall CTCCarriedActionUseAbility_ScalarDeletingDestructor(void* ability, int, unsigned int flags)
{
    CTCCarriedActionUseAbility_Destruct(ability);
    if (flags & 1)
        operator delete(ability);
    return ability;
}