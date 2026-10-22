#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseGive_Destruct(void* give);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseGive_ScalarDeletingDestructor(void* give, int, unsigned int flags)
{
    CTCCarriedActionUseGive_Destruct(give);
    if (flags & 1)
        OperatorDelete(give);
    return give;
}