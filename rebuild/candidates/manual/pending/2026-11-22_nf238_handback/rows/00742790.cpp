// NCreatureMode::CModeSitBench::Alloc(CThing& owner) -- mode factory: new CModeSitBench(owner)
// Leaf: ?Alloc@CModeSitBench@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00742790, 31 bytes, static __fastcall (ecx = owner).
// Retail bytes:  push esi ; push 0x38 ; mov esi,ecx ; call operator new ; test eax,eax ; pop ecx ; je N ; push esi ; mov ecx,eax ; call CModeSitBench::CModeSitBench ; pop esi ; ret ; N: xor eax,eax ; pop esi ; ret
// Allocates sizeof(CModeSitBench) = 0x38 bytes (operator new 0x00BFEA1A), null-checks the block and runs the out-of-line constructor
// 0x00740040 on it with the owner, returning the new object (or null).
// NF238 (2026-11-22): ported off its row-local `struct CModeSitBench { _body[0x38]; CModeSitBench(CThing*); }` and free-function leaf
//   (?CModeSitBench_Alloc@@...) onto the shared engine/NCreatureMode_CModeSitFloor.h (FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27 class,
//   sizeof 0x38 asserted there) with the add-only opt-in FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_ALLOC. Names (Ego_r.pdb / FableWin.pdb /
//   Ego_d.pdb): ?Alloc@CModeSitBench@NCreatureMode@@SIPAVCModeBase@CTCCreatureModeManager@@AAVCThing@@@Z (the spelling its only binder,
//   CTCCreatureModeManager::RegisterModes 0x0073B130, already binds) and the constructor ??0CModeSitBench@NCreatureMode@@QAE@AAVCThing@@@Z.
//   The shape follows the landed sibling factory 0x00742460 (CModeBasic::Alloc).
// Callees: operator new 0x00BFEA1A, NCreatureMode::CModeSitBench::CModeSitBench(CThing&) 0x00740040.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_PASS27
#define FABLE_RETAIL_NCREATUREMODE_CMODESITBENCH_ALLOC
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/NCreatureMode_CModeSitFloor.h"

namespace NCreatureMode {

CTCCreatureModeManager::CModeBase* __fastcall CModeSitBench::Alloc(CThing& owner)
{
    return new CModeSitBench(owner);
}

}
