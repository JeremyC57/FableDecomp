#include <cstdio>
static int g_AutoTinyMemberDestructorCalls = 0;
static int g_AutoTinyCompositeDestructorCalls = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
static void* g_AutoTinyObservedMember = 0;
void __fastcall DestructMember_0071ee70(void* member)
{
    ++g_AutoTinyMemberDestructorCalls;
    g_AutoTinyObservedMember = member;
}
void __fastcall CTCSimpleAppearanceMorph_Destruct(void*)
{
    ++g_AutoTinyCompositeDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CTCSimpleAppearanceMorph_CompositeScalarDeletingDestructor(void* morph, int, unsigned int flags)
{
    DestructMember_0071ee70(static_cast<unsigned char*>(morph) + 12);
    CTCSimpleAppearanceMorph_Destruct(morph);
    if (flags & 1)
        OperatorDelete(morph);
    return morph;
}
int main()
{
    unsigned char object[256] = {0};
    if (CTCSimpleAppearanceMorph_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyObservedMember != object + 12 ||
        g_AutoTinyMemberDestructorCalls != 1 ||
        g_AutoTinyCompositeDestructorCalls != 1 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CTCSimpleAppearanceMorph_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinyMemberDestructorCalls != 2 ||
        g_AutoTinyCompositeDestructorCalls != 2 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_007276d0_TEST PASS\n");
    return 0;
}