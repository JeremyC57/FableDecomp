#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDExplosiveTrail_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDExplosiveTrail_ScalarDeletingDestructor(void* trail, int, unsigned int flags)
{
    CTCDExplosiveTrail_Destruct(trail);
    if (flags & 1)
        OperatorDelete(trail);
    return trail;
}
int main()
{
    int object = 0;
    if (CTCDExplosiveTrail_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDExplosiveTrail_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e4029_TEST PASS\n");
    return 0;
}