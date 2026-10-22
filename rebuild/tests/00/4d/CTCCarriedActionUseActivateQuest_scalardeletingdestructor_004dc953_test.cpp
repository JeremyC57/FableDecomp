#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUseActivateQuest_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUseActivateQuest_ScalarDeletingDestructor(void* quest, int, unsigned int flags)
{
    CTCCarriedActionUseActivateQuest_Destruct(quest);
    if (flags & 1)
        OperatorDelete(quest);
    return quest;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUseActivateQuest_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseActivateQuest_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc953_TEST PASS\n");
    return 0;
}