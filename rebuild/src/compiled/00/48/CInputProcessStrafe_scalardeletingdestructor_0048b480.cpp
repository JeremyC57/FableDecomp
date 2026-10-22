// CInputProcessStrafe::`vector_deleting_destructor'  @ 0x0048b480
// Standard MSVC-generated vector deleting destructor:
//   push esi; mov esi,ecx; call <scalar dtor @0xa0d3d0>; test [esp+8],1
//   jz skip; push esi; call <op delete @0xbfe9bc>; add esp,4; skip: mov eax,esi; ret 4

struct CInputProcessStrafe
{
    void *vfptr;
};

// scalar destructor (this in ecx) and operator delete are external calls in retail;
// declared (not defined) so the /c parity compile emits real call relocations.
void __fastcall CInputProcessStrafe_dtor(CInputProcessStrafe *process);
extern "C" void __cdecl fable_op_delete(void *p);

void * __fastcall CInputProcessStrafe_scalar_deleting_destructor(
    CInputProcessStrafe *process, int /*edx*/, unsigned int flags)
{
    CInputProcessStrafe_dtor(process);
    if (flags & 1)
        fable_op_delete(process);
    return process;
}