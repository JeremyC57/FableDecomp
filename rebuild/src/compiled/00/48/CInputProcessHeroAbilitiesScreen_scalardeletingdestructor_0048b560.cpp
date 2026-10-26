// CInputProcessHeroAbilitiesScreen::`vector_deleting_destructor'
// Canonical MSVC vector deleting destructor.

class CInputProcessHeroAbilitiesScreen
{ public:
    void scalar_destructor();   // scalar dtor at 0xa0d3d0 (external, non-inlined)
};


void* __fastcall CInputProcessHeroAbilitiesScreen_scalar_deleting_destructor(
        CInputProcessHeroAbilitiesScreen* process, int /*edx*/, unsigned int flags)
{
    process->scalar_destructor();
    if (flags & 1)
        operator delete(process);
    return process;
}