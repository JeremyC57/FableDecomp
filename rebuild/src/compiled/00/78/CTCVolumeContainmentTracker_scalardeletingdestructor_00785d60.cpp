struct CTCVolumeContainmentTracker {
    void* vftable;
};

void __fastcall CTCVolumeContainmentTracker_dtor(CTCVolumeContainmentTracker* tracker);
void operator delete(void*);

void* __fastcall CTCVolumeContainmentTracker_scalar_deleting_destructor(CTCVolumeContainmentTracker* tracker, int edx_unused, unsigned int flags)
{
    CTCVolumeContainmentTracker_dtor(tracker);
    if (flags & 1)
        operator delete(tracker);
    return tracker;
}