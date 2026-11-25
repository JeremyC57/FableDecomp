#pragma optimize("t", on)
class CResource {
public:
    virtual ~CResource();   // 0x009FC550
};
// Naming (NF423, 2026-11-25): the member release 0x00AE94A0 now binds its PDB spelling ??1?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), called on the member through the shared engine/CCountedPointer.h under FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY (declaration only: retail calls the out-of-line COMDAT); was the free stand-in ?DestructFirstMember_00ae94a0@@YIXPAX@Z. Object otherwise unchanged.
// Naming (NF423, 2026-11-25): the member release 0x009A4400 now binds its PDB spelling ??1?$CCountedPointer@VCBankFileAsyncData@@@@QAE@XZ (FableWin.pdb name set; the self row's
//   Leaf), the same way; was the free stand-in ?DestructSecondMember_009a4400@@YIXPAX@Z. Object otherwise unchanged.
#define FABLE_RETAIL_CCOUNTEDPOINTER_DTOR_DECL_ONLY
#include "engine/CCountedPointer.h"
namespace NParticleEngine { class CParticleEmitter; }
class CBankFileAsyncData;
void* __fastcall CEmitterData_CompositeScalarDeletingDestructor(void* emitter, int, unsigned int flags)
{
    reinterpret_cast<CCountedPointer<NParticleEngine::CParticleEmitter>*>(static_cast<unsigned char*>(emitter) + 36)->CCountedPointer<NParticleEngine::CParticleEmitter>::~CCountedPointer();
    reinterpret_cast<CCountedPointer<CBankFileAsyncData>*>(static_cast<unsigned char*>(emitter) + 28)->CCountedPointer<CBankFileAsyncData>::~CCountedPointer();
    ((CResource*)(emitter))->CResource::~CResource();
    if (flags & 1)
        operator delete(emitter);
    return emitter;
}