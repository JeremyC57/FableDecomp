#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CQuestInfoElementMiniGameBar_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CQuestInfoElementMiniGameBar_ScalarDeletingDestructor(void* bar, int, unsigned int flags)
{
    CQuestInfoElementMiniGameBar_Destruct(bar);
    if (flags & 1)
        OperatorDelete(bar);
    return bar;
}
int main()
{
    int object = 0;
    if (CQuestInfoElementMiniGameBar_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CQuestInfoElementMiniGameBar_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00656873_TEST PASS\n");
    return 0;
}