#include <cstdio>
static int g_AutoTinyMemberDestructorCalls = 0;
static int g_AutoTinyCompositeDestructorCalls = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
static void* g_AutoTinyObservedMember = 0;
void __fastcall DestructMember(void* member)
{
    ++g_AutoTinyMemberDestructorCalls;
    g_AutoTinyObservedMember = member;
}
void __fastcall CScriptedControl_Destruct(void*)
{
    ++g_AutoTinyCompositeDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CScriptedControl_CompositeScalarDeletingDestructor(void* control, int, unsigned int flags)
{
    DestructMember(static_cast<unsigned char*>(control) + 164);
    CScriptedControl_Destruct(control);
    if (flags & 1)
        OperatorDelete(control);
    return control;
}
int main()
{
    unsigned char object[256] = {0};
    if (CScriptedControl_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyObservedMember != object + 164 ||
        g_AutoTinyMemberDestructorCalls != 1 ||
        g_AutoTinyCompositeDestructorCalls != 1 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CScriptedControl_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinyMemberDestructorCalls != 2 ||
        g_AutoTinyCompositeDestructorCalls != 2 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_0069e700_TEST PASS\n");
    return 0;
}