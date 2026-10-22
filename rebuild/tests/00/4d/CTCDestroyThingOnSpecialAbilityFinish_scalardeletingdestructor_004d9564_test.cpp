#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCDestroyThingOnSpecialAbilityFinish_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCDestroyThingOnSpecialAbilityFinish_ScalarDeletingDestructor(void* finish, int, unsigned int flags)
{
    CTCDestroyThingOnSpecialAbilityFinish_Destruct(finish);
    if (flags & 1)
        OperatorDelete(finish);
    return finish;
}
int main()
{
    int object = 0;
    if (CTCDestroyThingOnSpecialAbilityFinish_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCDestroyThingOnSpecialAbilityFinish_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d9564_TEST PASS\n");
    return 0;
}