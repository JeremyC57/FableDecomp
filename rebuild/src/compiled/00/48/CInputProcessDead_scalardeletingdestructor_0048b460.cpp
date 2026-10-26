// CInputProcessDead::`vector_deleting_destructor' 0x0048b460
// Standard MSVC scalar/vector deleting destructor idiom:
//   push esi; mov esi,ecx; call dtor; test [esp+8],1; je L; push esi; call delete; add esp,4; L: mov eax,esi; ret 4

class CInputProcessDead
{ public:
    void Destroy();  // real scalar destructor at 0xa0d3d0 (__fastcall)
};

// operator delete equivalent at 0xbfe9bc (cdecl)
void operator delete(void*);

// scalar destructor body (external target)
void __fastcall CInputProcessDead_Destroy(CInputProcessDead* process);

void* __fastcall CInputProcessDead_scalar_deleting_destructor(
    CInputProcessDead* process, int /*edx*/, unsigned int flags)
{
    CInputProcessDead_Destroy(process);
    if (flags & 1)
        operator delete(process);
    return process;
}