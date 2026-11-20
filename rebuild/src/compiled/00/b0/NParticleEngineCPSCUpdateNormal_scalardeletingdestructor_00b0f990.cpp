// NParticleEngine::CPSCUpdateNormal::`scalar deleting destructor'(unsigned int) -- stores the CPSCUpdateNormal vtable, kills all decal emitters, destroys the decal-emitter list (+0x9c) and the emitter name (+0x14), runs CBaseClassNonCopyable::~CBaseClassNonCopyable and frees on bit 0
// Leaf: ??_GCPSCUpdateNormal@NParticleEngine@@UAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0F990, length 81 bytes (vtable 0x0129FC1C slot 0).
// Naming (NF158, 2026-11-20, q4020): the local CPSCUpdateNormal / CDecalEmitterArray models replaced by the shared
//   engine/CPSCUpdateNormal.h (Ego_r.pdb layout; DecalEmitterList +0x9c = std::list<CIntelligentPointer<CParticleEmitter> >), so the
//   list teardown binds 0x00B0F630 as ?clear@?$_List_base@V?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@V?$allocator@..@std@@@std@@QAEXXZ
//   (the landed spelling of 0x00B0CA90 and the self row) instead of the CDecalEmitterArray::DestroyElements stand-in.
// Callees: CPSCUpdateNormal::KillAllDecalEmitters 0x00B0C730 (Ego_r + FableWin, protected), _List_base::clear 0x00B0F630
//   (FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE: retail reloads and null-tests the sentinel after the call), free 0x00BFEA14,
//   CCharString::~CCharString 0x0099EAE0, CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300, operator delete 0x00BFE9BC.
// Shape: the inline destructor (FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE, the body of 0x00B0CA90) expanded in the ??_G; CPSCBase and
//   CParticleUpdateParamCallback keep implicit destructors (retail chains straight to ~CBaseClassNonCopyable with one vptr store).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMCALLBACK_IMPLICIT_DTOR
#define FABLE_RETAIL_CPSCBASE_IMPLICIT_DTOR
#define FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE
#include "fable_compiler.h"
#include "engine/CPSCUpdateNormal.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline destructor, so this TU emits CPSCUpdateNormal's vtable
// and with it the scalar deleting destructor (both pick-any COMDATs). Internal to this TU.
void ReferenceCPSCUpdateNormalVtable(NParticleEngine::CPSCUpdateNormal* component)
{
    component->NParticleEngine::CPSCUpdateNormal::~CPSCUpdateNormal();
}
}
