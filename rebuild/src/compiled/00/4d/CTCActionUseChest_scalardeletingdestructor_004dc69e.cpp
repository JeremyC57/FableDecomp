#pragma optimize("s", on)
extern void __fastcall CTCActionUseChest_Destruct(void* chest);
void* __fastcall CTCActionUseChest_ScalarDeletingDestructor(void* chest, int, unsigned int flags)
{
    CTCActionUseChest_Destruct(chest);
    if (flags & 1)
        operator delete(chest);
    return chest;
}