#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCupboard_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCupboard_ScalarDeletingDestructor(void* cupboard, int, unsigned int flags)
{
    CTCCupboard_Destruct(cupboard);
    if (flags & 1)
        OperatorDelete(cupboard);
    return cupboard;
}
int main()
{
    int object = 0;
    if (CTCCupboard_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCupboard_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d7f5f_TEST PASS\n");
    return 0;
}