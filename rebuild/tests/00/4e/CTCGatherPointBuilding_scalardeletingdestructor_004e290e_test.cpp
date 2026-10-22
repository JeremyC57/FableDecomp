#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCGatherPointBuilding_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCGatherPointBuilding_ScalarDeletingDestructor(void* building, int, unsigned int flags)
{
    CTCGatherPointBuilding_Destruct(building);
    if (flags & 1)
        OperatorDelete(building);
    return building;
}
int main()
{
    int object = 0;
    if (CTCGatherPointBuilding_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCGatherPointBuilding_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e290e_TEST PASS\n");
    return 0;
}