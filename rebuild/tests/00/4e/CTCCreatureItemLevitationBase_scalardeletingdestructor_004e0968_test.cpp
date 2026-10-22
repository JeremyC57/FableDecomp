#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCreatureItemLevitationBase_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCreatureItemLevitationBase_ScalarDeletingDestructor(void* levitation, int, unsigned int flags)
{
    CTCCreatureItemLevitationBase_Destruct(levitation);
    if (flags & 1)
        OperatorDelete(levitation);
    return levitation;
}
int main()
{
    int object = 0;
    if (CTCCreatureItemLevitationBase_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCreatureItemLevitationBase_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e0968_TEST PASS\n");
    return 0;
}