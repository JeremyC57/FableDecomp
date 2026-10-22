#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUseFishingRod_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUseFishingRod_ScalarDeletingDestructor(void* rod, int, unsigned int flags)
{
    CTCCarriedActionUseFishingRod_Destruct(rod);
    if (flags & 1)
        OperatorDelete(rod);
    return rod;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUseFishingRod_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseFishingRod_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dcc44_TEST PASS\n");
    return 0;
}