#pragma optimize("s", on)
extern void __fastcall CTCDRumble_Destruct(void* rumble);
void* __fastcall CTCDRumble_ScalarDeletingDestructor(void* rumble, int, unsigned int flags)
{
    CTCDRumble_Destruct(rumble);
    if (flags & 1)
        operator delete(rumble);
    return rumble;
}