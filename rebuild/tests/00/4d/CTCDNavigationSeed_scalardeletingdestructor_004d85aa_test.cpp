#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDNavigationSeed_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDNavigationSeed_ScalarDeletingDestructor(void* seed, int, unsigned int flags)
{
    CTCDNavigationSeed_Destruct(seed);
    if (flags & 1)
        OperatorDelete(seed);
    return seed;
}
int main()
{
    int object = 0;
    if (CTCDNavigationSeed_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDNavigationSeed_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d85aa_TEST PASS\n");
    return 0;
}