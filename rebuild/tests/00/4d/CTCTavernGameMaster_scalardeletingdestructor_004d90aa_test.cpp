#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCTavernGameMaster_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCTavernGameMaster_ScalarDeletingDestructor(void* master, int, unsigned int flags)
{
    CTCTavernGameMaster_Destruct(master);
    if (flags & 1)
        OperatorDelete(master);
    return master;
}
int main()
{
    int object = 0;
    if (CTCTavernGameMaster_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCTavernGameMaster_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d90aa_TEST PASS\n");
    return 0;
}