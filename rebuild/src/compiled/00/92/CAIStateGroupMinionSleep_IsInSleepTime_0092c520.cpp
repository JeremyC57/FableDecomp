// CAIStateGroup_MinionSleep::IsInSleepTime() const -- is the world clock inside the creature's sleep window, the end extended by
// SleepEndTimeVariation: GetWorld (0x008FCFC0) game-time manager, CThingAICreature::GetSleepTimes (0x00833390), CGameTimeManager::IsTimeBetween
// (0x006BB390)
// Leaf: ?IsInSleepTime@CAIStateGroup_MinionSleep@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C520, length 64 bytes.
// NF39 (2026-11-16): moved off its `CAIStateGroup_GoToBed::IsInSleepTimeWithWakeDelay` stand-in (local CAIStateGroupBase / CWorld / CGameTime
//   models) onto the shared engine/CAIStateGroup_MinionSleep.h / CAIStateGroup_GoToBed.h (Ego_r.pdb SleepEndTimeVariation +0x24),
//   CAIStateGroupBase.h, CWorld.h and CGameTimeManager.h. Identity: Ego_r.pdb donor order InitStateGroup MS/GTB, InitCreaturePosition MS,
//   IsInSleepTime GTB/MS, IsStillValid GTB/MS = retail 0x0092C440/0x0092C490/0x0092C4D0/0x0092C4E0/0x0092C520/0x0092C560/0x0092C5A0 (vtable
//   slots confirm the virtual ones); FableWin.pdb ?IsInSleepTime@CAIStateGroup_MinionSleep@@QBE_NXZ. The landed MinionSleep::Update 0x0092DA20
//   defines this same body in its TU and expands it in the loop (CGameTimeManager.h: "the inline IsInSleepTime 0x0092C520").
// Defines: the landed 0x0092DA20 Update preamble (the body is that TU's definition).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_NOTEQUAL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMFILTER
#define FABLE_RETAIL_CTHINGCREATUREBASE_BEDSLEEP
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SLEEPTIMES
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_GOTOBED
#define FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_ANIMCTOR
#define FABLE_RETAIL_CCREATUREACTION_GETINBED
#define FABLE_RETAIL_CCREATUREACTION_GETOUTOFBED
#define FABLE_RETAIL_CCREATUREACTION_UNSHEATHEWEAPONS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCSLEEP_READY
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCCREATUREMODEMANAGER
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CGAMETIMEMANAGER_ISTIMEBETWEEN
#define FABLE_RETAIL_IS_THING_ALIVE
#include "engine/CAIStateGroupBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CDefString.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_GetInBed.h"
#include "engine/CCreatureAction_GetOutOfBed.h"
#include "engine/CCombatActionBase.h"
#include "engine/EUnsheatheWeaponType.h"
#include "engine/CCreatureAction_UnsheatheWeapons.h"
#include "engine/CCreatureAction_SheatheWeapons.h"
#include "engine/CTCSleep.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CGameTimeManager.h"
#include "engine/CBedDef.h"
#include "engine/CTCBed.h"
#include "engine/CAIStateGroup_MinionSleep.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIBrain.h"
#include "engine/CDefPointer.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterAnyGroup.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CThingBuilding.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/ThingAlive.h"

bool CAIStateGroup_MinionSleep::IsInSleepTime() const
{
    long start, end;
    const CGameTimeManager& time = GetWorld().GetGameTimeManager();
    GetCreature().GetSleepTimes(start, end);
    return time.IsTimeBetween(start, end + SleepEndTimeVariation);
}
