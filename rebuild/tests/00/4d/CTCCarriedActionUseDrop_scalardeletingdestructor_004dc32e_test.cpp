#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUseDrop_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUseDrop_ScalarDeletingDestructor(void* drop, int, unsigned int flags)
{
    CTCCarriedActionUseDrop_Destruct(drop);
    if (flags & 1)
        OperatorDelete(drop);
    return drop;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUseDrop_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseDrop_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc32e_TEST PASS\n");
    return 0;
}