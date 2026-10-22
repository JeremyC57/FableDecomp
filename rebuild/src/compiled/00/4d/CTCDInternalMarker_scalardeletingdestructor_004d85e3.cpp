#pragma optimize("s", on)
extern void __fastcall CTCDInternalMarker_Destruct(void* marker);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDInternalMarker_ScalarDeletingDestructor(void* marker, int, unsigned int flags)
{
    CTCDInternalMarker_Destruct(marker);
    if (flags & 1)
        OperatorDelete(marker);
    return marker;
}