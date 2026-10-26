// CInputProcessInventoryWeapons::`vector deleting destructor'
// Standard MSVC deleting-destructor shape:
//   push esi; mov esi,ecx; call ~T (0xa0d3d0); test [esp+8],1; je;
//   push esi; call operator delete (0xbfe9bc); add esp,4; mov eax,esi; pop esi; ret 4

class CInputProcessInventoryWeapons
{ public:
    void destroy();                        // scalar (base) destructor at 0xa0d3d0
    void* deleting(unsigned int flags);
};

// External targets (bodies live elsewhere) -> emitted as real calls, not inlined.

void* CInputProcessInventoryWeapons::deleting(unsigned int flags)
{
    this->destroy();
    if (flags & 1)
        operator delete(this);
    return this;
}