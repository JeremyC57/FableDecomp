#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseSpade_Destruct(void* spade);
void* __fastcall CTCCarriedActionUseSpade_ScalarDeletingDestructor(void* spade, int, unsigned int flags)
{
    CTCCarriedActionUseSpade_Destruct(spade);
    if (flags & 1)
        operator delete(spade);
    return spade;
}