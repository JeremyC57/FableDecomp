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
void __fastcall CModeDrained_Destruct(void*)
{
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 3;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CModeDrained_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(mode) + 116);
    DestructSecondMember(static_cast<unsigned char*>(mode) + 104);
    CModeDrained_Destruct(mode);
    if (flags & 1)
        OperatorDelete(mode);
    return mode;
}
int main()
{
    unsigned char object[256] = {0};
    if (CModeDrained_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyFirstMember != object + 116 ||
        g_AutoTinySecondMember != object + 104 ||
        g_AutoTinySequenceCount != 3 ||
        g_AutoTinySequence[0] != 1 ||
        g_AutoTinySequence[1] != 2 ||
        g_AutoTinySequence[2] != 3 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CModeDrained_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinySequenceCount != 6 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00741490_TEST PASS\n");
    return 0;
}