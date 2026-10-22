#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUseBlowHorn_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUseBlowHorn_ScalarDeletingDestructor(void* horn, int, unsigned int flags)
{
    CTCActionUseBlowHorn_Destruct(horn);
    if (flags & 1)
        OperatorDelete(horn);
    return horn;
}
int main()
{
    int object = 0;
    if (CTCActionUseBlowHorn_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUseBlowHorn_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc19f_TEST PASS\n");
    return 0;
}