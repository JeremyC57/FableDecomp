// CInputProcessInventoryTradeScreen::`vector_deleting_destructor'  @ 0x0048b5c0
// Standard MSVC7.1 vector deleting destructor thunk:
//   this->~T(); if (flags & 1) operator delete(this); return this;

struct CInputProcessInventoryTradeScreen
{
    void Destroy();  // scalar destructor body @ 0xa0d3d0 (this in ecx)
};

// operator delete forwarder @ 0xbfe9bc
void operator delete(void*);

void* __fastcall LandscapeBackgroundPatch_scalar_deleting_destructor(
    CInputProcessInventoryTradeScreen* process, int /*edx*/, unsigned int flags)
{
    process->Destroy();
    if (flags & 1)
        operator delete(process);
    return process;
}