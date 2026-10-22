#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCMeshAutomaticEntityCreator_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCMeshAutomaticEntityCreator_ScalarDeletingDestructor(void* creator, int, unsigned int flags)
{
    CTCMeshAutomaticEntityCreator_Destruct(creator);
    if (flags & 1)
        OperatorDelete(creator);
    return creator;
}
int main()
{
    int object = 0;
    if (CTCMeshAutomaticEntityCreator_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCMeshAutomaticEntityCreator_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d7b3f_TEST PASS\n");
    return 0;
}