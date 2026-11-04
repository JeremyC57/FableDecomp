// NUISystem::CParticleEffect::`vector deleting destructor'(unsigned int) `adjustor{24}' -- CActionParam-subobject (+0x18) vtable entry: this -= 0x18, jump to the deleting destructor 0x005440B0
// Leaf: ??_ECParticleEffect@NUISystem@@WBI@AEPAXI@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005440A0, 8 bytes: `sub ecx,18h ; jmp 0x005440B0`. Compiler-generated adjustor thunk.
// Identity (two sources): slot 0 of CParticleEffect's CActionParam vtable 0x01247330 at +0x18 (rtti-vtables.tsv, RTTI .?AVCParticleEffect@NUISystem@@;
//   the catalog's "vfunc_0"), and its jump target 0x005440B0 is slot 0 of the primary vtable 0x0124735C, the class's ??_G. VC7.1 names
//   the slot-0 entry ??_E (a weak alias of ??_G when the TU has no array new) and spells the thunk with the adjustor (W3 = 4,
//   WBI@ = 24); objdump -t of this object shows the leaf.
// TU: the thunks, the vtables and the ??_G are emitted where the vtable is first needed; this source is the constructor's TU: CParticleEffect(long) (0x00544010: CChangingStateComponent(long) 0x0052CC50, the vptrs, PHandle cleared, GetDataFromDef 0x00543FA0).
//   That function is not graded here (it is its own row); the class's code precedes the thunks and the ??_G in retail.
// Callees: ??_GCParticleEffect@NUISystem@@UAEPAXI@Z 0x005440B0 (bound as its ??_E alias).
// Header: engine/NUISystem_CParticleEffect.h.
// Twin: 0x00544090 is the other adjustor thunk of the same vtable set (same TU).
#include "engine/NUISystem_CParticleEffect.h"
namespace NUISystem {
CParticleEffect::CParticleEffect(long def)
    : CChangingStateComponent(def)
    , PHandle(0)
{
    GetDataFromDef();
}
}