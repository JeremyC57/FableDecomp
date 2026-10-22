// CParticle::`vector_deleting_destructor'
// Retail 0x00af7b40 — MSVC scalar/vector deleting destructor thunk.
//
// push esi; mov esi,ecx; call 0xaf7b60 (dtor body); test [esp+8],1;
// je skip; mov ecx,esi; call 0xaf8650 (member operator delete);
// skip: mov eax,esi; pop esi; ret 4

struct CParticle
{
    void inner_dtor();                 // real destructor body @ 0xaf7b60
    void member_free();                // member operator delete @ 0xaf8650 (this in ecx)
};

void* __fastcall CParticle_scalar_deleting_destructor(
    CParticle* particle, int /*edx*/, unsigned int flags)
{
    particle->inner_dtor();
    if (flags & 1)
        particle->member_free();
    return particle;
}