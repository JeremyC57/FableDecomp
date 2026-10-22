#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCShotDefinitionFireballSpell_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCShotDefinitionFireballSpell_ScalarDeletingDestructor(void* spell, int, unsigned int flags)
{
    CTCShotDefinitionFireballSpell_Destruct(spell);
    if (flags & 1)
        OperatorDelete(spell);
    return spell;
}
int main()
{
    int object = 0;
    if (CTCShotDefinitionFireballSpell_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCShotDefinitionFireballSpell_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dcb20_TEST PASS\n");
    return 0;
}