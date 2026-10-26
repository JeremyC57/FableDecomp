#pragma optimize("s", on)
extern void __fastcall CTCLightningOrb_Destruct(void* orb);
void* __fastcall CTCLightningOrb_ScalarDeletingDestructor(void* orb, int, unsigned int flags)
{
    CTCLightningOrb_Destruct(orb);
    if (flags & 1)
        operator delete(orb);
    return orb;
}