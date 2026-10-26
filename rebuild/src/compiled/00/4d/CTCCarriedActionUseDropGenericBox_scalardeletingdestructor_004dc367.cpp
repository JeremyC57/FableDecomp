#pragma optimize("s", on)
extern void __fastcall CTCCarriedActionUseDropGenericBox_Destruct(void* box);
void* __fastcall CTCCarriedActionUseDropGenericBox_ScalarDeletingDestructor(void* box, int, unsigned int flags)
{
    CTCCarriedActionUseDropGenericBox_Destruct(box);
    if (flags & 1)
        operator delete(box);
    return box;
}