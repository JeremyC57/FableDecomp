struct CTrack {
    void* vftable;
};

void __fastcall CTrack_dtor(CTrack* track);
void operator delete(void*);

void* __fastcall CTrack_scalar_deleting_destructor(CTrack* track, int edx_unused, unsigned int flags)
{
    CTrack_dtor(track);
    if (flags & 1)
        operator delete(track);
    return track;
}