#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDestroyThingOnActionFinish_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDestroyThingOnActionFinish_ScalarDeletingDestructor(void* finish, int, unsigned int flags)
{
    CTCDestroyThingOnActionFinish_Destruct(finish);
    if (flags & 1)
        OperatorDelete(finish);
    return finish;
}
int main()
{
    int object = 0;
    if (CTCDestroyThingOnActionFinish_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDestroyThingOnActionFinish_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d952b_TEST PASS\n");
    return 0;
}