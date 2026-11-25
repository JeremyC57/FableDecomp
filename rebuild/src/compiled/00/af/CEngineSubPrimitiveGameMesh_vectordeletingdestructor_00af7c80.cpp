// CEngineSubPrimitiveGameMesh::`vector deleting destructor'  0x00af7c80
// Standard MSVC vector-deleting-destructor thunk:
//   push esi; mov esi,ecx; call dtor(this); test [esp+8],1; je; mov ecx,esi; call delop(this);
//   mov eax,esi; pop esi; ret 4
// Both callees are __fastcall (this in ecx), so model as members.
// Naming (NF450 / NF451, 2026-11-25; r450a done through the shared header): callee-only respell to the PDB names (T1 tracker; target
//   self rows' Leaf) through engine/CDecal.h's add-only FABLE_RETAIL_CDECAL_NO_CONTAINERS view (the class without its CArray members,
//   so no STLport and no new .drectve token; r449d: the shared header, not a bodiless class):
//   0x00AF7C10 ??1CDecal@NParticleEngine@@QAE@XZ (was ?dtor_helper@CEngineSubPrimitiveGameMesh@@QAEXXZ), a qualified destructor call;
//   0x00AF8AD0 ??3CDecal@NParticleEngine@@SIXPAX@Z (was ?free_helper@CEngineSubPrimitiveGameMesh@@QAEXXZ), the static __fastcall class delete.
#define FABLE_RETAIL_CDECAL_NO_CONTAINERS
#include "engine/CDecal.h"

struct CEngineSubPrimitiveGameMesh {
    void* vector_deleting_destructor(unsigned int flags);
};

void* CEngineSubPrimitiveGameMesh::vector_deleting_destructor(unsigned int flags)
{
    reinterpret_cast<NParticleEngine::CDecal*>(this)->NParticleEngine::CDecal::~CDecal();
    if (flags & 1)
        NParticleEngine::CDecal::operator delete(this);
    return this;
}