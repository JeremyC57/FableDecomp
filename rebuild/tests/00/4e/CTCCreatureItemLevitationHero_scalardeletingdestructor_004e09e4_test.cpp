#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCreatureItemLevitationHero_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCreatureItemLevitationHero_ScalarDeletingDestructor(void* hero, int, unsigned int flags)
{
    CTCCreatureItemLevitationHero_Destruct(hero);
    if (flags & 1)
        OperatorDelete(hero);
    return hero;
}
int main()
{
    int object = 0;
    if (CTCCreatureItemLevitationHero_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCreatureItemLevitationHero_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e09e4_TEST PASS\n");
    return 0;
}