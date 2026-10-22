#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCBoastingPodium_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCBoastingPodium_ScalarDeletingDestructor(void* podium, int, unsigned int flags)
{
    CTCBoastingPodium_Destruct(podium);
    if (flags & 1)
        OperatorDelete(podium);
    return podium;
}
int main()
{
    int object = 0;
    if (CTCBoastingPodium_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCBoastingPodium_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e755c_TEST PASS\n");
    return 0;
}