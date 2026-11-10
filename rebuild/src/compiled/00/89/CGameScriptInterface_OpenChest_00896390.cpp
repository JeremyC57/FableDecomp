// CGameScriptInterface::OpenChest(const CScriptThing& chest, bool useHero) const -- open a scripted chest: with useHero the live hero plays the open-chest action (refused while a tutorial for it is pending), and opening a chest whose owner is a live village member of a trespassable place or shop (and not a house the player owns) posts a theft deed against the hero; without it the chest opens directly
// Leaf: ?OpenChest@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00896390, 711 bytes (+ int3 padding), thiscall `ret 8`. Identity: retail
//   CGameScriptInterface vtable 0x01260F0C (engine/CGameScriptInterface.h); Ego_r.pdb / FableWin.pdb
//   ?OpenChest@CGameScriptInterface@@UBE_NABVCScriptThing@@_N@Z.
// Callees (PDB spellings): CScriptThing slots 75 IsAlive / 11 GetPThing, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//   0x0040F020 (GetTC) / const 0x004365B0 (PeekTC), CPlayerManager::GetMainPlayer() 0x00449970, CPlayer::GetPControlledCreature()
//   const 0x00487DC0, CPlayerGui singleton 0x013B8790 (GetSubtitleRenderer().IsInCutscene(), +0xF6), CTCHero::GiveTutorialForCategory
//   0x006C9FA0, CThingCreatureBase::FinishCurrentAction 0x00662650 / SetCurrentAction 0x006644F0, CTCChest::GetOpenAnimNameForCreature()
//   const 0x00750240 / Open() 0x00750540, CCreatureAction_OpenChest ctor 0x007EE7E0 (implicit dtor expanded: ~CIntelligentPointer
//   0x00A01C10, ~CCreatureActionBase 0x00693EF0), CCharString dtor 0x0099EAE0, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   IS_THING_ALIVE<CThing*> 0x0040DCE0 (out of line), CTCVillageMember::GetPVillage 0x007EE540, CThing::GetPos() const 0x004C73D0,
//   CTCHeroOpinionDeedLog::PostDeed 0x006C6990.
// Model: shared headers (chunk 1010 opt-ins, committed). The parked 711v711 drafts (gotos to one false tail) laid the direct
//   CTCChest::Open() arm after the shared false tail; the natural nested form -- positive tests, `if (useHero) {...} else
//   { chestTC->Open(); return true; }` and one final `return false;` -- gives retail's order: theft-deed true tail, Open() arm
//   (0x00896632), false tail (0x00896648).
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
    if (chest.IsAlive()) {
        CThing* thing = chest.GetPThing();
        CTCChest* chestTC;
        if (thing->GetTC(TCI_CONTAINER, &chestTC)) {
            if (useHero) {
        CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
        if (!IS_THING_ALIVE(hero))
            return false;
        if (!CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) {
            CTCHero* heroTC;
            if (hero->GetTC(TCI_HERO, &heroTC) && heroTC->GiveTutorialForCategory((ETutorialCategory)0x21, false))
                return false;
        }
        hero->FinishCurrentAction();
                if (hero->SetCurrentAction(CCreatureAction_OpenChest(*hero, *thing, chestTC->GetOpenAnimNameForCreature()))) {
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
            }
            else {
                chestTC->Open();
                return true;
            }
        }
    }
    return false;
}