#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUseStartExtrapolationMorph_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUseStartExtrapolationMorph_ScalarDeletingDestructor(void* morph, int, unsigned int flags)
{
    CTCActionUseStartExtrapolationMorph_Destruct(morph);
    if (flags & 1)
        OperatorDelete(morph);
    return morph;
}
int main()
{
    int object = 0;
    if (CTCActionUseStartExtrapolationMorph_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUseStartExtrapolationMorph_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc24a_TEST PASS\n");
    return 0;
}