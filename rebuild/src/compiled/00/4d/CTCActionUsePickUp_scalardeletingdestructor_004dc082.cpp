#pragma optimize("s", on)
extern void __fastcall CTCActionUsePickUp_Destruct(void* pickUp);
void* __fastcall CTCActionUsePickUp_ScalarDeletingDestructor(void* pickUp, int, unsigned int flags)
{
    CTCActionUsePickUp_Destruct(pickUp);
    if (flags & 1)
        operator delete(pickUp);
    return pickUp;
}