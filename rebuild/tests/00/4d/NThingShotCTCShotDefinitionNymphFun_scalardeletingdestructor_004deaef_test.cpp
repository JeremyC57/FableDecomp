#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionNymphFun_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionNymphFun_ScalarDeletingDestructor(void* fun, int, unsigned int flags)
{
    CTCShotDefinitionNymphFun_Destruct(fun);
    if (flags & 1)
        OperatorDelete(fun);
    return fun;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionNymphFun_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionNymphFun_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004deaef_TEST PASS\n");
    return 0;
}