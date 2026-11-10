// CGameScriptInterface::TeleportAllFollowersToHeroPosition() const -- move every follower of the hero (plus its live summoned minions) that is more than 2 units away to a wander position near the hero
// Grade: DIFFER(654v738) -- inline budget + registers: retail expands CTCSummonSpell::GetSummonedCreature and the minion-loop vector<CThing*>::push_back (calling _M_insert_overflow 0x00436AC0) while ours calls them; hero lives in ebp with a spill at [esp+0xc] (ours ebx, zero in edi). /d2inlS has no effect; /d2inlT85 expands everything (702 bytes) but over-inlines
// Leaf: ?TeleportAllFollowersToHeroPosition@CGameScriptInterface@@UBEXXZ
// Retail: 0x008A7A70, 738 bytes, __thiscall (virtual, const)
// Overlay: graded with FABLE_HDR_OVERLAY=$A/hdr (see hdr/README.md)
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_MINION
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCFOLLOWED
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CTCSummonSpell.h"
#include "engine/CTCGhostSword.h"
#include "engine/CTCFollowed.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/C2DVector.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::TeleportAllFollowersToHeroPosition() const
{
    CThing* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!hero || hero->IsDead())
        return;
    CTCFollowed* followed;
    if (!hero->GetTC(TCI_FOLLOWED, &followed))
        return;
    std::vector<CThing*> followers;
    followed->GetFollowers(followers);
    const CTCSummonSpell* summon;
    if (hero->PeekTC(TCI_SUMMON_SPELL, &summon)) {
        CIntelligentPointer<CThingAICreature> creature = summon->GetSummonedCreature();
        if (creature.Get() && !creature->IsDead())
            followers.push_back(creature.Get());
    }
    const CTCGhostSword* sword;
    if (hero->PeekTC(TCI_GHOST_SWORD, &sword)) {
        std::vector<CIntelligentPointer<CThingAICreature> > minions = sword->GetSummonedCreatures();
        for (long i = 0; i < (long)minions.size(); ++i) {
            if (minions[i].Get() && !minions[i]->IsDead())
                followers.push_back(minions[i].Get());
        }
    }
    for (unsigned long i = 0; i < followers.size(); ++i) {
        CThing* follower = followers[i];
        C2DVector heroPosition(hero->GetPos());
        const CTCCreatureNavigation* navigation;
        if ((heroPosition - C2DVector(follower->GetPos())).GetSquaredMagnitude() > 4.0f &&
            follower->PeekTC(TCI_NAVIGATION, &navigation)) {
            C3DVector position;
            if (navigation->GetNearbyWanderPosition(hero->GetPos(), 0.8f, position, false) && follower->GetPhysicsTC())
                follower->GetPhysicsTC()->ResetPosition(position);
        }
    }
}
