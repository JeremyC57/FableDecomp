#pragma optimize("s", on)
extern void __fastcall CTCActionUseSwitch_Destruct(void* useSwitch);
void* __fastcall CTCActionUseSwitch_ScalarDeletingDestructor(void* useSwitch, int, unsigned int flags)
{
    CTCActionUseSwitch_Destruct(useSwitch);
    if (flags & 1)
        operator delete(useSwitch);
    return useSwitch;
}