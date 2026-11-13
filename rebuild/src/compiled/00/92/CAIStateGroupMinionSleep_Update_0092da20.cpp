// CAIStateGroup_MinionSleep::Update() -- a minion goes to sleep in a free bed (own, then bandit; else terminate): note whether its home building is alive, require a CTCSleep (else terminate), pick the bed side and, when the creature has the BED_GET_IN_<side> animation, the get-in / get-out / fast get-out (falling back to the get-out) / sleep animation indices; take the bed's CBedDef, pick a random get-in animation entry; when the sleep component is fast-tracked, note and sheathe the weapons (dropping creature mode 4), reset position and orientation to the bed and interrupt navigation; sheathe weapons when carrying one, get into bed; then idle while it is sleep time, the creature is ready to sleep, the hero is not seen trespassing and the home (if it was alive) still stands; on a disturbance with a fast get-up animation on the def, get out fast, otherwise get out normally (each starting the bed's get-out animation when the bed is alive); unsheathe the weapons if they were carried; terminate
// Leaf: ?Update@CAIStateGroup_MinionSleep@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x0092DA20, 1802 bytes, plain `ret`, frame 0x150 (extent checked: int3 padding before, last `ret`
//   at 0x0092E129, next function 0x0092E130 CAIStateGroup_MinionSleep::IsRunnable). Ego_r.pdb ?Update@CAIStateGroup_MinionSleep@@UAEXXZ
//   (donor 0x00950F30, the class's last member) = slot 12 of vtable 0x0128B140 (rtti-vtables.tsv); FableWin.pdb the same spelling.
// Callees (each checked against retail with tmp/rel.py): FindFreeBed 0x0092D1F0 / FindFreeBanditBed 0x0092C8A0 / IsHeroTrespassingPerceived
//   0x0092CB30 (CAIStateGroup_GoToBed), Terminate 0x008FCF40, PeekWorld 0x008FCFA0, GetWorld 0x008FCFC0, AIAction_PerformActionAndWait
//   0x0097DB70, AIAction_DoNothing 0x0097E200 (CAIStateGroupBase), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound
//   0x0040F020 / const 0x004365B0 (the expanded GetTC / PeekTC: 0xD4 CTCSleep, 0x35 CTCBed, 0x31 CTCCreatureModeManager, 7
//   CTCCreatureNavigation), CCharString operator=(const char*) 0x0099EFE0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0 / operator+(const char*, const CCharString&) 0x0099F690 / operator+(const CCharString&, const char*) 0x0099F600,
//   CThingCreatureBase::HasAnim 0x00662C70 / GetAnimFromName 0x00662A00 / GetRandomAnimFromNameWithFilter 0x00662EF0 / SheatheAllWeapons
//   0x006647F0, CDefStringTable::GetString 0x009D49B0 on GDefStringTable 0x013CA828 (the inline CDefString conversion), CTCSleep::GetFastTrack
//   0x0092C3B0, CTCCarrying::IsThingCarryingWeapon 0x006A1540, CTCCreatureModeManager::RemoveMode 0x0073ABA0, CTCPhysicsBase::ResetPosition /
//   ResetRHSet (virtual +0x7C / +0x80 on CThing::PhysicsTC +0x60), CTCCreatureNavigation::InterruptNavigation 0x006CB6B0,
//   CCreatureAction_SheatheWeapons 0x008CC370 / CCreatureAction_UnsheatheWeapons 0x008B8590 (~CCombatActionBase 0x0062E900),
//   CCreatureAction_GetInBed 0x0092E300 (the inline ctor's out-of-line COMDAT of this TU) / CCreatureAction_GetOutOfBed 0x00860060
//   (~CBaseIntelligentPointer 0x00A01C10, ~CCreatureActionBase 0x00693EF0), CWorld::GetFrame 0x0049D870, CMainGameComponent::ConstantFPS
//   0x01375550, CThingAICreature::GetSleepTimes 0x00833390, CGameTimeManager::IsTimeBetween(long, long) 0x006BB390, CDefString::operator!=
//   0x005FA740, IS_THING_ALIVE<CIntelligentPointer<CThing> > 0x004CC340 (called at the fast site, expanded at the normal one),
//   CTCBed::StartGetOutOfBedAnimation 0x00746A70. Literals: "RIGHT" 0x01238494, "LEFT" 0x01238484, "BED_GET_IN_" 0x01291D40,
//   "BED_GET_OUT_" 0x01291D30, "_FAST" 0x01287CC4, "BED_SLEEP" 0x01291D24, "" 0x0122D70E; vtables CAnimFilterAnyGroup 0x0127261C /
//   CAnimFilterBase 0x0122FFCC.
// Shape: (1) the bed side is an if / else of two assignments (VC7.1 cross-jumps the operator= call: `push "RIGHT" ; jmp` / `push "LEFT"`,
//   with the GetTC-failure exit block placed between, 0x0092DAC9..0x0092DAE5); the ternary loads the chosen literal into eax first. (2) "BED_SLEEP" is an
//   implicit conversion temporary (built before the creature is loaded and the pushes, 0x0092DBE7); an explicit CCharString temporary loads
//   the creature first. (3) The two wake-up paths hang on a local `disturbed` flag that VC7.1 threads through the loop exits: the
//   sleep-time and home-building exits jump straight to the normal get-out (0x0092E00C), the others to the fast-name test (0x0092DF3E);
//   under `disturbed && name != ""` (then-frequency 0.25) IS_THING_ALIVE(PBed) is called (0x004CC340), in the else (0.75) expanded.
//   (4) MinionSleep::IsInSleepTime (0x0092C520) is defined in this TU before Update and expanded in the loop. (5) The CDefPointer to
//   the bed def is copied from the const CTCBed (GetDefinition), dereferenced on every path, so its release has no null test (0x0092E113).
// Layout: shared headers with the chunk 2026 overlay (hdr/README.md): retail CAIStateGroup_MinionSleep (: CAIStateGroup_GoToBed,
//   WasCarryingWeapon +0x60), new CTCBed.h / CBedDef.h / CCreatureAction_SheatheWeapons.h, retail opt-ins for CCreatureAction_GetInBed
//   (inline ctor) / GetOutOfBed / UnsheatheWeapons, CTCSleep READY, CThingCreatureBase BEDSLEEP, CThingAICreature SLEEPTIMES,
//   CGameTimeManager ISTIMEBETWEEN, CDefString NOTEQUAL, CCreatureActionBase ANIMCTOR (CAIStateGroupBase.h is the chunk 2017 copy).
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

inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }

bool CAIStateGroup_MinionSleep::IsInSleepTime() const
{
    long start, end;
    const CGameTimeManager& time = GetWorld().GetGameTimeManager();
    GetCreature().GetSleepTimes(start, end);
    return time.IsTimeBetween(start, end + SleepEndTimeVariation);
}

void CAIStateGroup_MinionSleep::Update()
{
    if (!FindFreeBed(true) && !FindFreeBanditBed()) {
        Terminate();
        return;
    }
    bool home_alive = IS_THING_ALIVE(GetCreature().GetHomeBuilding());
    CTCSleep* sleep;
    if (!GetCreature().GetTC(&sleep)) {
        Terminate();
        return;
    }
    PlayEmergencyGetOutOfBed = false;
    if (BedToRightOfCreature)
        BedSide = "RIGHT";
    else
        BedSide = "LEFT";
    BedGetOutOfBedFastAnim = 0;
    long get_in_anim = 0;
    long get_out_anim = 0;
    long get_out_fast_anim = 0;
    long sleep_anim = 0;
    if (GetCreature().HasAnim("BED_GET_IN_" + BedSide, CAnimFilterAnyGroup())) {
        get_in_anim = GetCreature().GetAnimFromName("BED_GET_IN_" + BedSide, 0).BankIndex;
        get_out_anim = GetCreature().GetAnimFromName("BED_GET_OUT_" + BedSide, 0).BankIndex;
        get_out_fast_anim = GetCreature().GetAnimFromName("BED_GET_OUT_" + BedSide + "_FAST", 0).BankIndex;
        if (!get_out_fast_anim)
            get_out_fast_anim = get_out_anim;
        sleep_anim = GetCreature().GetAnimFromName("BED_SLEEP", 0).BankIndex;
        BedGetOutOfBedFastAnim = get_out_fast_anim;
    }
    const CTCBed* bed = 0;
    PBed->PeekTC(&bed);
    CDefPointer<const CBedDef> bed_def = bed->GetDefinition();
    const CAnimationEntry& get_in_entry = GetCreature().GetRandomAnimFromNameWithFilter(bed_def->GetInBedAnimName, CAnimFilterAnyGroup());
    if (sleep->GetFastTrack()) {
        WasCarryingWeapon = CTCCarrying::IsThingCarryingWeapon(GetCreature());
        if (WasCarryingWeapon) {
            GetCreature().SheatheAllWeapons();
            CTCCreatureModeManager* modes;
            if (GetCreature().GetTC(&modes))
                modes->RemoveMode(4);
        }
        GetCreature().GetPhysicsTC()->ResetPosition(BedPos);
        GetCreature().GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), Rhset.GetForward()));
        CTCCreatureNavigation* navigation = 0;
        GetCreature().GetTC(&navigation);
        navigation->InterruptNavigation();
    }
    WasCarryingWeapon = CTCCarrying::IsThingCarryingWeapon(GetCreature());
    if (WasCarryingWeapon)
        AIAction_PerformActionAndWait(CCreatureAction_SheatheWeapons(GetCreature()), false);
    PlayEmergencyGetOutOfBed = true;
    AIAction_PerformActionAndWait(CCreatureAction_GetInBed(GetCreature(), *PBed, get_in_entry, bed_def->SleepInBedAnimName,
                                                           get_in_anim, sleep_anim), false);
    unsigned long last_frame = PeekWorld().GetFrame();
    bool disturbed = true;
    while (AIAction_DoNothing() == 4) {
        if (PeekWorld().GetFrame() - last_frame > 4 * CMainGameComponent::GetConstantFPS())
            last_frame = PeekWorld().GetFrame();
        if (!IsInSleepTime()) {
            disturbed = false;
            break;
        }
        if (!sleep->GetIsReadyToSleep() || IsHeroTrespassingPerceived())
            break;
        if (home_alive && !IS_THING_ALIVE(GetCreature().GetHomeBuilding())) {
            disturbed = false;
            break;
        }
    }
    if (disturbed && bed_def->GetUpFromBedFastAnimName != "") {
        if (IS_THING_ALIVE(PBed)) {
            CTCBed* bed_tc;
            if (PBed->GetTC(&bed_tc) && get_out_fast_anim)
                bed_tc->StartGetOutOfBedAnimation(get_out_fast_anim);
        }
        if (AIAction_PerformActionAndWait(CCreatureAction_GetOutOfBed(GetCreature(), *PBed, bed_def->GetUpFromBedFastAnimName), false) == 2)
            PlayEmergencyGetOutOfBed = false;
    } else {
        if (IS_THING_ALIVE(PBed)) {
            CTCBed* bed_tc;
            if (PBed->GetTC(&bed_tc) && get_out_anim)
                bed_tc->StartGetOutOfBedAnimation(get_out_anim);
        }
        if (AIAction_PerformActionAndWait(CCreatureAction_GetOutOfBed(GetCreature(), *PBed, bed_def->GetUpFromBedAnimName), false) == 2)
            PlayEmergencyGetOutOfBed = false;
    }
    if (WasCarryingWeapon) {
        AIAction_PerformActionAndWait(CCreatureAction_UnsheatheWeapons(GetCreature()), false);
        WasCarryingWeapon = false;
    }
    Terminate();
}