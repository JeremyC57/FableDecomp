#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionThrow_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionThrow_ScalarDeletingDestructor(void* definitionThrow, int, unsigned int flags)
{
    CTCShotDefinitionThrow_Destruct(definitionThrow);
    if (flags & 1)
        OperatorDelete(definitionThrow);
    return definitionThrow;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionThrow_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionThrow_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8dbc_TEST PASS\n");
    return 0;
}