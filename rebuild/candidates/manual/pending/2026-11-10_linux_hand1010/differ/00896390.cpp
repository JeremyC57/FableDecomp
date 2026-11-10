// CGameScriptInterface::OpenChest(const CScriptThing& chest, bool useHero) const -- open a scripted chest: with useHero the live hero plays the open-chest action (refused while a tutorial for it is pending), and opening a chest whose owner is a live village member of a trespassable place or shop (and not a house the player owns) posts a theft deed against the hero; without it the chest opens directly
// Grade: DIFFER(711v711) -- block order only: retail lays the direct Open() arm (0x00896632) between the theft-deed true tail and the shared false tail (0x00896648); ours puts the false tail first and Open() last. `if (!started) goto fail;` (d) fixes Open but lays the false tail at the SetCurrentAction test (703v711). Shared headers, FABLE_HDR_OVERLAY=$A/hdr; the inline IS_THING_ALIVE<CThingPlayerCreature*> keeps the open-chest temporary's implicit dtor inline (a plain `!hero || hero->IsDead()` pushes ~CCreatureAction_OpenChest out of line).
// Leaf: ?OpenChest@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00896390, 711 bytes, thiscall `ret 8`.
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_OPENCHEST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CTCHero.h"
#include "engine/CTCChest.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CCreatureAction_OpenChest.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"

template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing)   // PDB template; <CThing*> out of line at 0x0040DCE0
{
    if (thing) {
        if (!thing->IsDead())
            return true;
    }
    return false;
}

bool CGameScriptInterface::OpenChest(const CScriptThing& chest, bool useHero) const
{
    {
    if (!chest.IsAlive())
        goto fail;
    CThing* thing = chest.GetPThing();
    CTCChest* chestTC;
    if (!thing->GetTC(TCI_CONTAINER, &chestTC))
        goto fail;
    if (useHero) {
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        goto fail;
    if (!CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) {
        CTCHero* heroTC;
        if (hero->GetTC(TCI_HERO, &heroTC) && heroTC->GiveTutorialForCategory((ETutorialCategory)0x21, false))
            goto fail;
    }
    hero->FinishCurrentAction();
    bool started = hero->SetCurrentAction(CCreatureAction_OpenChest(*hero, *thing, chestTC->GetOpenAnimNameForCreature()));
    if (started) {
    const CTCOwnedEntity* owned;
    if (thing->PeekTC(TCI_OWNED_ENTITY, &owned) && IS_THING_ALIVE(owned->GetPOwner())) {
        CTCVillageMember* member;
        if (owned->GetPOwner()->GetTC(TCI_VILLAGE_MEMBER, &member) && member->GetPVillage()) {
            if (owned->GetPOwner()->HasTC(TCI_TRESPASSABLE) || owned->GetPOwner()->HasTC(TCI_SHOP)) {
                CTCBuyableHouse* house;
                if (!owned->GetPOwner()->GetTC(TCI_BUYABLE_HOUSE, &house) || !house->GetOwnedByPlayer()) {
                    CTCHeroOpinionDeedLog* deedLog;
                    if (hero->GetTC(TCI_HERO_OPINION_DEED_LOG, &deedLog))
                        deedLog->PostDeed(OPINION_DEED_TYPE_CRIME_THEFT, hero->GetPos(), thing, 0, (EOpinionPostDeedType)2, true, false, 0, false, 0);
                }
            }
        }
    }
    return true;
    }
    goto fail;
    }
    chestTC->Open();
    return true;
    }
fail:
    return false;
}
