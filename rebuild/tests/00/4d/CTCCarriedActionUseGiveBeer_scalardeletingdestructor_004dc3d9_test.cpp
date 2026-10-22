#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUseGiveBeer_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUseGiveBeer_ScalarDeletingDestructor(void* beer, int, unsigned int flags)
{
    CTCCarriedActionUseGiveBeer_Destruct(beer);
    if (flags & 1)
        OperatorDelete(beer);
    return beer;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUseGiveBeer_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseGiveBeer_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc3d9_TEST PASS\n");
    return 0;
}