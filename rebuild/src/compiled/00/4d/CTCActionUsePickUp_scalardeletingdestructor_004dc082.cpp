#pragma optimize("s", on)
extern void __fastcall CTCActionUsePickUp_Destruct(void* pickUp);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUsePickUp_ScalarDeletingDestructor(void* pickUp, int, unsigned int flags)
{
    CTCActionUsePickUp_Destruct(pickUp);
    if (flags & 1)
        OperatorDelete(pickUp);
    return pickUp;
}