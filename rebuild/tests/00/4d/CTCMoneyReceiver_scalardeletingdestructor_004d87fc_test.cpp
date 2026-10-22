#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCMoneyReceiver_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCMoneyReceiver_ScalarDeletingDestructor(void* receiver, int, unsigned int flags)
{
    CTCMoneyReceiver_Destruct(receiver);
    if (flags & 1)
        OperatorDelete(receiver);
    return receiver;
}
int main()
{
    int object = 0;
    if (CTCMoneyReceiver_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCMoneyReceiver_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d87fc_TEST PASS\n");
    return 0;
}