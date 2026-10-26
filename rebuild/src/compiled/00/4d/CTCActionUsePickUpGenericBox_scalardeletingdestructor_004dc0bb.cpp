#pragma optimize("s", on)
extern void __fastcall CTCActionUsePickUpGenericBox_Destruct(void* box);
void* __fastcall CTCActionUsePickUpGenericBox_ScalarDeletingDestructor(void* box, int, unsigned int flags)
{
    CTCActionUsePickUpGenericBox_Destruct(box);
    if (flags & 1)
        operator delete(box);
    return box;
}