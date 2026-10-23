#include <cstdio>
static int g_AutoTinySequence[6] = {0};
static int g_AutoTinySequenceCount = 0;
static void* g_AutoTinyFirstMember = 0;
static void* g_AutoTinySecondMember = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
void __fastcall DestructFirstMember_005142e0(void* member)
{
    g_AutoTinyFirstMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 1;
}
void __fastcall DestructSecondMember_00a01c10(void* member)
{
    g_AutoTinySecondMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 2;
}
void __fastcall CTCTrespasser_Destruct(void*)
{
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 3;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CTCTrespasser_CompositeScalarDeletingDestructor(void* trespasser, int, unsigned int flags)
{
    DestructFirstMember_005142e0(static_cast<unsigned char*>(trespasser) + 56);
    DestructSecondMember_00a01c10(static_cast<unsigned char*>(trespasser) + 45);
    CTCTrespasser_Destruct(trespasser);
    if (flags & 1)
        OperatorDelete(trespasser);
    return trespasser;
}
int main()
{
    unsigned char object[256] = {0};
    if (CTCTrespasser_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyFirstMember != object + 56 ||
        g_AutoTinySecondMember != object + 45 ||
        g_AutoTinySequenceCount != 3 ||
        g_AutoTinySequence[0] != 1 ||
        g_AutoTinySequence[1] != 2 ||
        g_AutoTinySequence[2] != 3 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CTCTrespasser_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinySequenceCount != 6 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_007bb630_TEST PASS\n");
    return 0;
}