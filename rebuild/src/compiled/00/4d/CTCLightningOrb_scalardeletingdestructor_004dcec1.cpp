#pragma optimize("s", on)
extern void __fastcall CTCLightningOrb_Destruct(void* orb);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCLightningOrb_ScalarDeletingDestructor(void* orb, int, unsigned int flags)
{
    CTCLightningOrb_Destruct(orb);
    if (flags & 1)
        OperatorDelete(orb);
    return orb;
}