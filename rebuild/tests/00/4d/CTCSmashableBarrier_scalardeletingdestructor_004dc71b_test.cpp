#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCSmashableBarrier_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCSmashableBarrier_ScalarDeletingDestructor(void* barrier, int, unsigned int flags)
{
    CTCSmashableBarrier_Destruct(barrier);
    if (flags & 1)
        OperatorDelete(barrier);
    return barrier;
}
int main()
{
    int object = 0;
    if (CTCSmashableBarrier_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCSmashableBarrier_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc71b_TEST PASS\n");
    return 0;
}