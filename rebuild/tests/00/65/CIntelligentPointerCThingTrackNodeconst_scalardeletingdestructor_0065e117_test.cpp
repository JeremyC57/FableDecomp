#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CIntelligentPointer_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CIntelligentPointer_ScalarDeletingDestructor(void* pointer, int, unsigned int flags)
{
    CIntelligentPointer_Destruct(pointer);
    if (flags & 1)
        OperatorDelete(pointer);
    return pointer;
}
int main()
{
    int object = 0;
    if (CIntelligentPointer_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CIntelligentPointer_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_0065e117_TEST PASS\n");
    return 0;
}