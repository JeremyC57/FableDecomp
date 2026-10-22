#pragma optimize("s", on)
extern void __fastcall CTCIdleScheduler_Destruct(void* scheduler);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCIdleScheduler_ScalarDeletingDestructor(void* scheduler, int, unsigned int flags)
{
    CTCIdleScheduler_Destruct(scheduler);
    if (flags & 1)
        OperatorDelete(scheduler);
    return scheduler;
}