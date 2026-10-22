#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCResurrectionItem_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCResurrectionItem_ScalarDeletingDestructor(void* item, int, unsigned int flags)
{
    CTCResurrectionItem_Destruct(item);
    if (flags & 1)
        OperatorDelete(item);
    return item;
}
int main()
{
    int object = 0;
    if (CTCResurrectionItem_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCResurrectionItem_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dccb6_TEST PASS\n");
    return 0;
}