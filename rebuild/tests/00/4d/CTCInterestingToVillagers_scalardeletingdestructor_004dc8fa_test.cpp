#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCInterestingToVillagers_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCInterestingToVillagers_ScalarDeletingDestructor(void* villagers, int, unsigned int flags)
{
    CTCInterestingToVillagers_Destruct(villagers);
    if (flags & 1)
        OperatorDelete(villagers);
    return villagers;
}
int main()
{
    int object = 0;
    if (CTCInterestingToVillagers_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCInterestingToVillagers_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc8fa_TEST PASS\n");
    return 0;
}