#pragma optimize("s", on)
extern void __fastcall CTCParticleAttacher_Destruct(void* attacher);
void* __fastcall CTCParticleAttacher_ScalarDeletingDestructor(void* attacher, int, unsigned int flags)
{
    CTCParticleAttacher_Destruct(attacher);
    if (flags & 1)
        operator delete(attacher);
    return attacher;
}