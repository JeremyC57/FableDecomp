#pragma optimize("s", on)
extern void __fastcall CTCActionUseSearch_Destruct(void* search);
void* __fastcall CTCActionUseSearch_ScalarDeletingDestructor(void* search, int, unsigned int flags)
{
    CTCActionUseSearch_Destruct(search);
    if (flags & 1)
        operator delete(search);
    return search;
}