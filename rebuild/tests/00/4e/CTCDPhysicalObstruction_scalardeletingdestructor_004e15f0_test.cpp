#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDPhysicalObstruction_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDPhysicalObstruction_ScalarDeletingDestructor(void* obstruction, int, unsigned int flags)
{
    CTCDPhysicalObstruction_Destruct(obstruction);
    if (flags & 1)
        OperatorDelete(obstruction);
    return obstruction;
}
int main()
{
    int object = 0;
    if (CTCDPhysicalObstruction_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDPhysicalObstruction_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e15f0_TEST PASS\n");
    return 0;
}