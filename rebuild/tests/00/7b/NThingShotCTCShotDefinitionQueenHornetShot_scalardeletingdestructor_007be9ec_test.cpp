#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionQueenHornetShot_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionQueenHornetShot_ScalarDeletingDestructor(void* shot, int, unsigned int flags)
{
    CTCShotDefinitionQueenHornetShot_Destruct(shot);
    if (flags & 1)
        OperatorDelete(shot);
    return shot;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionQueenHornetShot_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionQueenHornetShot_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_007be9ec_TEST PASS\n");
    return 0;
}