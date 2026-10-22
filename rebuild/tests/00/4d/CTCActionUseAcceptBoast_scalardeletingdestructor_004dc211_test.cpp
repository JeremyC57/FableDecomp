#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUseAcceptBoast_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUseAcceptBoast_ScalarDeletingDestructor(void* boast, int, unsigned int flags)
{
    CTCActionUseAcceptBoast_Destruct(boast);
    if (flags & 1)
        OperatorDelete(boast);
    return boast;
}
int main()
{
    int object = 0;
    if (CTCActionUseAcceptBoast_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUseAcceptBoast_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc211_TEST PASS\n");
    return 0;
}