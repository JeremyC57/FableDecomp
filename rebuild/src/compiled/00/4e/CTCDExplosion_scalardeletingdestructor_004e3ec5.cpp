#pragma optimize("s", on)
extern void __fastcall CTCDExplosion_Destruct(void* explosion);
void* __fastcall CTCDExplosion_ScalarDeletingDestructor(void* explosion, int, unsigned int flags)
{
    CTCDExplosion_Destruct(explosion);
    if (flags & 1)
        operator delete(explosion);
    return explosion;
}