#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCFireRangedWeaponOnLeavingStateGroup_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCFireRangedWeaponOnLeavingStateGroup_ScalarDeletingDestructor(void* group, int, unsigned int flags)
{
    CTCFireRangedWeaponOnLeavingStateGroup_Destruct(group);
    if (flags & 1)
        OperatorDelete(group);
    return group;
}
int main()
{
    int object = 0;
    if (CTCFireRangedWeaponOnLeavingStateGroup_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCFireRangedWeaponOnLeavingStateGroup_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d9740_TEST PASS\n");
    return 0;
}