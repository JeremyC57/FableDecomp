#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActivationReceptorChest_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActivationReceptorChest_ScalarDeletingDestructor(void* chest, int, unsigned int flags)
{
    CTCActivationReceptorChest_Destruct(chest);
    if (flags & 1)
        OperatorDelete(chest);
    return chest;
}
int main()
{
    int object = 0;
    if (CTCActivationReceptorChest_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActivationReceptorChest_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e080b_TEST PASS\n");
    return 0;
}