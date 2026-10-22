#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCAvoidRegionExit_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCAvoidRegionExit_ScalarDeletingDestructor(void* exit, int, unsigned int flags)
{
    CTCAvoidRegionExit_Destruct(exit);
    if (flags & 1)
        OperatorDelete(exit);
    return exit;
}
int main()
{
    int object = 0;
    if (CTCAvoidRegionExit_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCAvoidRegionExit_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d97b2_TEST PASS\n");
    return 0;
}