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
void __fastcall CTCTextureDecal_Destruct(void*)
{
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 3;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CTCTextureDecal_CompositeScalarDeletingDestructor(void* decal, int, unsigned int flags)
{
    DestructFirstMember(static_cast<unsigned char*>(decal) + 16);
    DestructSecondMember(static_cast<unsigned char*>(decal) + 12);
    CTCTextureDecal_Destruct(decal);
    if (flags & 1)
        OperatorDelete(decal);
    return decal;
}
int main()
{
    unsigned char object[256] = {0};
    if (CTCTextureDecal_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyFirstMember != object + 16 ||
        g_AutoTinySecondMember != object + 12 ||
        g_AutoTinySequenceCount != 3 ||
        g_AutoTinySequence[0] != 1 ||
        g_AutoTinySequence[1] != 2 ||
        g_AutoTinySequence[2] != 3 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CTCTextureDecal_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinySequenceCount != 6 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_0070b320_TEST PASS\n");
    return 0;
}