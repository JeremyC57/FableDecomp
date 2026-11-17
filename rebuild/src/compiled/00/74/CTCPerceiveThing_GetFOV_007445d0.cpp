// CTCPerceiveThing::GetFOV() const -- the perceiver's field of view as a fraction of a full turn: the def's FieldOfView (degrees) / 360
// Leaf: ?GetFOV@CTCPerceiveThing@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007445D0, 13 bytes:
//   mov eax,[ecx+34h] ; fld dword ptr [eax+28h] ; fmul dword ptr [01238E00h] ; ret
// Name-fix (agentNF55, round 7; was the descriptive `<unattributed>::GetScaledDefValue` on a local CDefUser_Retail model):
//   Ego_r.pdb 0x00757070 and FableWin.pdb 0x0258DA60 ?GetFOV@CTCPerceiveThing@@QBEMXZ, and the retail placement is the PDB
//   source order: Ego_r GetTimeBasedVisibilityMultiplier 0x00757010 / GetFOV 0x00757070 / ??0CTCPerceiveThing 0x00757090 =
//   retail 0x00744580 / 0x007445D0 / 0x007445E0 (both neighbours landed under those names). Layout: engine/CTCPerceiveThing.h
//   (PDef +0x34) and engine/CPerceivedThingDef.h (FieldOfView +0x28, degrees); the callers CThingCreatureBase::IsThingInFOV
//   0x00665190 / IsPosInFOV 0x00665260 compare the result against half the facing-angle fraction.
// Shape: `/ 360.0f`, as the same body is spelled where 0x00745120 CTCPerceiveThing::UpdateLineOfSight expands it; VC7.1 emits the
//   multiply by the pooled 1/360 (0x01238E00 = 0.0027777778f).
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_HEARING
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CPerceivedThingDef.h"

float CTCPerceiveThing::GetFOV() const
{
    return PDef->FieldOfView / 360.0f;
}
