#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCIdleScheduler_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCIdleScheduler_ScalarDeletingDestructor(void* scheduler, int, unsigned int flags)
{
    CTCIdleScheduler_Destruct(scheduler);
    if (flags & 1)
        OperatorDelete(scheduler);
    return scheduler;
}
int main()
{
    int object = 0;
    if (CTCIdleScheduler_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCIdleScheduler_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e3259_TEST PASS\n");
    return 0;
}