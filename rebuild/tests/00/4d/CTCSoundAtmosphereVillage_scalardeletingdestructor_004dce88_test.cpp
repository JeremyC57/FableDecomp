#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCSoundAtmosphereVillage_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCSoundAtmosphereVillage_ScalarDeletingDestructor(void* village, int, unsigned int flags)
{
    CTCSoundAtmosphereVillage_Destruct(village);
    if (flags & 1)
        OperatorDelete(village);
    return village;
}
int main()
{
    int object = 0;
    if (CTCSoundAtmosphereVillage_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCSoundAtmosphereVillage_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dce88_TEST PASS\n");
    return 0;
}