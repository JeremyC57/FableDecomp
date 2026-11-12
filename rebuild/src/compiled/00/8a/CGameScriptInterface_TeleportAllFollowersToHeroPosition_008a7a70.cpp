// CGameScriptInterface::TeleportAllFollowersToHeroPosition() const -- move every follower of the hero (plus the live summoned
//   creature of the summon spell and the live ghost-sword minions) that is more than 2 units away (2D) to a wander position near the hero
// Leaf: ?TeleportAllFollowersToHeroPosition@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe 0x008A7A70, 738 bytes, thiscall (virtual slot 239 of CGameScriptInterface 0x01260F0C, const, no args)
// Callees: CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0; CVectorMap::LowerBound 0x0040F020
//   (GetTC) / 0x004365B0 (const, PeekTC); CTCFollowed::GetFollowers 0x006D5850; CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10; vector<CThing*>::push_back 0x00436C80 (COMDAT) and its
//   _M_insert_overflow 0x00436AC0; vector<CIntelligentPointer<CThingAICreature> > copy ctor 0x008AC540 / dtor 0x00742210;
//   CThing::GetPos 0x004C73D0; CTCCreatureNavigation::GetNearbyWanderPosition 0x006CB6E0; CTCPhysicsBase::ResetPosition (vslot 31,
//   +0x7C); free 0x00BFEA14. Constants: 4.0f @0x0123088C, 0.8f immediate.
// Shape: VC7.1 expands an inline callee at a site iff c1*(1/f+6)/7 < 74 (f = static site frequency; GOTCHAS/0x0056AD40). The minion
//   loop is a `while` (body 9x), so its push_back expands (`finish!=eos`, placement-new null test, else _M_insert_overflow), while the
//   summon-spell push_back (f < 1) stays a call to the 0x00436C80 COMDAT. CTCSummonSpell::GetSummonedCreature is expanded in place
//   (copy ctor: base ctor, vptr 0x0123E7E0, GetPItem/SetPItem) through the per-TU opt-in
//   FABLE_RETAIL_CTCSUMMONSPELL_GETSUMMONEDCREATURE_FORCEINLINE (header overlay). The 2D distance is written with float locals
//   (dx, dy; squared = dy*dy, += dx*dx): with a C2DVector difference temporary the x spill gets its own 8-byte frame slot
//   (sub esp,38h); as a scalar it shares the TC-key slot, giving retail's 0x30 frame and the hero spill at [esp+0Ch].
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
#define FABLE_RETAIL_CTCSUMMONSPELL_GETSUMMONEDCREATURE_FORCEINLINE
#define FABLE_RETAIL_CTCGHOSTSWORD
#define FABLE_RETAIL_CTCFOLLOWED
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_STLP_VECTOR_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_LIST
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
    if (!hero->GetTC(&followed))
        return;
    std::vector<CThing*> followers;
    followed->GetFollowers(followers);
    const CTCSummonSpell* summon;
    if (hero->PeekTC(&summon)) {
        CIntelligentPointer<CThingAICreature> creature = summon->GetSummonedCreature();
        if (creature.Get() && !creature->IsDead())
            followers.push_back(creature.Get());
    }
    const CTCGhostSword* sword;
    if (hero->PeekTC(&sword)) {
        std::vector<CIntelligentPointer<CThingAICreature> > minions = sword->GetSummonedCreatures();
        long i = 0;
        while (i < (long)minions.size()) {
            if (minions[i].Get() && !minions[i]->IsDead())
                followers.push_back(minions[i].Get());
            ++i;
        }
    }
    for (unsigned long i = 0; i < followers.size(); ++i) {
        CThing* follower = followers[i];
        C2DVector heroPosition(hero->GetPos());
        C2DVector followerPosition(follower->GetPos());
        float dx = heroPosition.X - followerPosition.X;
        float dy = heroPosition.Y - followerPosition.Y;
        float squared = dy * dy;
        squared += dx * dx;
        const CTCCreatureNavigation* navigation;
        if (squared > 4.0f &&
            follower->PeekTC(&navigation)) {
            C3DVector position;
            if (navigation->GetNearbyWanderPosition(hero->GetPos(), 0.8f, position, false) && follower->GetPhysicsTC())
                follower->GetPhysicsTC()->ResetPosition(position);
        }
    }
}