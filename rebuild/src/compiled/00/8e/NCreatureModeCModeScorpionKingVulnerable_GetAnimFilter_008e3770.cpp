// NCreatureMode::CModeScorpionKingVulnerable::GetAnimFilter() const -- returns the mode's own animation filter (Filter)
// Leaf: ?GetAnimFilter@CModeScorpionKingVulnerable@NCreatureMode@@UBEABVCAnimFilterBase@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E3770, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 4 of NCreatureMode::CModeScorpionKingVulnerable's vtable 0x0128582C (the slot holds this body in 1 vtable(s), owner NCreatureMode::CModeScorpionKingVulnerable); the Xbox 360 Test PDB virtual order (third source) aligned on 7 TLC-named slots of that vtable puts GetAnimFilter(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetAnimFilter@CModeScorpionKingVulnerable@NCreatureMode@@UBEABVCAnimFilterBase@@XZ; `ret` matches the signature.
// Was: NCreatureMode::CModeScorpionKingVulnerable::vfunc_4 (rebuild/src/compiled/00/8e/gapscan_sub008e3770_008e3770.cpp).
// Opt-ins as in the landed row NCreatureModeCModeScorpionKingVulnerable_GetType_008e3760.cpp.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CMODEBASE_PARENTTHING
#define FABLE_RETAIL_CMODESTANDBASE
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_DUMMYOBJECT
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOSS_EFFECTS
#define FABLE_RETAIL_LOOK_HEAD_ANIM
#define FABLE_RETAIL_CTCHITLOCATIONS_FROMNAME
#define FABLE_RETAIL_CHITLOCATION_SETENABLED
#define FABLE_RETAIL_CCREATUREACTION_SCORPIONSUMMON_CLASS
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/ThingAlive.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_ScorpionSummon.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CTCBoss.h"
#include "engine/CTCAnimationComplex.h"
#include "engine/CAnimateThing.h"
#include "engine/CTCHitLocations.h"
#include "engine/NHitLocations_CHitLocation.h"
#include "engine/NCreatureMode_CModeScorpionKingVulnerable.h"
const CAnimFilterBase& NCreatureMode::CModeScorpionKingVulnerable::GetAnimFilter() const
{
    return Filter;
}
