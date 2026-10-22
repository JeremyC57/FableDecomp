#pragma optimize("s", on)
extern void __fastcall CTCDExplosion_Destruct(void* explosion);
extern void __cdecl OperatorDelete(void* object);
void* __fastcall CTCDExplosion_ScalarDeletingDestructor(void* explosion, int, unsigned int flags)
{
    CTCDExplosion_Destruct(explosion);
    if (flags & 1)
        OperatorDelete(explosion);
    return explosion;
}