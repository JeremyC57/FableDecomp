#pragma optimize("s", on)
extern void __fastcall CTCDExplosiveTrail_Destruct(void* trail);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDExplosiveTrail_ScalarDeletingDestructor(void* trail, int, unsigned int flags)
{
    CTCDExplosiveTrail_Destruct(trail);
    if (flags & 1)
        OperatorDelete(trail);
    return trail;
}