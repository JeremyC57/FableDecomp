#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseBase_Destruct(void* use);
void* __fastcall CTCCarriedActionUseBase_ScalarDeletingDestructor(void* use, int, unsigned int flags)
{
    CTCCarriedActionUseBase_Destruct(use);
    if (flags & 1)
        operator delete(use);
    return use;
}