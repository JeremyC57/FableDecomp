#include <cstdio>
static int g_AutoTinyVectorDestructorCalls = 0;
static int g_AutoTinyVectorDeleteCalls = 0;
class CTCCameraPointTrack
{ public:
    void* vftable;
};
void* const g_CTCCameraPointTrack_vftable = (void*)0x12345678;
void __fastcall CTCCameraPointTrack_Destruct(void*)
{
    ++g_AutoTinyVectorDestructorCalls;
}
void __cdecl operator delete(void*)
{
    ++g_AutoTinyVectorDeleteCalls;
}
void* __fastcall scalar_deleting_destructor(CTCCameraPointTrack* track, int, unsigned int flags)
{
    track->vftable = (void*)&g_CTCCameraPointTrack_vftable;
    CTCCameraPointTrack_Destruct(track);
    if (flags & 1)
        operator delete(track);
    return track;
}
int main()
{
    CTCCameraPointTrack object = {0};
    if (scalar_deleting_destructor(&object, 0, 0) != &object ||
        object.vftable != &g_CTCCameraPointTrack_vftable ||
        g_AutoTinyVectorDestructorCalls != 1 ||
        g_AutoTinyVectorDeleteCalls != 0)
        return 1;
    scalar_deleting_destructor(&object, 0, 1);
    if (g_AutoTinyVectorDestructorCalls != 2 ||
        g_AutoTinyVectorDeleteCalls != 1)
        return 1;
    std::printf("AUTO_TINY_004d8281_TEST PASS\n");
    return 0;
}