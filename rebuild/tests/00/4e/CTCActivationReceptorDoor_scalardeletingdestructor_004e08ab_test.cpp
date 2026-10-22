#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActivationReceptorDoor_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActivationReceptorDoor_ScalarDeletingDestructor(void* door, int, unsigned int flags)
{
    CTCActivationReceptorDoor_Destruct(door);
    if (flags & 1)
        OperatorDelete(door);
    return door;
}
int main()
{
    int object = 0;
    if (CTCActivationReceptorDoor_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActivationReceptorDoor_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e08ab_TEST PASS\n");
    return 0;
}