#pragma optimize("s", on)
struct CTCCameraPointTrack
{
    void* vftable;
};
extern void* const g_CTCCameraPointTrack_vftable;
extern void __fastcall CTCCameraPointTrack_Destruct(void* track);
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointTrack* track, int, unsigned int flags)
{
    track->vftable = (void*)&g_CTCCameraPointTrack_vftable;
    CTCCameraPointTrack_Destruct(track);
    if (flags & 1)
        operator delete(track);
    return track;
}