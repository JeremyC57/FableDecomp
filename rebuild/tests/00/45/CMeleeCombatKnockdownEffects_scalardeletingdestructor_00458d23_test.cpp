#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CMeleeCombatKnockdownEffects_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CMeleeCombatKnockdownEffects_ScalarDeletingDestructor(void* effects, int, unsigned int flags)
{
    CMeleeCombatKnockdownEffects_Destruct(effects);
    if (flags & 1)
        OperatorDelete(effects);
    return effects;
}
int main()
{
    int object = 0;
    if (CMeleeCombatKnockdownEffects_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CMeleeCombatKnockdownEffects_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00458d23_TEST PASS\n");
    return 0;
}