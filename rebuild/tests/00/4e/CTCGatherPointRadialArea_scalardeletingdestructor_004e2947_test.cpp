#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCGatherPointRadialArea_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCGatherPointRadialArea_ScalarDeletingDestructor(void* area, int, unsigned int flags)
{
    CTCGatherPointRadialArea_Destruct(area);
    if (flags & 1)
        OperatorDelete(area);
    return area;
}
int main()
{
    int object = 0;
    if (CTCGatherPointRadialArea_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCGatherPointRadialArea_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e2947_TEST PASS\n");
    return 0;
}