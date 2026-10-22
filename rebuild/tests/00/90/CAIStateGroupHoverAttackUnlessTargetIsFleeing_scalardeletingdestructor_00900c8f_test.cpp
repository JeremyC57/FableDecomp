#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct CAIStateGroup_HoverAttackUnlessTargetIsFleeing
{
    void* vftable;
};
void* const g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable = (void*)0x12345678;
void __fastcall CAIStateGroup_HoverAttackUnlessTargetIsFleeing_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CAIStateGroup_HoverAttackUnlessTargetIsFleeing* stateGroup, int, unsigned int flags)
{
    stateGroup->vftable = (void*)&g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable;
    CAIStateGroup_HoverAttackUnlessTargetIsFleeing_Destruct(stateGroup);
    if (flags & 1)
        operator delete(stateGroup);
    return stateGroup;
}
int main()
{
    CAIStateGroup_HoverAttackUnlessTargetIsFleeing object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CAIStateGroup_HoverAttackUnlessTargetIsFleeing_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00900c8f_TEST PASS\n");
    return 0;
}