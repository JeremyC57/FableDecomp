#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CVillageTaskPickCrops_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CVillageTaskPickCrops_ScalarDeletingDestructor(void* task, int, unsigned int flags)
{
    CVillageTaskPickCrops_Destruct(task);
    if (flags & 1)
        OperatorDelete(task);
    return task;
}
int main()
{
    int object = 0;
    if (CVillageTaskPickCrops_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CVillageTaskPickCrops_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_0065ef35_TEST PASS\n");
    return 0;
}