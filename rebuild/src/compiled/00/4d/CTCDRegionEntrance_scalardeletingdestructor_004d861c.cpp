#pragma optimize("s", on)
extern void __fastcall CTCDRegionEntrance_Destruct(void* entrance);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDRegionEntrance_ScalarDeletingDestructor(void* entrance, int, unsigned int flags)
{
    CTCDRegionEntrance_Destruct(entrance);
    if (flags & 1)
        OperatorDelete(entrance);
    return entrance;
}