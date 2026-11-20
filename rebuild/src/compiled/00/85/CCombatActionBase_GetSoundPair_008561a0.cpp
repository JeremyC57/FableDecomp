// CCombatActionBase::GetSoundPair(const CThingCreatureBase& creature, const CCharString& name) -- protected static: the creature's own sound pair for the named sound (its virtual GetSoundPair, slot 89), returned through the caller's buffer
// Leaf: ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008561A0, __fastcall: hidden result in ecx, the creature in edx, the name on the stack (`ret 4`);
//   it forwards the same result buffer to the creature's virtual at +0x164 (slot 89) and returns it.
// NF123 (2026-11-20, q6403): re-spelled from the free stand-in ?GetSoundPair@@YIPAVCSoundPair@@PAV1@PAVCThingCreatureBase@@PAVCCharString@@@Z
//   (a 89-slot local CThingCreatureBase model and an explicit result pointer) to Ego_r.pdb
//   ?GetSoundPair@CCombatActionBase@@KI?AVCSoundPair@@ABVCThingCreatureBase@@ABVCCharString@@@Z (protected static __fastcall; the landed
//   0x008C3930 and 30 other combat-action constructors bind it), on the shared engine/CCombatActionBase.h (SOUNDCTOR opt-in) and
//   engine/CThingCreatureBase.h (CTHING_ORIGINAL_VTABLE: Ego_r.pdb ?GetSoundPair@CThingCreatureBase@@UBE?AVCSoundPair@@ABVCCharString@@@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_SOUNDCTOR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CSoundPair.h"
#include "engine/CCombatActionBase.h"

CSoundPair __fastcall CCombatActionBase::GetSoundPair(const CThingCreatureBase& creature, const CCharString& name)
{
    return creature.GetSoundPair(name);
}
