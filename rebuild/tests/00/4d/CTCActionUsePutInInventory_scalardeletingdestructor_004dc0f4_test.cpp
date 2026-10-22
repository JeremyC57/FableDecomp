#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCActionUsePutInInventory_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCActionUsePutInInventory_ScalarDeletingDestructor(void* inventory, int, unsigned int flags)
{
    CTCActionUsePutInInventory_Destruct(inventory);
    if (flags & 1)
        OperatorDelete(inventory);
    return inventory;
}
int main()
{
    int object = 0;
    if (CTCActionUsePutInInventory_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCActionUsePutInInventory_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc0f4_TEST PASS\n");
    return 0;
}