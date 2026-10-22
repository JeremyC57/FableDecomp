#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCBlacksmiths_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCBlacksmiths_ScalarDeletingDestructor(void* blacksmiths, int, unsigned int flags)
{
    CTCBlacksmiths_Destruct(blacksmiths);
    if (flags & 1)
        OperatorDelete(blacksmiths);
    return blacksmiths;
}
int main()
{
    int object = 0;
    if (CTCBlacksmiths_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCBlacksmiths_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8466_TEST PASS\n");
    return 0;
}