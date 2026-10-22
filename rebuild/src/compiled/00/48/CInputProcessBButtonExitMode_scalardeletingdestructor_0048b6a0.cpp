// CInputProcessBButtonExitMode::`vector_deleting_destructor'  @ 0x0048b6a0
// Standard MSVC vector-deleting-destructor thunk shape:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je L;
//   push esi; call <operator delete>; add esp,4; L: mov eax,esi; pop esi; ret 4

struct CInputProcessBButtonExitMode;

// base/scalar destructor (this in ecx) -> retail 0xa0d3d0
void __fastcall CInputProcessBButtonExitMode_dtor(CInputProcessBButtonExitMode* process);
// operator delete -> retail 0xbfe9bc
void operator_delete_stub(void* p);

void* __fastcall CInputProcessBButtonExitMode_scalar_deleting_destructor(
        CInputProcessBButtonExitMode* process, int /*edx*/, unsigned int flags)
{
    CInputProcessBButtonExitMode_dtor(process);
    if (flags & 1)
        operator_delete_stub(process);
    return process;
}