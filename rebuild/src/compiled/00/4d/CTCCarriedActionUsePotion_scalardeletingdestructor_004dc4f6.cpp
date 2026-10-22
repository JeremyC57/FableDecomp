#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUsePotion_Destruct(void* potion);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUsePotion_ScalarDeletingDestructor(void* potion, int, unsigned int flags)
{
    CTCCarriedActionUsePotion_Destruct(potion);
    if (flags & 1)
        OperatorDelete(potion);
    return potion;
}