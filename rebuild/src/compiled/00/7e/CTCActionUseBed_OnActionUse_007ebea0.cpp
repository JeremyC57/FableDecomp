// 007ebea0 CTCActionUseBed::OnActionUse
// Leaf: ?OnActionUse@CTCActionUseBed@@UAEXAAVCThing@@@Z
// Flags: /GS
#define FABLE_RETAIL_BED_USE_PASS28
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_BUILDING_GETTERS_PASS26
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_PEEKVILLAGEPEOPLE
#define FABLE_RETAIL_CTCVILLAGE_LIMBO
#define FABLE_RETAIL_CTCVILLAGE_YELLFORHELP
#define FABLE_RETAIL_CTCVILLAGEMEMBER
#define FABLE_RETAIL_CTCOWNEDENTITY
#define FABLE_RETAIL_CTCBED
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR4
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTCBASE_ADDTOGLOBALTCS
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/CPlayer.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTavernDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CTCActionUseBed.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCBed.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CPlayerManager.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerGui.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CGameEvent.h"
void CTCActionUseBed::OnActionUse(CThing& thing_performing_action)
{
    {
        const CIntelligentPointer<CThing> user(&thing_performing_action);
        PThingUser = user;
    }
    bool allowed_to_sleep = true;
    CTCOwnedEntity* tc_owned_entity = 0;
    CTCVillageMember* tc_village_member = 0;
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    if ((GetParentThing().GetTC(TCI_OWNED_ENTITY, &tc_owned_entity) && tc_owned_entity->GetPOwner()
        && tc_owned_entity->GetPOwner()->GetTC(TCI_VILLAGE_MEMBER, &tc_village_member)
        && tc_village_member->GetPVillage() && (tc_village_member->GetPVillage()->GetVillageLimbo()
        || tc_village_member->GetPVillage()->IsHeroEnemyBecauseOfCrime())) || !phero || phero->IsInMelee())
        allowed_to_sleep = false;
    if (!allowed_to_sleep || WaitingForFadeOut || WaitingForFadeIn)
        return;
    CTCActionUseBase::OnActionUse(thing_performing_action);
    PlayerNumberOfThingUsingBed = thing_performing_action.GetPlayerNumber();
    CCharString main("");
    CThing& bed = GetParentThing();
    {
        const CIntelligentPointer<CThing> no_building(0);
        PBuilding = no_building;
    }
    bool building_has_occupants = false;
    if (!PeekWorld().IsHeroSleepingEnabled()) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    {
        CTCOwnedEntity* tc_bed_owned_entity;
        if (bed.GetTC(TCI_OWNED_ENTITY, &tc_bed_owned_entity)) {
            {
                const CIntelligentPointer<CThing> building(tc_bed_owned_entity->GetPOwner());
                PBuilding = building;
            }
            CTCVillageMember* tc_bed_village_member;
            if (PBuilding.Get()->GetTC(TCI_VILLAGE_MEMBER, &tc_bed_village_member)) {
                CTCVillage* tc_village = tc_bed_village_member->GetPVillage();
                if (tc_village) {
                    for (std::list<CIntelligentPointer<CThingAICreature> >::const_iterator person = tc_village->PeekVillagePeople().begin();
                         person != tc_village->PeekVillagePeople().end(); ++person) {
                        if (PBuilding.Get() == (*person)->GetHomeBuilding() || PBuilding.Get() == (*person)->GetWorkBuilding()) {
                            building_has_occupants = true;
                            break;
                        }
                    }
                }
                else
                    building_has_occupants = true;
            }
            else
                building_has_occupants = true;
        }
        else
            building_has_occupants = true;
    }
    CTCBuyableHouse* tc_buyable_house = 0;
    CTCBed* tc_bed = 0;
    bed.GetTC(TCI_BED, &tc_bed);
    if (thing_performing_action.HasTC(TCI_BERSERK)) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    }
    if (tc_bed && !tc_bed->IsEmpty()) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    } else if (OwnedByHero) {
        main = "TEXT_GUI_USEBED_CHECK";
        Cost = 0;
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else if (!UseableByHero) {
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), "TEXT_GUI_USEBED_FAIL", "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_CONTINUE", "", 0x122, 0.0f, true, true);
        return;
    } else if (!PBuilding.Get()) {
        main = "TEXT_GUI_USEBED_ILLEGAL";
        Cost = -1;
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else if (!building_has_occupants) {
        main = "TEXT_GUI_USEBED_CHECK";
        Cost = 0;
        CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                               "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
    } else {
        if (PBuilding.Get()->HasTC(TCI_BUYABLE_HOUSE) && PBuilding.Get()->GetTC(TCI_BUYABLE_HOUSE, &tc_buyable_house)
            && tc_buyable_house->GetOwnedByPlayer() && !tc_buyable_house->GetRented()) {
            main = "TEXT_GUI_USEBED_CHECK";
            Cost = 0;
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                                   "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        } else if (PBuilding.Get()->HasTC(TCI_TAVERN)) {
            main = "TEXT_GUI_USEBED_COST";
            CDefPointer<const CTavernDef> pdef;
            PBuilding.Get()->PeekBaseDef().GetSubDef(&pdef);
            Cost = pdef->BedCost;
            CWideString display_text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol(main);
            NGameText::ConstructString(display_text, Cost);
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), display_text, L"",
                                                   L"TEXT_GUI_TAVERNGAMES_GENERIC_YES", L"TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        } else {
            main = "TEXT_GUI_USEBED_ILLEGAL";
            Cost = -1;
            CPlayerGui::Get().GetSubtitleRenderer().SetText(CMainGameComponent::Get()->PeekCutSceneMainFont(), main, "",
                                                   "TEXT_GUI_TAVERNGAMES_GENERIC_YES", "TEXT_GUI_TAVERNGAMES_GENERIC_NO", 4, 0.0f, true, true);
        }

    }
    AddToGlobalTCs();
    WaitingForConfirmation = true;
    CGameEvent pause_event(9);
    pause_event.SetPlayer(PlayerNumberOfThingUsingBed);
    pause_event.AddUBYTE(1);
    CMainGameComponent::Get()->AddGameEvent(pause_event);
}