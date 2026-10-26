#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorDoor_Destruct(void* door);
void* __fastcall CTCActivationReceptorDoor_ScalarDeletingDestructor(void* door, int, unsigned int flags)
{
    CTCActivationReceptorDoor_Destruct(door);
    if (flags & 1)
        operator delete(door);
    return door;
}