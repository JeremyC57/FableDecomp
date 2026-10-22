#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCParticleAttacher_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCParticleAttacher_ScalarDeletingDestructor(void* attacher, int, unsigned int flags)
{
    CTCParticleAttacher_Destruct(attacher);
    if (flags & 1)
        OperatorDelete(attacher);
    return attacher;
}
int main()
{
    int object = 0;
    if (CTCParticleAttacher_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCParticleAttacher_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e0bda_TEST PASS\n");
    return 0;
}