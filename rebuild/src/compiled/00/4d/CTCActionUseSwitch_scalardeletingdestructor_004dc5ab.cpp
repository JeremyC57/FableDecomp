#pragma optimize("s", on)
extern void __fastcall CTCActionUseSwitch_Destruct(void* useSwitch);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActionUseSwitch_ScalarDeletingDestructor(void* useSwitch, int, unsigned int flags)
{
    CTCActionUseSwitch_Destruct(useSwitch);
    if (flags & 1)
        OperatorDelete(useSwitch);
    return useSwitch;
}