#pragma optimize("s", on)
extern void __fastcall CTCActionUsePickUpGenericBox_Destruct(void* box);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUsePickUpGenericBox_ScalarDeletingDestructor(void* box, int, unsigned int flags)
{
    CTCActionUsePickUpGenericBox_Destruct(box);
    if (flags & 1)
        OperatorDelete(box);
    return box;
}