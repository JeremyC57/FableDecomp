// CInputProcessInventoryQuestsScreen::`vector_deleting_destructor' 0x0048b5e0
// Standard MSVC vector deleting destructor shape:
//   push esi; mov esi,ecx; call scalar_dtor; test [esp+8],1; je; push esi; call op_delete; add esp,4; mov eax,esi; ret 4

struct CInputProcessInventoryQuestsScreen;

// scalar destructor (direct non-virtual call) @ 0xa0d3d0
extern void __fastcall CInputProcessInventoryQuestsScreen_scalar_dtor(CInputProcessInventoryQuestsScreen* process);
// operator delete @ 0xbfe9bc
extern void __cdecl op_delete(void* p);

// vector deleting destructor, this in ecx, flags pushed on stack.
void* __fastcall scalar_deleting_destructor(CInputProcessInventoryQuestsScreen* process, int edx, unsigned int flags)
{
    (void)edx;
    CInputProcessInventoryQuestsScreen_scalar_dtor(process);
    if (flags & 1)
        op_delete(process);
    return process;
}