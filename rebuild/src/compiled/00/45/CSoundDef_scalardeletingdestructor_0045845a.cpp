#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x0045845a  CSoundDef::`scalar deleting destructor'
// Standard MSVC scalar/vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call <dtor>; test [esp+8],1; je +7;
//   push esi; call operator delete; pop ecx; mov eax,esi; pop esi; ret 4

class CLandscapeBackgroundPatch;

// scalar destructor (this in ecx) -- relocation-masked target
extern "C" void __fastcall CSoundDef_dtor(CLandscapeBackgroundPatch* thisptr);
// operator delete(void*) -- relocation-masked target
extern "C" void __cdecl engine_operator_delete(void* p);

// __fastcall(this, unsigned flag) modeled as __fastcall with an edx filler
// so the flag argument lands on the stack (ret 4).
void* __fastcall LandscapeBackgroundPatch_vector_deleting_dtor(
    CLandscapeBackgroundPatch* thisptr, int /*edx*/, unsigned int flag)
{
    CSoundDef_dtor(thisptr);
    if (flag & 1)
        engine_operator_delete(thisptr);
    return thisptr;
}