#pragma optimize("s", on)
extern void __fastcall CTCActionUseChest_Destruct(void* chest);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseChest_ScalarDeletingDestructor(void* chest, int, unsigned int flags)
{
    CTCActionUseChest_Destruct(chest);
    if (flags & 1)
        OperatorDelete(chest);
    return chest;
}