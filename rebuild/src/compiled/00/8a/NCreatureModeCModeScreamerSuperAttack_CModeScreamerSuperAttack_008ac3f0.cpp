// NCreatureMode::CModeScreamerSuperAttack::CModeScreamerSuperAttack(CThing&, const CThing&) -- mode ctor: filter at +0x68, target pointer at +0x74, empty thing pointer at +0x7c
// Leaf: ??0CModeScreamerSuperAttack@NCreatureMode@@QAE@AAVCThing@@ABV2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AC3F0, 160 bytes (padding included).
// Label correction: the catalog row says CModeBriarRoseTeleportInvulnerable, but the vtable store
// 0x01278338 is NCreatureMode::CModeScreamerSuperAttack (rtti-vtables.tsv).
// Shape: base mode ctor @ 0x00861E00, vtable 0x01278338, CAnimFilterOfGroup at +0x68 from the
// literal @ 0x01278368 (inline ctor: default string 0x0099E4B0, assign 0x0099EFB0, CRC 0x004014A0;
// temporary freed by 0x0099EAE0), then two CIntelligentPointer members over the out-of-line CBaseIntelligentPointer ctor (0x00A01B10):
// PTarget at +0x74 set from the target (SetPItem 0x00A01B90) and PLightningBeamParticleEmitter at +0x7c left empty.
// Naming (NF157, 2026-11-20; PRIORITY q11881): the row-local models (CModeBase_00861e00, CAnimFilterOfGroup, CIntelligentPointerCreature /
//   CIntelligentPointerThing, the mode class itself) replaced by the shared engine/NCreatureMode_CModeScreamerSuperAttack.h (over
//   CModeStandBase.h / CTCCreatureModeManager.h / CAnimFilterOfGroup.h with its retail inline constructor / CIntelligentPointer.h). The leaf is
//   Ego_r.pdb 0x008EA130 / FableWin.pdb 0x02AABF30 ??0CModeScreamerSuperAttack@NCreatureMode@@QAE@AAVCThing@@ABV2@@Z (was the stand-in
//   ...@PAVCThing@@PAVCThingCreatureBase@@@Z); its only landed caller 0x008A08A0 already binds it (NF147). PTarget's vtable 0x01237F1C is
//   .?AV?$CIntelligentPointer@$$CBVCThing@@@@ (RTTI type descriptor read from Fable.exe), the header's CIntelligentPointer<const CThing>, not the
//   CThingCreatureBase the old model named. The base ctor 0x00861E00 now binds ??0CModeStandBase@@QAE@AAVCThing@@@Z (both PDBs; was
//   ??0CModeBase_00861e00@@QAE@PAVCThing@@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CModeStandBase.h"
#include "engine/NCreatureMode_CModeScreamerSuperAttack.h"

namespace NCreatureMode {
CModeScreamerSuperAttack::CModeScreamerSuperAttack(CThing& thing, const CThing& target)
    : CModeStandBase(thing)
    , Filter("SUPER_ATTACK")                   // literal @ 0x01278368 (masked)
    , PTarget(&target)
{
}
}
