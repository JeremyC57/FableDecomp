#include <cstdio>
static int g_AutoTinyScalarDestructorCalls = 0;
static int g_AutoTinyScalarDeleteCalls = 0;
void __fastcall CTCKrakenTentacle_Destruct(void*)
{
    ++g_AutoTinyScalarDestructorCalls;
}
void __cdecl OperatorDelete(void*)
{
    ++g_AutoTinyScalarDeleteCalls;
}
void* __fastcall CTCKrakenTentacle_ScalarDeletingDestructor(void* tentacle, int, unsigned int flags)
{
    CTCKrakenTentacle_Destruct(tentacle);
    if (flags & 1)
        OperatorDelete(tentacle);
    return tentacle;
}
int main()
{
    int object = 0;
    if (CTCKrakenTentacle_ScalarDeletingDestructor(&object, 0, 0) != &object ||
        g_AutoTinyScalarDestructorCalls != 1 ||
        g_AutoTinyScalarDeleteCalls != 0)
        return 1;
    CTCKrakenTentacle_ScalarDeletingDestructor(&object, 0, 1);
    if (g_AutoTinyScalarDestructorCalls != 2 ||
        g_AutoTinyScalarDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004dcd0f_TEST PASS\n");
    return 0;
}