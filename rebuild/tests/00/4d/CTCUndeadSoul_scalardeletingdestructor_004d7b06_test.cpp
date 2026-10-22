#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCUndeadSoul_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCUndeadSoul_ScalarDeletingDestructor(void* soul, int, unsigned int flags)
{
    CTCUndeadSoul_Destruct(soul);
    if (flags & 1)
        OperatorDelete(soul);
    return soul;
}
int main()
{
    int object = 0;
    if (CTCUndeadSoul_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCUndeadSoul_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d7b06_TEST PASS\n");
    return 0;
}