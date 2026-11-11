// CAIStateGroup_DeedReactionRoot::IsRunnable() -- runnable when there is a deed to react to: first a fresh virtual deed from an opinion source (the world's exclusive source, else the nearest one in a perception-sized box), else the creature's head pending hero deed, whose reactee is the hero (an always-aware source not blocked by following) or the perceived thing the creature is aware of
// Leaf: ?IsRunnable@CAIStateGroup_DeedReactionRoot@@UAE_NXZ
// Retail: Fable.exe (TLC, VC7.1 RTM) @ 0x009609B0, 1352 bytes, plain `ret` (virtual, vtable slot 11 IsRunnable). Ego_r.pdb / FableWin.pdb
//   ?IsRunnable@CAIStateGroup_DeedReactionRoot@@UAE_NXZ; the landed FromSleep override 0x00960F00 calls this body non-virtually.
// Retail-asm: 0x00960A31..0x00960A60 `mov [ebp-18h],41700000h ; mov [ebp-14h],3F000000h ; mov [ebp-10h],3F800000h ; fld [ebp-18h] ;
//   fadd [ebp-14h] ; fistp dword ptr [ebp-8] ; fild [ebp-8] ; fsub [ebp-10h] ; fstp [ebp-1Ch] ; mov eax,[ebp-18h] ; cmp eax,[ebp-1Ch] ;
//   jne ; dec dword ptr [ebp-8]` (15.0f) and 0x00960AA3..0x00960ACB (the GetExtendedSightRadius result) -- the engine's inline-assembly
//   ceiling helper expanded in place (0.5f / 1.0f frame constants, a bare dword fistp into a named long, the integer compare and the
//   `dec` on memory in one block). VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`,
//   /arch:SSE2 cvttss2si: the rasm1 probe families recorded in engine/GFFloatToLongCeil.h); the asm is also why VC7.1 gives this function
//   its ebp frame. The row reaches it only through the shared helper header engine/GFFloatToLongCeil.h (GFFloatToLongCeil, FABLE_PORTABLE
//   twin and dual-path test there); no asm in this source. Expected grade retail_asm.
// Callees: CAIStateGroupBase::PeekWorld 0x008FCFA0, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 /
//   dtor 0x00A01C10, CThingFilter_IsOpinionSourceWithFreshDeed::operator() 0x00960FC0, CTCPerceiveThing::GetExtendedSightRadius 0x00744950 /
//   GetPerceivedThing 0x007446A0 / IsAwareOfThing 0x00643B3B, CThing::GetPos 0x004C73D0, __ftol2 0x00BFEA70, CThingPhysical::PeekPos
//   0x006A4D00, CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_IsOpinionSourceWithFreshDeed,CThingCompare_Nearest,const CThing>
//   0x00961080 (same TU), CVectorMap<ETCInterfaceType,CTCBase*,..>::LowerBound 0x0040F020 / const 0x004365B0,
//   CTCCreatureOpinionOfHero::GetHeadPendingDeed 0x00683770, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature
//   0x00487DC0, CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630, CTCFollowed::IsFollower 0x006D4D70,
//   CTCRegionFollower::IsRegionFollower 0x006AEB60, CIntelligentPointer<const CThing>(const CThing*) 0x0056C860 / operator= 0x005ED300.
//   Global: CMainGameComponent::PMainGameComponent 0x013B86A0. Vtables: CIntelligentPointer<CThingAICreature> 0x0123E7E0,
//   CIntelligentPointer<const CThing> 0x01237F1C (RTTI type descriptors read from Fable.exe).
// Layout: shared headers only. CAIStateGroup_DeedReactionRoot / CThingFilter_IsOpinionSourceWithFreshDeed / CTCOpinionSource /
//   CTCRegionFollower are new overlay headers; CTCPerceiveThing / CTCCreatureOpinionOfHero / COpinionReactionManager gain declaration
//   opt-ins (hdr/README.md). TC kinds: 0x34 CTCPerceiveThing, 0x4F CTCCreatureOpinionOfHero, 0x50 CTCOpinionSource,
//   0x51 CTCHeroOpinionDeedLog, 0x7A CTCRegionFollower, 0xBC CTCFollowed.
// Shape: the search's body is visible (FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY, the instantiation retail emits next in the TU):
//   VC7.1 then knows it does not capture `&thing`, so `thing` stays in edi with its frame home at [ebp-0Ch] (declaration only: kept in
//   memory, 1347 bytes). The two `ThingReactingTo = ...` sites are one inline split decided by VC7.1 itself (the first expands the
//   ctor and the inline ASSIGN operator=, the second, colder one calls 0x0056C860 / 0x005ED300); the hero / perceived-thing arms end in
//   the same statement, which c2 cross-jumps (`push edi ; jmp` into the perceived arm's copy, 0x00960DA4). The target test is the
//   FableWin free comparison `deed->PTarget == &GetCreature()` (EQUAL_RAW: the creature is an argument, loaded before GetPItem, then
//   `cmp eax,edi`); `&GetCreature() == deed->PTarget.Get()` gives `cmp edi,eax`. The two conditional CDefPointer temporaries share the
//   flag word [ebp-4] (bits 1 and 2), each released before its bool is tested. The box is C2DBoxI(GetPos().ToC2DCoordI(), range)
//   (Ego_r.pdb ?ToC2DCoordI@C3DVector@@QBE?AVC2DCoordI@@XZ, inline: the two __ftol2 calls at 0x00960AE0 / 0x00960AEB).
#define FABLE_RETAIL_CAISTATEGROUP_DEEDREACTIONROOT
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_HEADPENDINGDEED
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCOPINIONSOURCE
#define FABLE_RETAIL_CTCREGIONFOLLOWER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHINGFILTER_ISOPINIONSOURCEWITHFRESHDEED
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_PEEKSOURCE
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "stlport/fable_map.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCOpinionSource.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCRegionFollower.h"
#include "engine/CThingFilter_IsOpinionSourceWithFreshDeed.h"
#include "engine/CAIStateGroup_DeedReactionRoot.h"
#include "engine/ThingAlive.h"
#include "engine/GFFloatToLongCeil.h"


bool CAIStateGroup_DeedReactionRoot::IsRunnable()
{
    const CThing* thing = PeekWorld().PeekOpinionReactionManager().PeekThingOpinionSource();
    CThingFilter_IsOpinionSourceWithFreshDeed filter(&GetCreature(), LastOpinionSourceVirtualDeedID, LastThingOpinionSourceUID);
    if (thing == 0 || !filter(*thing)) {
        long range = GFFloatToLongCeil(15.0f);
        const CTCPerceiveThing* perceive;
        if (GetCreature().PeekTC(&perceive))
            range = GFFloatToLongCeil(perceive->GetExtendedSightRadius());
        C2DBoxI area(GetCreature().GetPos().ToC2DCoordI(), range);
        PeekWorld().PeekThingSearchTools().GetPBestThingInRoughArea(area, filter, CThingCompare_Nearest(GetCreature().PeekPos()), &thing);
    }
    if (thing) {
        const CTCOpinionSource* source;
        thing->PeekTC(&source);
        if (thing != &GetCreature()) {
            ThingReactingTo = thing;
            DeedIsVirtual = true;
            CurrentDeedType = source->PeekVirtualDeed();
            CurrentDeedHandle = source->PeekVirtualDeedID();
            return true;
        }
    }
    CTCCreatureOpinionOfHero* opinion;
    if (GetCreature().GetTC(&opinion)) {
        const COpinionDeed* deed = opinion->GetHeadPendingDeed();
        if (deed) {
            CThingPlayerCreature* hero = CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            if (IS_THING_ALIVE(hero)) {
                const CTCHeroOpinionDeedLog* deed_log;
                bool blocked = hero->PeekTC(&deed_log) && deed_log->PeekPDeedEffectsDef(deed->DeedType)->BlockWhileFollowingPlayer;
                if (blocked) {
                    const CTCFollowed* followed;
                    if (hero->PeekTC(&followed) && followed->IsFollower(&GetCreature()))
                        return false;
                    const CTCRegionFollower* region_follower;
                    if (hero->PeekTC(&region_follower) && region_follower->IsRegionFollower(&GetCreature()))
                        return false;
                }
            }
            const CTCOpinionSource* hero_source;
            if (IS_THING_ALIVE(hero) && hero->PeekTC(&hero_source) && hero_source->IsAlwaysAwareOf()) {
                ThingReactingTo = hero;
            } else {
                const CTCPerceiveThing* perceive;
                if (!GetCreature().PeekTC(&perceive) || !perceive->GetPerceivedThing() || !perceive->IsAwareOfThing()
                    || !perceive->IsThingIgnoreFOVLineOfSight())
                    return false;
                ThingReactingTo = perceive->GetPerceivedThing();
            }
            const CTCHeroOpinionDeedLog* reactee_log;
            bool reacts = ThingReactingTo->PeekTC(&reactee_log)
                && (!reactee_log->PeekPDeedEffectsDef(deed->DeedType)->ReactionRestrictToTarget || deed->PTarget == &GetCreature());
            if (reacts) {
                DeedIsVirtual = false;
                CurrentDeedType = deed->DeedType;
                CurrentDeedHandle = deed->DeedHandle;
                return true;
            }
        }
    }
    return false;
}
