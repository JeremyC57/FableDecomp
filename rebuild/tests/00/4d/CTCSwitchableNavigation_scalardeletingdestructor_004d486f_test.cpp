#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCSwitchableNavigation_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCSwitchableNavigation_ScalarDeletingDestructor(void* navigation, int, unsigned int flags)
{
    CTCSwitchableNavigation_Destruct(navigation);
    if (flags & 1)
        OperatorDelete(navigation);
    return navigation;
}
int main()
{
    int object = 0;
    if (CTCSwitchableNavigation_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCSwitchableNavigation_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d486f_TEST PASS\n");
    return 0;
}