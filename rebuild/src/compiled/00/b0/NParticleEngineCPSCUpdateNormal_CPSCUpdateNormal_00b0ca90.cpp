// NParticleEngine::CPSCUpdateNormal::~CPSCUpdateNormal() -- store the update-normal component vtable, kill every decal emitter, destroy the decal-emitter list and the emitter name, then the inline CPSCBase chain and ~CBaseClassNonCopyable
// Leaf: ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ
// Naming (NF158, 2026-11-20, q4020 family with ??_G 0x00B0F990): the local CPSCUpdateNormal / CPSCBase / list models replaced by the
//   shared engine/CPSCUpdateNormal.h. The destructor body is the header's inline one (opt-in FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE),
//   so this row and 0x00B0F990 (which expands it) both emit ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ as a pick-any COMDAT: one
//   definition (nf86 / nf88 scaffolding). CPSCBase / CParticleUpdateParamCallback declare no destructor (opt-ins
//   FABLE_RETAIL_CPSCBASE_IMPLICIT_DTOR / FABLE_RETAIL_CPARTICLEUPDATEPARAMCALLBACK_IMPLICIT_DTOR), as the Shape note below says.
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B0CA90, 60 bytes, thiscall, tail `jmp ~CBaseClassNonCopyable` 0x0099A300; no
//   callers (the slot-0 `??_G` 0x00B0F990 of vtable 0x0129FC1C holds the same body inline).
// Identity (proven): the first store is vtable 0x0129FC1C (RTTI .?AVCPSCUpdateNormal@NParticleEngine@@ : CPSCBase :
//   CParticleUpdateParamCallback<KK> : CBaseClassNonCopyable : CBaseClass); Ego_r.pdb / FableWin.pdb
//   ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ (a user dtor: the store and the KillAllDecalEmitters body), the body
//   ??_GCPSCUpdateNormal 0x00B0F990 inlines (landed). The catalog label CExclusiveAssert::~CExclusiveAssert is propagated.
// Callees: CPSCUpdateNormal::KillAllDecalEmitters 0x00B0C730 (?KillAllDecalEmitters@CPSCUpdateNormal@NParticleEngine@@IAEXXZ:
//   protected in both PDBs; the bound QAEXXZ is a stand-in, NAME_FIXES.tsv), std::_List_base<CIntelligentPointer<NParticleEngine::
//   CParticleEmitter> >::clear 0x00B0F630 (landed spelling), free 0x00BFEA14 (the sentinel), CCharString::~CCharString 0x0099EAE0,
//   CBaseClassNonCopyable::~CBaseClassNonCopyable 0x0099A300 (MAE, tail).
// Layout = Ego_r.pdb CPSCUpdateNormal (no container before DecalEmitterList, so the offsets are retail's; 0xa0 with the 4-byte
//   STLport list): CPSCBase 0xc, the ulong bit-field word +0xc, the bool bit-field byte +0x10, DecalEmitterName +0x14, the
//   floats +0x18..+0x50, five C3DVectors +0x54..+0x84, AccelerationParam +0x90, OrientFromGameParam +0x94, LifeTimer +0x98,
//   DecalEmitterList +0x9c.
// Shape: own vtable store, the user body, then the members in reverse order: the real STLport 4.6 list (`~list() {}`, inline
//   ~_List_base with the out-of-class clear() kept out of line -- member address in edi) and the string; CPSCBase /
//   CParticleUpdateParamCallback declare no destructor (no store of their own: the landed CPSCAttractor 0x00B18780 chain), so
//   the tail is ~CBaseClassNonCopyable.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST_CLEAR_OUTOFLINE
#define FABLE_RETAIL_CPARTICLEUPDATEPARAMCALLBACK_IMPLICIT_DTOR
#define FABLE_RETAIL_CPSCBASE_IMPLICIT_DTOR
#define FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE
#include "fable_compiler.h"
#include "engine/CPSCUpdateNormal.h"

namespace {
// Fidelity: scaffolding, not retail: the smallest use of the header's inline destructor, so this TU emits it (and the vtable) as
// pick-any COMDATs, like every includer. Internal to this TU.
void ReferenceCPSCUpdateNormalDestructor(NParticleEngine::CPSCUpdateNormal* component)
{
    component->NParticleEngine::CPSCUpdateNormal::~CPSCUpdateNormal();
}
}
