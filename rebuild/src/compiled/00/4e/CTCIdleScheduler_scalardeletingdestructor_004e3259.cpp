#pragma optimize("s", on)
extern void __fastcall CTCIdleScheduler_Destruct(void* scheduler);
void* __fastcall CTCIdleScheduler_ScalarDeletingDestructor(void* scheduler, int, unsigned int flags)
{
    CTCIdleScheduler_Destruct(scheduler);
    if (flags & 1)
        operator delete(scheduler);
    return scheduler;
}