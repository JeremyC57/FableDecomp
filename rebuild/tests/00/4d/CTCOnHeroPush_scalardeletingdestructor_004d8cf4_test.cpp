#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCOnHeroPush_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCOnHeroPush_ScalarDeletingDestructor(void* push, int, unsigned int flags)
{
    CTCOnHeroPush_Destruct(push);
    if (flags & 1)
        OperatorDelete(push);
    return push;
}
int main()
{
    int object = 0;
    if (CTCOnHeroPush_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCOnHeroPush_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8cf4_TEST PASS\n");
    return 0;
}