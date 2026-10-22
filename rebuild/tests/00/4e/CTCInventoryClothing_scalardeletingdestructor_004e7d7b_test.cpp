#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCInventoryClothing_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCInventoryClothing_ScalarDeletingDestructor(void* clothing, int, unsigned int flags)
{
    CTCInventoryClothing_Destruct(clothing);
    if (flags & 1)
        OperatorDelete(clothing);
    return clothing;
}
int main()
{
    int object = 0;
    if (CTCInventoryClothing_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCInventoryClothing_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004e7d7b_TEST PASS\n");
    return 0;
}