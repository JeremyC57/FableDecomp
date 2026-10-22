#pragma optimize("s", on)
extern void __fastcall CTCActivationTrigger_Destruct(void* trigger);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCActivationTrigger_ScalarDeletingDestructor(void* trigger, int, unsigned int flags)
{
    CTCActivationTrigger_Destruct(trigger);
    if (flags & 1)
        OperatorDelete(trigger);
    return trigger;
}