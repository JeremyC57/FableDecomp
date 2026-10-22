#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCreatureGroupTag_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCreatureGroupTag_ScalarDeletingDestructor(void* tag, int, unsigned int flags)
{
    CTCCreatureGroupTag_Destruct(tag);
    if (flags & 1)
        OperatorDelete(tag);
    return tag;
}
int main()
{
    int object = 0;
    if (CTCCreatureGroupTag_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCreatureGroupTag_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e764a_TEST PASS\n");
    return 0;
}