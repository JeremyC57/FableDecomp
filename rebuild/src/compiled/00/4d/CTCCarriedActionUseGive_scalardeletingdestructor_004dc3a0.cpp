#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseGive_Destruct(void* give);
void* __fastcall CTCCarriedActionUseGive_ScalarDeletingDestructor(void* give, int, unsigned int flags)
{
    CTCCarriedActionUseGive_Destruct(give);
    if (flags & 1)
        operator delete(give);
    return give;
}