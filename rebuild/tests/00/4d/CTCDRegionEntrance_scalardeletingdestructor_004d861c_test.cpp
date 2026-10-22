#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDRegionEntrance_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDRegionEntrance_ScalarDeletingDestructor(void* entrance, int, unsigned int flags)
{
    CTCDRegionEntrance_Destruct(entrance);
    if (flags & 1)
        OperatorDelete(entrance);
    return entrance;
}
int main()
{
    int object = 0;
    if (CTCDRegionEntrance_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDRegionEntrance_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d861c_TEST PASS\n");
    return 0;
}