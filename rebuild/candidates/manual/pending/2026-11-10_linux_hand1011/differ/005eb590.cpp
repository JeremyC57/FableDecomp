// CTCTavernGame::GivePrize() -- give the hero the tavern game's prize object and renown, tell every creature around the table owner that the hero won (opinion deed TAVERN_GAME_WIN), and mark the prize given
// Grade: DIFFER(631v631) -- 22 diff lines, register tie: the def pointer's reload piece (c2prio #16, prio 46) is coloured before player / prize (prio 32) and takes esi, so prize gets edi and the def load sinks below the player push; retail has def in edi (loaded before the Create pushes) and prize in esi. Levers that moved it: hero_stats in its own {} block (frame 0x58 -> 0x54), the implicit "" -> CCharString conversion (`lea` push of the temporary)
// Leaf: ?GivePrize@CTCTavernGame@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB590, 631 bytes (+ int3 padding), plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?GivePrize@CTCTavernGame@@QAEXXZ (public; engine/CTCTavernGame.h binds 0x005EB590; FrameUpdate's case 9 reads HasPrizeBeenGiven).
// Callees: CDefClassBase::GetSubDef<CTavernGameDef> 0x005ED4A0 (the inline GetDef; same-TU body), CPlayerManager::GetMainPlayer()
//   const 0x00449960 (on CMainGameComponent::PMainGameComponent 0x013B86A0's player manager word +0x1C), CPlayer::GetPControlledCreature
//   0x00487DC0, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (GetTC<CTCHeroReceiveItems> 203 / <CTCHeroOpinionDeedLog>
//   81 / <CTCOwnedEntity> 51), CTCBase::PeekPlayerManager 0x00686D50, CPlayerManager::GetMainPlayerNumber 0x00449700, CThing::GetPos
//   0x004C73D0, CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0, CThingObject::Create 0x00703210 (__fastcall),
//   CTCHeroReceiveItems::GiveItem 0x007CAB30, CTCTavernGame::GetHeroStats 0x005EB4B0, CTCHeroStats::AddRenown 0x0057D8BE, malloc
//   0x00BFEA0E (the creature list's header), CBaseIntelligentPointer::GetPItem 0x00A01B50 (CTCOwnedEntity::GetPOwner),
//   CTCPhysicsBase::GetBoundingInfo 0x006AF640, CTCBase::PeekThingSearchTools 0x00686D90, CThingSearchTools::GetAllThings
//   <CThingAICreature, list<CThingAICreature*>, CThingFilter_OverlappingCircle> 0x005ED7D0 (same-TU body), CTCHeroOpinionDeedLog::PostDeed
//   0x006C6990, ~list<CThingAICreature*> 0x004AB4F0 (out of line); the def's release (slot 1). Data: "" 0x0122D70E.
// Shape: the owned-entity component pointer is not tested (an absent component reads the uninitialised word, as retail); the deed log
//   is tested only through HasTC; the prize's orientation is the set Up (0,0,1), Forward (0,1,0).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTHINGOBJECT_CREATE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_DTOR_OUTOFLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefinitionManager.h"
// GetSubDef<CTavernGameDef> 0x005ED4A0 (landed, same body; as the landed FrameUpdate 0x005EB9E0): retail's TU holds it, so VC7.1 sees
// the def slot is not captured and keeps the def in edi across the later calls.
template <>
bool CDefClassBase::GetSubDef<CTavernGameDef>(CDefPointer<const CTavernGameDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CTavernGameDef::StatGetTypeName());
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#include "engine/CTCTavernGame.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingObject.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroReceiveItems.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_OverlappingCircle.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"

void CTCTavernGame::GivePrize()
{
    CDefPointer<const CTavernGameDef> def;
    GetDef(&def);
    CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
    CTCHeroReceiveItems* receive_items;
    if (hero->GetTC(&receive_items) && receive_items) {
        long player = PeekPlayerManager().GetMainPlayerNumber();
        C3DVector position = ParentThing->GetPos();
        CRightHandedSet orientation(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f));
        CThingObject* prize = CThingObject::Create(def->Prize, position, player, &orientation, 0, "");
        receive_items->GiveItem(*prize, false);
        {
            CTCHeroStats* hero_stats;
            GetHeroStats(&hero_stats);
            hero_stats->AddRenown(def->PrizeRenown);
        }
        CTCHeroOpinionDeedLog* deed_log;
        if (hero->GetTC(&deed_log)) {
            std::list<CThingAICreature*> creatures;
            CTCOwnedEntity* owned_entity;
            ParentThing->GetTC(&owned_entity);
            C3DVector centre;
            float radius;
            owned_entity->GetPOwner()->GetPhysicsTC()->GetBoundingInfo(centre, radius);
            PeekThingSearchTools().GetAllThings<CThingAICreature>(CThingFilter_OverlappingCircle(centre, radius), creatures);
            for (std::list<CThingAICreature*>::iterator it = creatures.begin(); it != creatures.end(); ++it)
                deed_log->PostDeed(OPINION_DEED_TYPE_TAVERN_GAME_WIN, hero->GetPos(), 0, *it, (EOpinionPostDeedType)1, false, false, 0,
                                   false, 0);
        }
    }
    HasPrizeBeenGiven = true;
}
