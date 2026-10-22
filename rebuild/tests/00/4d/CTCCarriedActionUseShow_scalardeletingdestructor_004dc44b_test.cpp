#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUseShow_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUseShow_ScalarDeletingDestructor(void* show, int, unsigned int flags)
{
    CTCCarriedActionUseShow_Destruct(show);
    if (flags & 1)
        OperatorDelete(show);
    return show;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUseShow_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseShow_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc44b_TEST PASS\n");
    return 0;
}