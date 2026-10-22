#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CFireEffectCreationSet_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CFireEffectCreationSet_ScalarDeletingDestructor(void* set, int, unsigned int flags)
{
    CFireEffectCreationSet_Destruct(set);
    if (flags & 1)
        OperatorDelete(set);
    return set;
}
int main()
{
    int object = 0;
    if (CFireEffectCreationSet_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CFireEffectCreationSet_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e0a32_TEST PASS\n");
    return 0;
}