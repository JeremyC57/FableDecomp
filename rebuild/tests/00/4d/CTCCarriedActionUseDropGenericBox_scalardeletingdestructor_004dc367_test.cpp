#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUseDropGenericBox_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUseDropGenericBox_ScalarDeletingDestructor(void* box, int, unsigned int flags)
{
    CTCCarriedActionUseDropGenericBox_Destruct(box);
    if (flags & 1)
        OperatorDelete(box);
    return box;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUseDropGenericBox_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUseDropGenericBox_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc367_TEST PASS\n");
    return 0;
}