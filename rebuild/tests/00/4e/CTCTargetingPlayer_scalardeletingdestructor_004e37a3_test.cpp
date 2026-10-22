#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCTargetingPlayer_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCTargetingPlayer_ScalarDeletingDestructor(void* player, int, unsigned int flags)
{
    CTCTargetingPlayer_Destruct(player);
    if (flags & 1)
        OperatorDelete(player);
    return player;
}
int main()
{
    int object = 0;
    if (CTCTargetingPlayer_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCTargetingPlayer_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e37a3_TEST PASS\n");
    return 0;
}