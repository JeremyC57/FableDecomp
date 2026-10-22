#pragma optimize("s", on)
extern void __fastcall CTCActionUseBase_Destruct(void* use);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseBase_ScalarDeletingDestructor(void* use, int, unsigned int flags)
{
    CTCActionUseBase_Destruct(use);
    if (flags & 1)
        OperatorDelete(use);
    return use;
}