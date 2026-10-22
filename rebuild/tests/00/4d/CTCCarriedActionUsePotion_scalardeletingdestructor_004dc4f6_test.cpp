#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCCarriedActionUsePotion_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCCarriedActionUsePotion_ScalarDeletingDestructor(void* potion, int, unsigned int flags)
{
    CTCCarriedActionUsePotion_Destruct(potion);
    if (flags & 1)
        OperatorDelete(potion);
    return potion;
}
int main()
{
    int object = 0;
    if (CTCCarriedActionUsePotion_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCCarriedActionUsePotion_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dc4f6_TEST PASS\n");
    return 0;
}