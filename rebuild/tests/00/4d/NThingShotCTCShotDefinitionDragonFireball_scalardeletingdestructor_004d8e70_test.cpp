#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionDragonFireball_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionDragonFireball_ScalarDeletingDestructor(void* fireball, int, unsigned int flags)
{
    CTCShotDefinitionDragonFireball_Destruct(fireball);
    if (flags & 1)
        OperatorDelete(fireball);
    return fireball;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionDragonFireball_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionDragonFireball_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8e70_TEST PASS\n");
    return 0;
}