#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCInventoryStats_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCInventoryStats_ScalarDeletingDestructor(void* stats, int, unsigned int flags)
{
    CTCInventoryStats_Destruct(stats);
    if (flags & 1)
        OperatorDelete(stats);
    return stats;
}
int main()
{
    int object = 0;
    if (CTCInventoryStats_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCInventoryStats_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_005d88fa_TEST PASS\n");
    return 0;
}