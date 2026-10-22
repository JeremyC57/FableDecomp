#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionForcePushBall_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionForcePushBall_ScalarDeletingDestructor(void* ball, int, unsigned int flags)
{
    CTCShotDefinitionForcePushBall_Destruct(ball);
    if (flags & 1)
        OperatorDelete(ball);
    return ball;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionForcePushBall_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionForcePushBall_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004deb67_TEST PASS\n");
    return 0;
}