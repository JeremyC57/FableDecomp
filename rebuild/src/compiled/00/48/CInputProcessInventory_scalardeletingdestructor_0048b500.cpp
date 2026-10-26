// CInputProcessInventory::`vector deleting destructor' @ 0x0048b500
// Standard MSVC vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; jz L;
//   push esi; call operator delete; add esp,4; L: mov eax,esi; pop esi; ret 4

class CInputProcessInventory
{ public:
    void dtor();  // real scalar destructor @ 0xa0d3d0 (declaration only -> real call)
};


void* __fastcall scalar_deleting_destructor(CInputProcessInventory* process, int edx, unsigned int flags)
{
    process->dtor();
    if (flags & 1)
        operator delete(process);
    return process;
}