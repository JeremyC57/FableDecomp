#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
class CTCCameraPointFixedDolly
{ public:
    void* vftable;
};
void* const g_CTCCameraPointFixedDolly_vftable = (void*)0x12345678;
void __fastcall CTCCameraPointFixedDolly_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CTCCameraPointFixedDolly* dolly, int, unsigned int flags)
{
    dolly->vftable = (void*)&g_CTCCameraPointFixedDolly_vftable;
    CTCCameraPointFixedDolly_Destruct(dolly);
    if (flags & 1)
        operator delete(dolly);
    return dolly;
}
int main()
{
    CTCCameraPointFixedDolly object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CTCCameraPointFixedDolly_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8242_TEST PASS\n");
    return 0;
}