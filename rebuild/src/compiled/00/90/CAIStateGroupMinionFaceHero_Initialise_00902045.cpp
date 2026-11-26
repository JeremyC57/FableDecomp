// CAIStateGroup_MinionFaceHero::Initialise(CAIBrain&, CThingAICreature&, long, EScriptAIPriority, const
// CCharString&) -- clear the hero pointer and the look-at frame, then the base initialisation.
// Leaf: ?Initialise@CAIStateGroup_MinionFaceHero@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00902045, length 89 bytes, `ret 14h`, frame pointer kept
// (`push ebp ; mov ebp,esp` with `leave`) and the size-optimised `and dword ptr [esi+1Ch],0` zero store:
// this translation unit was built with frame pointers and /Os, reproduced in-source with
// `#pragma optimize("y", off)` + `#pragma optimize("s", on)`. Vtable slot 1 of CAIStateGroup_MinionFaceHero (0x0128B3D4),
// its only retail reference.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90 (twice),
// GetPItem @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 on the converting temporary (vtable 0x01237F14
// = CIntelligentPointer<const CThingPlayerCreature>), then CAIStateGroupBase::Initialise @ 0x008FCEC0 with the five
// arguments pushed straight from the frame (`push dword ptr [ebp+..]`).
// `Hero = 0` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment shape; the zero
// store at +0x1c is scheduled between the argument pushes.
// Identity (NF476, T3 nf374 binder): Ego_r.pdb / FableWin.pdb ?Initialise@CAIStateGroup_MinionFaceHero@@UAE...JW4EScriptAIPriority@@...@Z
//   (was ...JJ...: the HEAD row's local `long` priority). The class is now the header's retail view (engine/CAIStateGroup_MinionFaceHero.h
//   FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO + the add-only FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO_INITIALISE slot-1 declaration;
//   Ego_r members Hero +0x14, FrameLastLookedAtHero +0x1c), so the base call binds the landed self
//   ?Initialise@CAIStateGroupBase@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z (was the stand-in
//   ?Initialise@CAIStateGroupBase@@QAE..JJ..@Z) and the temporary is the PDB CIntelligentPointer<const CThingPlayerCreature> (the HEAD row's
//   local non-template CIntelligentPointer / CBaseIntelligentPointer / CAIStateGroupBase models are gone).
#include "fable_compiler.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO
#define FABLE_RETAIL_CAISTATEGROUP_MINIONFACEHERO_INITIALISE
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroup_MinionFaceHero.h"

void CAIStateGroup_MinionFaceHero::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    Hero = 0;
    FrameLastLookedAtHero = 0;
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
}
// Fidelity: scaffolding, not retail (R-NF409-2 row-end optimize reset): VC7.1 compiles the CIntelligentPointer<const CThingPlayerCreature>
//   template instances at the end of the TU in the optimize mode in force there; this reset gives their pick-any copies the command-line
//   /O2 /Oy mode (the owner row 0x006892A0 has no CompilerFlags override and no size pragma), and changes neither the leaf nor anything
//   spelled in the row text (NF476 batch b1 evidence/pk/pk_00902045_noreset.txt: without it ??0..@PBVCThingPlayerCreature@@@Z is NEQ at 0x006892A0, frame
//   pointer only; with it EQ).
#pragma optimize("", on)
