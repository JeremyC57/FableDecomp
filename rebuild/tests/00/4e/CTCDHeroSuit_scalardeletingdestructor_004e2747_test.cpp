#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDHeroSuit_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDHeroSuit_ScalarDeletingDestructor(void* suit, int, unsigned int flags)
{
    CTCDHeroSuit_Destruct(suit);
    if (flags & 1)
        OperatorDelete(suit);
    return suit;
}
int main()
{
    int object = 0;
    if (CTCDHeroSuit_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDHeroSuit_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e2747_TEST PASS\n");
    return 0;
}