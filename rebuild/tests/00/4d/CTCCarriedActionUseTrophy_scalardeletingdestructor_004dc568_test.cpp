#include <cstdio>
static int g_AutoTinyMemberDestructorCalls = 0;
static int g_AutoTinyCompositeDestructorCalls = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
static void* g_AutoTinyObservedMember = 0;
void __fastcall AutoTinyMemberDestructor(void* member)
{
    ++g_AutoTinyMemberDestructorCalls;
    g_AutoTinyObservedMember = member;
}
void __fastcall AutoTinyCompositeDestructor(void*)
{
    ++g_AutoTinyCompositeDestructorCalls;
}
void __cdecl AutoTinyCompositeDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CTCCarriedActionUseTrophy_CompositeScalarDeletingDestructor(void* trophy, int, unsigned int flags)
{
    AutoTinyMemberDestructor(static_cast<unsigned char*>(trophy) + 20);
    AutoTinyCompositeDestructor(trophy);
    if (flags & 1)
        AutoTinyCompositeDelete(trophy);
    return trophy;
}
int main()
{
    unsigned char object[256] = {0};
    if (CTCCarriedActionUseTrophy_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyObservedMember != object + 20 ||
        g_AutoTinyMemberDestructorCalls != 1 ||
        g_AutoTinyCompositeDestructorCalls != 1 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseTrophy_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinyMemberDestructorCalls != 2 ||
        g_AutoTinyCompositeDestructorCalls != 2 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc568_TEST PASS\n");
    return 0;
}