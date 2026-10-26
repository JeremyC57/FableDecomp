// CInputProcessHeroAbilitiesScreen::`vector_deleting_destructor'
// Canonical MSVC vector deleting destructor.

class CInputProcessHeroAbilitiesScreen
{ public:
    void scalar_destructor();   // scalar dtor at 0xa0d3d0 (external, non-inlined)
};

extern void __cdecl fable_operator_delete(void*);  // operator delete at 0xbfe9bc

void* __fastcall CInputProcessHeroAbilitiesScreen_scalar_deleting_destructor(
        CInputProcessHeroAbilitiesScreen* process, int /*edx*/, unsigned int flags)
{
    process->scalar_destructor();
    if (flags & 1)
        fable_operator_delete(process);
    return process;
}