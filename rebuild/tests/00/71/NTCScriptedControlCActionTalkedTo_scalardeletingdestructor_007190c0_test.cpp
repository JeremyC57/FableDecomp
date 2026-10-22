#include <cstdio>
static int g_AutoTinySequence[6] = {0};
static int g_AutoTinySequenceCount = 0;
static void* g_AutoTinyFirstMember = 0;
static void* g_AutoTinySecondMember = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
void __fastcall DestructFirstMember(void* member)
{
    g_AutoTinyFirstMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 1;
}
void __fastcall DestructSecondMember(void* member)
{
    g_AutoTinySecondMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 2;
}
void __fastcall CActionTalkedTo_Destruct(void*)
{
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 3;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CActionTalkedTo_CompositeScalarDeletingDestructor(void* action, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(action) + 20);
    DestructSecondMember(static_cast<unsigned char*>(action) + 12);
    CActionTalkedTo_Destruct(action);
    if (flags & 1)
        OperatorDelete(action);
    return action;
}
int main()
{
    unsigned char object[256] = {0};
    if (CActionTalkedTo_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyFirstMember != object + 20 ||
        g_AutoTinySecondMember != object + 12 ||
        g_AutoTinySequenceCount != 3 ||
        g_AutoTinySequence[0] != 1 ||
        g_AutoTinySequence[1] != 2 ||
        g_AutoTinySequence[2] != 3 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CActionTalkedTo_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinySequenceCount != 6 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_007190c0_TEST PASS\n");
    return 0;
}