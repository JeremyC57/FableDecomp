#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseGiveBeer_Destruct(void* beer);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseGiveBeer_ScalarDeletingDestructor(void* beer, int, unsigned int flags)
{
    CTCCarriedActionUseGiveBeer_Destruct(beer);
    if (flags & 1)
        OperatorDelete(beer);
    return beer;
}