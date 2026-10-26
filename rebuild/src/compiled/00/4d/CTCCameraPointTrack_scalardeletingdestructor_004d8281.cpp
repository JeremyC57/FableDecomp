#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCCameraPointTrack
{ public:
    void* vftable;
};
extern void* const g_CTCCameraPointTrack_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointTrack* track, int, unsigned int flags)
{
    track->vftable = (void*)&g_CTCCameraPointTrack_vftable;
    ((CTCBase*)(track))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(track);
    return track;
}