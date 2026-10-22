#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionProjectile_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionProjectile_ScalarDeletingDestructor(void* projectile, int, unsigned int flags)
{
    CTCShotDefinitionProjectile_Destruct(projectile);
    if (flags & 1)
        OperatorDelete(projectile);
    return projectile;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionProjectile_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionProjectile_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dea77_TEST PASS\n");
    return 0;
}