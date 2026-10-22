#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
struct CTCCameraPointDead
{
    void* vftable;
};
void* const g_CTCCameraPointDead_vftable = (void*)0x12345678;
void __fastcall CTCCameraPointDead_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CTCCameraPointDead* dead, int, unsigned int flags)
{
    dead->vftable = (void*)&g_CTCCameraPointDead_vftable;
    CTCCameraPointDead_Destruct(dead);
    if (flags & 1)
        operator delete(dead);
    return dead;
}
int main()
{
    CTCCameraPointDead object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CTCCameraPointDead_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d82ff_TEST PASS\n");
    return 0;
}