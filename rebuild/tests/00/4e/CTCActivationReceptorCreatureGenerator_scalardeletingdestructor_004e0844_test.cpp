#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActivationReceptorCreatureGenerator_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActivationReceptorCreatureGenerator_ScalarDeletingDestructor(void* generator, int, unsigned int flags)
{
    CTCActivationReceptorCreatureGenerator_Destruct(generator);
    if (flags & 1)
        OperatorDelete(generator);
    return generator;
}
int main()
{
    int object = 0;
    if (CTCActivationReceptorCreatureGenerator_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActivationReceptorCreatureGenerator_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e0844_TEST PASS\n");
    return 0;
}