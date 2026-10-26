#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseGiveBeer_Destruct(void* beer);
void* __fastcall CTCCarriedActionUseGiveBeer_ScalarDeletingDestructor(void* beer, int, unsigned int flags)
{
    CTCCarriedActionUseGiveBeer_Destruct(beer);
    if (flags & 1)
        operator delete(beer);
    return beer;
}