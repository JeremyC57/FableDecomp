#pragma optimize("s", on)
extern void __fastcall CTCHitLocations_Destruct(void* locations);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCHitLocations_ScalarDeletingDestructor(void* locations, int, unsigned int flags)
{
    CTCHitLocations_Destruct(locations);
    if (flags & 1)
        OperatorDelete(locations);
    return locations;
}