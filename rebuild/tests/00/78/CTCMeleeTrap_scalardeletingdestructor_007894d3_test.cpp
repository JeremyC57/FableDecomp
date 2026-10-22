#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCMeleeTrap_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCMeleeTrap_ScalarDeletingDestructor(void* trap, int, unsigned int flags)
{
    CTCMeleeTrap_Destruct(trap);
    if (flags & 1)
        OperatorDelete(trap);
    return trap;
}
int main()
{
    int object = 0;
    if (CTCMeleeTrap_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCMeleeTrap_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_007894d3_TEST PASS\n");
    return 0;
}