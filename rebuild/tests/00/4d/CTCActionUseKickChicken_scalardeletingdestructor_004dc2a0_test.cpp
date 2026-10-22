#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUseKickChicken_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUseKickChicken_ScalarDeletingDestructor(void* chicken, int, unsigned int flags)
{
    CTCActionUseKickChicken_Destruct(chicken);
    if (flags & 1)
        OperatorDelete(chicken);
    return chicken;
}
int main()
{
    int object = 0;
    if (CTCActionUseKickChicken_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUseKickChicken_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc2a0_TEST PASS\n");
    return 0;
}