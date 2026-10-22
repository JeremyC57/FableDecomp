#pragma optimize("s", on)
extern void __fastcall CTCActivationReceptorChest_Destruct(void* chest);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActivationReceptorChest_ScalarDeletingDestructor(void* chest, int, unsigned int flags)
{
    CTCActivationReceptorChest_Destruct(chest);
    if (flags & 1)
        OperatorDelete(chest);
    return chest;
}