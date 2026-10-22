#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorDoor_Destruct(void* door);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActivationReceptorDoor_ScalarDeletingDestructor(void* door, int, unsigned int flags)
{
    CTCActivationReceptorDoor_Destruct(door);
    if (flags & 1)
        OperatorDelete(door);
    return door;
}