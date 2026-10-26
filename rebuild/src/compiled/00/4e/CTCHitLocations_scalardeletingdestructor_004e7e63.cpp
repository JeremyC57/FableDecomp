#pragma optimize("s", on)
extern void __fastcall CTCHitLocations_Destruct(void* locations);
void* __fastcall CTCHitLocations_ScalarDeletingDestructor(void* locations, int, unsigned int flags)
{
    CTCHitLocations_Destruct(locations);
    if (flags & 1)
        operator delete(locations);
    return locations;
}