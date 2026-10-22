#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCInventoryAbilities_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCInventoryAbilities_ScalarDeletingDestructor(void* abilities, int, unsigned int flags)
{
    CTCInventoryAbilities_Destruct(abilities);
    if (flags & 1)
        OperatorDelete(abilities);
    return abilities;
}
int main()
{
    int object = 0;
    if (CTCInventoryAbilities_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCInventoryAbilities_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_005fa461_TEST PASS\n");
    return 0;
}