#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionDrainLife_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionDrainLife_ScalarDeletingDestructor(void* life, int, unsigned int flags)
{
    CTCShotDefinitionDrainLife_Destruct(life);
    if (flags & 1)
        OperatorDelete(life);
    return life;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionDrainLife_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionDrainLife_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004db1a6_TEST PASS\n");
    return 0;
}