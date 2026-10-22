#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCPreCalculatedNavigationRoute_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCPreCalculatedNavigationRoute_ScalarDeletingDestructor(void* route, int, unsigned int flags)
{
    CTCPreCalculatedNavigationRoute_Destruct(route);
    if (flags & 1)
        OperatorDelete(route);
    return route;
}
int main()
{
    int object = 0;
    if (CTCPreCalculatedNavigationRoute_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCPreCalculatedNavigationRoute_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e2bab_TEST PASS\n");
    return 0;
}