#pragma optimize("s", on)
extern void __fastcall CTCActionUseSearch_Destruct(void* search);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseSearch_ScalarDeletingDestructor(void* search, int, unsigned int flags)
{
    CTCActionUseSearch_Destruct(search);
    if (flags & 1)
        OperatorDelete(search);
    return search;
}