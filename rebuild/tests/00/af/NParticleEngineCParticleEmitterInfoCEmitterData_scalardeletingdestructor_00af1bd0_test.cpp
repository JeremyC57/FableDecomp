#include <cstdio>
static int g_AutoTinySequence[6] = {0};
static int g_AutoTinySequenceCount = 0;
static void* g_AutoTinyFirstMember = 0;
static void* g_AutoTinySecondMember = 0;
static int g_AutoTinyCompositeDeleteCalls = 0;
void __fastcall DestructFirstMember_00ae94a0(void* member)
{
    g_AutoTinyFirstMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 1;
}
void __fastcall DestructSecondMember_009a4400(void* member)
{
    g_AutoTinySecondMember = member;
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 2;
}
void __fastcall CEmitterData_Destruct(void*)
{
    g_AutoTinySequence[g_AutoTinySequenceCount++] = 3;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyCompositeDeleteCalls;
}
void* __fastcall CEmitterData_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    DestructFirstMember_00ae94a0(static_cast<unsigned char*>(emitter) + 36);
    DestructSecondMember_009a4400(static_cast<unsigned char*>(emitter) + 28);
    CEmitterData_Destruct(emitter);
    if (flags & 1)
        OperatorDelete(emitter);
    return emitter;
}
int main()
{
    unsigned char object[256] = {0};
    if (CEmitterData_CompositeScalarDeletingDestructor(object, 0, 0) != object ||
        g_AutoTinyFirstMember != object + 36 ||
        g_AutoTinySecondMember != object + 28 ||
        g_AutoTinySequenceCount != 3 ||
        g_AutoTinySequence[0] != 1 ||
        g_AutoTinySequence[1] != 2 ||
        g_AutoTinySequence[2] != 3 ||
        g_AutoTinyCompositeDeleteCalls != 0)
        return 1;
    CEmitterData_CompositeScalarDeletingDestructor(object, 0, 1);
    if (g_AutoTinySequenceCount != 6 ||
        g_AutoTinyCompositeDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_00af1bd0_TEST PASS\n");
    return 0;
}