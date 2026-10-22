#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCInventoryTrade_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCInventoryTrade_ScalarDeletingDestructor(void* trade, int, unsigned int flags)
{
    CTCInventoryTrade_Destruct(trade);
    if (flags & 1)
        OperatorDelete(trade);
    return trade;
}
int main()
{
    int object = 0;
    if (CTCInventoryTrade_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCInventoryTrade_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_005b320f_TEST PASS\n");
    return 0;
}