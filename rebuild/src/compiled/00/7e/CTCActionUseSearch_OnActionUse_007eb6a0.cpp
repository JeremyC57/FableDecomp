// CTCActionUseSearch::OnActionUse(CThing&) -- the user searches this container thing: run the base action-use, pick the
//   "SEARCH_HIGH" / "SEARCH_LOW" animation name from the half Z extent of the thing's physics mesh (2.0 when it has no mesh info;
//   high above 1.15), offer the hero the search tutorial (ETutorialCategory 0x21) and stop there when it is shown (not in a
//   cutscene), search the container (remembering whether it held a reward), send the owner a theft message, post a theft deed for
//   the user when this use counts as theft (and, if the container held a reward, apply the hero-stats theft morality change to the
//   hero), and finally record the user's current action as the one stealth permits while sneaking.
// Leaf: ?OnActionUse@CTCActionUseSearch@@UAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EB6A0, 748 bytes, thiscall `ret 4`, no frame pointer; vtable 0x0123F4DC slot 28.
// Identity: Ego_r.pdb ?OnActionUse@CTCActionUseSearch@@UAEXAAVCThing@@@Z (Ego 0x007E5430) + rtti-vtables slot 28 (base 0x007EAB90).
// Callees: CTCActionUseBase::OnActionUse 0x007EAB90, CTCPhysicsBase::GetPhysicsMeshInfo (vslot +0x174),
//   CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850, CPhysicsMeshObject::GetBoundingBoxZEdge 0x00A11BA0 / ~CPhysicsMeshObject 0x00A127D0,
//   CCharString ctor 0x0099E4B0 / operator=(const char*) 0x0099EFE0 / dtor 0x0099EAE0, CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CBaseIntelligentPointer::GetPItem 0x00A01B50 (inline GetPPlayerCharacter / GetPOwner),
//   CVectorMap::LowerBound 0x0040F020 (expanded GetTC), CTCHero::GiveTutorialForCategory 0x006C9FA0,
//   CTCContainerRewardHero::HasReward 0x0074E500, CTCSearchableContainer::Search 0x00812340, CTCBase::GetWorld 0x00686A70,
//   CThing::GetPos 0x004C73D0, CMessageEventManager::AddMessage 0x00687540, CTCActionUseBase::IsThisTheft 0x007EACB0,
//   CTCHeroOpinionDeedLog::PostDeed 0x006C6990, CTCBase::GetDefinitionManager 0x006869D0, CTCHeroStats::SetMorality 0x0057C2E6,
//   CTCStealth::SetActionPermittedToSneak 0x00747030.
// Shape notes: CPlayer.h must precede CThingPlayerCreature.h (else the inline GetPPlayerCharacter is not defined and becomes a call);
//   the deed's thing argument is &GetParentThing() (the ParentThing member read picks other registers); CTCSearchableContainer has
//   no shared header (declared here on its CTCContainerRewardHero base).
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCACTIONUSEBASE_ISTHISTHEFT
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CPHYSICSMESHOBJECT_ZEDGE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCONTAINERREWARDHERO_HASREWARD
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_HEROSTATSDEF
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER_PEEKHEROSTATSDEF
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTCHEROSTATS_MORALITY
#define FABLE_RETAIL_CWORLD
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CTCHero.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCStealth.h"
#include "engine/CTCContainerRewardHero.h"
#include "engine/CMessageEventManager.h"
#include "engine/CWorld.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CHeroStatsDef.h"
#include "engine/EMessageEventType.h"

class CTCSearchableContainer : public CTCContainerRewardHero {
public:
    void Search(CThing& user);                          // 0x00812340
};

class CTCActionUseSearch : public CTCActionUseBase {
public:
    virtual void OnActionUse(CThing& user);
};

void CTCActionUseSearch::OnActionUse(CThing& user)
{
    CTCActionUseBase::OnActionUse(user);
    float height;
    if (ParentThing->GetPhysicsTC()->GetPhysicsMeshInfo()) {
        C3DVector edge = ParentThing->GetPhysicsTC()->GetPhysicsMeshObject().GetBoundingBoxZEdge();
        height = edge.Z;
    } else
        height = 2.0f;
    CCharString animation;
    if (height > 1.15f)
        animation = "SEARCH_HIGH";
    else
        animation = "SEARCH_LOW";
    CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    if (hero && !hero->IsDead() && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) {
        CTCHero* heroTC;
        if (hero->GetTC(TCI_HERO, &heroTC) && heroTC->GiveTutorialForCategory((ETutorialCategory)0x21, false))
            return;
    }
    bool hadReward = false;
    CTCSearchableContainer* container;
    if (ParentThing->GetTC(TCI_CONTAINER, &container)) {
        hadReward = container->HasReward();
        container->Search(user);
    }
    CTCOwnedEntity* owned;
    if (ParentThing->GetTC(TCI_OWNED_ENTITY, &owned) && owned->GetPOwner())
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_THEFT, 0x32, user.GetPos(), &user, owned->GetPOwner(), 0.0f, 0.0f, 0);
    CTCHeroOpinionDeedLog* deedLog;
    if (IsThisTheft() && user.GetTC(TCI_HERO_OPINION_DEED_LOG, &deedLog)) {
        deedLog->PostDeed(OPINION_DEED_TYPE_CRIME_THEFT, user.GetPos(), &GetParentThing(), 0, (EOpinionPostDeedType)2, true, false, 0, false, 0);
        CTCHeroStats* stats;
        if (hadReward && hero && !hero->IsDead() && hero->GetTC(TCI_HERO_STATS, &stats))
            stats->SetMorality(GetDefinitionManager().PeekHeroStatsDef().MoralityChangeOnTheftFromHome + stats->Morality);
    }
    CTCStealth* stealth;
    if (user.GetTC(TCI_STEALTH, &stealth))
        stealth->SetActionPermittedToSneak(static_cast<CThingCreatureBase&>(user).PeekPCurrentAction());
}