#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCStealableItemLocation_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCStealableItemLocation_ScalarDeletingDestructor(void* location, int, unsigned int flags)
{
    CTCStealableItemLocation_Destruct(location);
    if (flags & 1)
        OperatorDelete(location);
    return location;
}
int main()
{
    int object = 0;
    if (CTCStealableItemLocation_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCStealableItemLocation_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8998_TEST PASS\n");
    return 0;
}