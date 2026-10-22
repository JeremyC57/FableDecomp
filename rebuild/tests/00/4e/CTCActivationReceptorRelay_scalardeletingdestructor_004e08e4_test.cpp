#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActivationReceptorRelay_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActivationReceptorRelay_ScalarDeletingDestructor(void* relay, int, unsigned int flags)
{
    CTCActivationReceptorRelay_Destruct(relay);
    if (flags & 1)
        OperatorDelete(relay);
    return relay;
}
int main()
{
    int object = 0;
    if (CTCActivationReceptorRelay_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActivationReceptorRelay_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e08e4_TEST PASS\n");
    return 0;
}