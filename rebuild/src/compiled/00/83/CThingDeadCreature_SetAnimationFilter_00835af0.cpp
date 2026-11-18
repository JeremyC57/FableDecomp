// CThingDeadCreature::SetAnimationFilter(const CAnimFilterBase& filter) -- store a clone of the death animation filter: PAnimFilter.Reset(filter.Clone())
// Flags: /Gr
// Leaf: ?SetAnimationFilter@CThingDeadCreature@@QAEXABVCAnimFilterBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00835AF0, 28 bytes, thiscall `ret 4`:
//   push esi ; mov esi,ecx ; mov ecx,[esp+8] ; mov eax,[ecx] ; call [eax+4] ; push eax ; lea ecx,[esi+0D0h] ; call 0x00836580 ; pop esi ; ret 4
// Identity: Ego_r.pdb / FableWin.pdb ?SetAnimationFilter@CThingDeadCreature@@QAEXABVCAnimFilterBase@@@Z (public); Ego_d.pdb has it out of
//   line in thing_dead_creature.obj (between CreateFromDef and Create), and retail 0x00835AF0 lies inside that TU's range (0x00835AA0
//   CreateFromDef, 0x00835AC0 DrawPreparePrimitivesForRendering, 0x00835B10 ~CThingDeadCreature). The body is SetAnimationFilter's:
//   CAnimFilterBase slot 1 (Clone) on the argument, then CCountedPointer<CAnimFilterBase>::Reset 0x00836580 on this+0xD0
//   (Ego_r.pdb PAnimFilter); NF80's Create 0x00835C80 expands the same body. The catalog name
//   NCreatureMode::CModeSitBase::RemoveModeWithAction was a label propagated over byte-similar bodies (it also sits on 0x008D7230,
//   0x008D7250, 0x00F35A10, 0x00860130).
// NF82 (q11004): re-authored from the CModeSitBase / CSubObject stand-in on engine/CThingDeadCreature.h (NF80's CREATE opt-in set plus the
//   new overlay opt-in FABLE_RETAIL_CTHINGDEADCREATURE_SETANIMATIONFILTER_OUTOFLINE, which declares the member for its owning row) and
//   engine/CAnimFilterBase.h / CCountedPointer.h; the Reset call now binds ?Reset@?$CCountedPointer@VCAnimFilterBase@@@@QAEXPAVCAnimFilterBase@@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_OPERATORNEW
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#define FABLE_RETAIL_CTHINGDEADCREATURE_CREATE
#define FABLE_RETAIL_CTHINGDEADCREATURE_SETANIMATIONFILTER_OUTOFLINE
#include "engine/CThing.h"
#include "engine/CThingDeadCreature.h"

void CThingDeadCreature::SetAnimationFilter(const CAnimFilterBase& filter)
{
    PAnimFilter.Reset(filter.Clone());
}
