// NCreatureAI::IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight) -- whether the creature is
//   aware of the thing: the shared awareness test with CTCPerceiveThing::CanSeeOrSmellPerceivedThing as the perception predicate
// Leaf: ?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008645E0, 18 bytes, __fastcall (creature ecx, thing edx, the bool on the stack), `ret 4`:
//   `mov eax,[esp+4] ; push offset 0x00661600 ; push eax ; call 0x00864250 ; ret 4` (ecx / edx pass straight through).
// Name: Ego_r.pdb 0x00834AF0 + FableWin.pdb 0x02817D10 ?IsAwareOfThing@NCreatureAI@@YI_NPBVCThingAICreature@@PBVCThing@@_N@Z
//   (engine/NCreatureAI.h, "both original PDBs"); 15 landed callers bind it (0x009210A0 ... 0x00803280, the AI-state IsRunnable rows).
//   Callee: the anonymous-namespace IsAwareOfThingImplementation 0x00864250 (Ego_r.pdb 0x008348D0
//   ?IsAwareOfThingImplementation@?A0x4b0ddf32@@YI_NPBVCThingAICreature@@PBVCThing@@_NP8CTCPerceiveThing@@BE_NXZ@Z; landed, and its TU
//   carries this same wrapper) with the member predicate CTCPerceiveThing::CanSeeOrSmellPerceivedThing 0x00661600 (landed). The
//   anonymous-namespace name is TU-local, so the relocation names this TU's own `?A0x...` spelling.
//   NF95 2026-11-19 (q11325): was the stand-in fn_008645e0 calling h_008645e0 with a cast address (catalog CMainGameComponent::
//   BeginInputLoading, propagated).
// NF97 2026-11-19 (port): CTCPerceiveThing is the shared engine/CTCPerceiveThing.h class (FABLE_RETAIL_CTCPERCEIVETHING + _AWARENESS, whose
//   block holds the existing _CANSEEORSMELL declaration; single base CTCBase, so the member pointer stays 4 bytes), no longer a local model.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_CANSEEORSMELL
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/NCreatureAI.h"
#include "engine/CTCPerceiveThing.h"
namespace {
bool __fastcall IsAwareOfThingImplementation(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight,
                                             bool (CTCPerceiveThing::*is_aware)() const);   // 0x00864250
}
bool FABLE_FASTCALL NCreatureAI::IsAwareOfThing(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight)
{
    return IsAwareOfThingImplementation(creature, thing, check_line_of_sight, &CTCPerceiveThing::CanSeeOrSmellPerceivedThing);
}
