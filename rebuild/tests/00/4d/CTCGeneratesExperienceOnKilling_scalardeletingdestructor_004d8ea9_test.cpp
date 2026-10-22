#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCGeneratesExperienceOnKilling_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCGeneratesExperienceOnKilling_ScalarDeletingDestructor(void* killing, int, unsigned int flags)
{
    CTCGeneratesExperienceOnKilling_Destruct(killing);
    if (flags & 1)
        OperatorDelete(killing);
    return killing;
}
int main()
{
    int object = 0;
    if (CTCGeneratesExperienceOnKilling_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCGeneratesExperienceOnKilling_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8ea9_TEST PASS\n");
    return 0;
}