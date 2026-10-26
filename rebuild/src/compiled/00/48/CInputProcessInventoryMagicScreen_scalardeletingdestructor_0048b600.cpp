// CInputProcessInventoryMagicScreen::`vector_deleting_destructor' @ 0x0048b600
// Canonical MSVC vector deleting destructor.

class CInputProcessInventoryMagicScreen
{ public:
    void scalar_destructor();   // scalar dtor at 0xa0d3d0 (external, non-inlined)
};


void* __fastcall CInputProcessInventoryMagicScreen_scalar_deleting_destructor(
        CInputProcessInventoryMagicScreen* process, int /*edx*/, unsigned int flags)
{
    process->scalar_destructor();
    if (flags & 1)
        operator delete(process);
    return process;
}