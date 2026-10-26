#pragma optimize("s", on)
extern void __fastcall CTCActionUseBase_Destruct(void* use);
void* __fastcall CTCActionUseBase_ScalarDeletingDestructor(void* use, int, unsigned int flags)
{
    CTCActionUseBase_Destruct(use);
    if (flags & 1)
        operator delete(use);
    return use;
}