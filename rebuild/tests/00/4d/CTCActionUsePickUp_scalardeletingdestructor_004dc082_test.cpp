#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUsePickUp_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUsePickUp_ScalarDeletingDestructor(void* pickUp, int, unsigned int flags)
{
    CTCActionUsePickUp_Destruct(pickUp);
    if (flags & 1)
        OperatorDelete(pickUp);
    return pickUp;
}
int main()
{
    int object = 0;
    if (CTCActionUsePickUp_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUsePickUp_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc082_TEST PASS\n");
    return 0;
}