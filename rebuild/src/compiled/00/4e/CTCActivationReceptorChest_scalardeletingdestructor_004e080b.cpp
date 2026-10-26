#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorChest_Destruct(void* chest);
void* __fastcall CTCActivationReceptorChest_ScalarDeletingDestructor(void* chest, int, unsigned int flags)
{
    CTCActivationReceptorChest_Destruct(chest);
    if (flags & 1)
        operator delete(chest);
    return chest;
}