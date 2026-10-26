#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUsePotion_Destruct(void* potion);
void* __fastcall CTCCarriedActionUsePotion_ScalarDeletingDestructor(void* potion, int, unsigned int flags)
{
    CTCCarriedActionUsePotion_Destruct(potion);
    if (flags & 1)
        operator delete(potion);
    return potion;
}