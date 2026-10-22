#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CArmourDef_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CArmourDef_ScalarDeletingDestructor(void* def, int, unsigned int flags)
{
    CArmourDef_Destruct(def);
    if (flags & 1)
        OperatorDelete(def);
    return def;
}
int main()
{
    int object = 0;
    if (CArmourDef_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CArmourDef_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004585a1_TEST PASS\n");
    return 0;
}