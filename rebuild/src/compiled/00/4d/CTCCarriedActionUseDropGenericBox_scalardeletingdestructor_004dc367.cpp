#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseDropGenericBox_Destruct(void* box);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCCarriedActionUseDropGenericBox_ScalarDeletingDestructor(void* box, int, unsigned int flags)
{
    CTCCarriedActionUseDropGenericBox_Destruct(box);
    if (flags & 1)
        OperatorDelete(box);
    return box;
}