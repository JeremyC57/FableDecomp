#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUseSwitch_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUseSwitch_ScalarDeletingDestructor(void* useSwitch, int, unsigned int flags)
{
    CTCActionUseSwitch_Destruct(useSwitch);
    if (flags & 1)
        OperatorDelete(useSwitch);
    return useSwitch;
}
int main()
{
    int object = 0;
    if (CTCActionUseSwitch_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUseSwitch_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc5ab_TEST PASS\n");
    return 0;
}