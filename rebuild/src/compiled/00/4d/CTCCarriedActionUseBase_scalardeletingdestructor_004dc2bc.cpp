#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseBase_Destruct(void* use);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseBase_ScalarDeletingDestructor(void* use, int, unsigned int flags)
{
    CTCCarriedActionUseBase_Destruct(use);
    if (flags & 1)
        OperatorDelete(use);
    return use;
}