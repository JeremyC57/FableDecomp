#pragma optimize("s", on)
extern void __fastcall CTCActionUseKickChicken_Destruct(void* chicken);
void* __fastcall CTCActionUseKickChicken_ScalarDeletingDestructor(void* chicken, int, unsigned int flags)
{
    CTCActionUseKickChicken_Destruct(chicken);
    if (flags & 1)
        operator delete(chicken);
    return chicken;
}