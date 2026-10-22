#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUsePickUpGenericBox_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUsePickUpGenericBox_ScalarDeletingDestructor(void* box, int, unsigned int flags)
{
    CTCActionUsePickUpGenericBox_Destruct(box);
    if (flags & 1)
        OperatorDelete(box);
    return box;
}
int main()
{
    int object = 0;
    if (CTCActionUsePickUpGenericBox_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUsePickUpGenericBox_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc0bb_TEST PASS\n");
    return 0;
}