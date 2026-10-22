#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActivationTrigger_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActivationTrigger_ScalarDeletingDestructor(void* trigger, int, unsigned int flags)
{
    CTCActivationTrigger_Destruct(trigger);
    if (flags & 1)
        OperatorDelete(trigger);
    return trigger;
}
int main()
{
    int object = 0;
    if (CTCActivationTrigger_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActivationTrigger_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8655_TEST PASS\n");
    return 0;
}