#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCSingleUserActionPoints_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCSingleUserActionPoints_ScalarDeletingDestructor(void* points, int, unsigned int flags)
{
    CTCSingleUserActionPoints_Destruct(points);
    if (flags & 1)
        OperatorDelete(points);
    return points;
}
int main()
{
    int object = 0;
    if (CTCSingleUserActionPoints_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCSingleUserActionPoints_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e037c_TEST PASS\n");
    return 0;
}