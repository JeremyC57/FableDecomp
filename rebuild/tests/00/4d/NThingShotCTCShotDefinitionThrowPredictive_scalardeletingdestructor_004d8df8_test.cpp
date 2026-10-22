#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionThrowPredictive_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionThrowPredictive_ScalarDeletingDestructor(void* predictive, int, unsigned int flags)
{
    CTCShotDefinitionThrowPredictive_Destruct(predictive);
    if (flags & 1)
        OperatorDelete(predictive);
    return predictive;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionThrowPredictive_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionThrowPredictive_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8df8_TEST PASS\n");
    return 0;
}