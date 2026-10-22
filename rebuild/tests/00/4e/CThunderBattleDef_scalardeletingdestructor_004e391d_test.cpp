#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CThunderBattleDef_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CThunderBattleDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CThunderBattleDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}
int main()
{
    int object = 0;
    if (CThunderBattleDef_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CThunderBattleDef_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e391d_TEST PASS\n");
    return 0;
}