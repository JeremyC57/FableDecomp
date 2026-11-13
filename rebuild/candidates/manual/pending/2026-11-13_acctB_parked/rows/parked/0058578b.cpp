// Grade: DIFFER(7948v8081) -- optimize(s) alone gives the biased frame; residue: top_level in esi (retail edi), TC key consts in edi, loop1 regs, item-loop releases
// CTCInventory::OpenInventory() -- open the console inventory's top-level menu: clear the pad rumble, fade the music / sound /
// ambient groups out, stop 3D scene rendering, select this hero-information screen, open the base inventory and size its anim
// box, register the inventory with the UI manager, then build the top-level menu: gold amount, mini map with the region markers,
// the active quest list (or the "check the guild" text), the five back actions, the map / quests / logbook / skills entries, the
// item counts of the weapons / items / clothing / style-card entries and the stats / experience / personality entries; finally
// keep the menu and initialise every component in state 5.
// Leaf: ?OpenInventory@CTCInventory@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058578B, 8081 bytes, thiscall `leave ; ret`; frame-pointer size TU
//   (optimize("y",off) + ("s",on)), no EH states.
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING_FORCEINLINE_EQ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_PDEF
#define FABLE_RETAIL_CTCINVENTORYBASE_ANIMBOX
#define FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPENINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_HISCREEN
#define FABLE_RETAIL_CTCINVENTORYMAP_MINIMAPMARKERS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ISCHILD
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_CACTIONPARAMSTATE
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_JOYSTICKDEVICE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKS
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_CORNERS
#define FABLE_RETAIL_C2DBOXF_CORNERS_FORCEINLINE
#define FABLE_RETAIL_CJOYSTICK
#define FABLE_RETAIL_CJOYSTICK_VIRTUALS
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_QUESTNAME
#define FABLE_RETAIL_CTCQUESTCARD_QUESTTEXT
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCINVENTORYSTATS
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_STLP_MAP
#include "engine/C2DVector.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTCQuestCard.h"
#include "engine/CQuestCardDef.h"
#include "engine/CQuestManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CASoundManager.h"
#include "engine/CInputManager.h"
#include "engine/CJoystick.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CWorldMap.h"
#include "engine/CRegion.h"
#include "engine/CInventoryDef.h"
#include "engine/C2DBoxF.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CHeroSuitDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CTattooDef.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamState.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/CCountedPointer.h"

using namespace NUISystem;

extern CInputManager* GPInputManager;                                   // 0x013B8388 ?GPInputManager@@3PAVCInputManager@@A
CWideString __fastcall GFIntToWideString(long value);                   // 0x00405F00
class CTCInventoryWeapons;
class CTCInventoryClothing;
class CTCHeroAbilities;

// Same-TU body of the def lookup retail's TU holds (out of line in retail: GetDef<CUIMiscThingsDef> 0x0042B312,
// GetDef<CThingBaseDef> 0x0042AF3C), as the landed CTCInventoryTrade::ConstructSpecialShopList 0x005A29F4.
#pragma auto_inline(off)
template <class T>
bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    if (index > 0) {
        CDefPointer<const CDefClassBase> def = GetPDefFromGlobalIndex(index);
        if (!def.IsNull()) {
            out->SetDynamicCast(def);
            return true;
        }
    }
    return false;
}
#pragma auto_inline(on)

void CTCInventory::OpenInventory()
{
    CManager::OnOpenMenus();
    GPInputManager->GetJoystick(GetPlayerManager().GetMainPlayerJoystickDeviceNumber()).ClearAllRumbles();
    if (PSoundManager) {
        PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
        PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
    }
    CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
    CPlayerGui::Get().GetPHeroInfoScreensManager()->SelectScreenOfType(GetScreenType());
    NInventory::CTCInventoryBase::OpenInventory();
    SetInventoryAnimRenderBox(C2DBoxF(GetPDef()->MeshAreaTLPos, GetPDef()->MeshAreaBRPos));

    CManager* manager = CManager::GetInstance();
    CManager::GetInstance()->ClearActionParams();
    CManager::GetInstance()->SetInventory(this);
    CManager::GetInstance()->SetPlayerThing(ParentThing);
    CComponent* top_level = CManager::GetInstance()->CreateComponent("UI_TOP_LEVEL_MENU", false);

    NUISystem::CText* gold = static_cast<NUISystem::CText*>(top_level->FindChild("UI_GOLD_AMOUNT_TOP_LEVEL"));
    if (gold) {
        const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const CTCHeroStats* hero_stats;
        hero->PeekTC(TCI_HERO_STATS, &hero_stats);
        gold->SetValue(GFIntToWideString(hero_stats->GetMoney()), true, true);
    }

    CTCInventoryWeapons* weapons_tc = 0;
    CTCInventory* inventory_tc = 0;
    CTCInventoryMap* map_tc = 0;
    CTCHeroAbilities* abilities_tc = 0;
    CTCInventoryStats* stats_tc = 0;
    NInventory::CTCInventoryBase* clothing_tc = 0;
    ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &weapons_tc);
    ParentThing->GetTC(TCI_INVENTORY, &inventory_tc);
    ParentThing->GetTC(TCI_INVENTORY_MAP, &map_tc);
    ParentThing->GetTC(TCI_HERO_ABILITIES, &abilities_tc);
    ParentThing->GetTC(TCI_INVENTORY_STATS, &stats_tc);
    ParentThing->GetTC(TCI_INVENTORY_CLOTHING, &clothing_tc);

    CCountedPointer<CComponent> mini_map(CManager::GetInstance()->CreateComponent("UI_MAIN_MENU_MINI_MAP_TEMPLATE", false));
    CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
    if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
        static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(
            GetMiniMapGraphic(GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(ParentThing->GetCurrentMapNumber()))));
    if (map_tc)
        map_tc->CreateMiniMapMarkers(mini_map.Get(),
            GetWorldMap().GetRegion(GetWorldMap().GetRegionNumberMapIsIn(ParentThing->GetCurrentMapNumber())));
    mini_map->AddChild(mini_map_sprite);
    top_level->FindChild("UI_MINI_MAP_VIEWPORT")->AddChild(mini_map);

    CCountedPointer<CComponent> description(CManager::GetInstance()->CreateComponent("UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_FOR_LIST", false));
    const std::list<CIntelligentPointer<CThing> >& quests = CQuestManager::Get().GetActiveQuestCards();
    CWideString quest_text;
    CDefPointer<const CUIMiscThingsDef> misc_def;
    long misc_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().GetDef(misc_index, &misc_def);

    for (std::list<CIntelligentPointer<CThing> >::const_iterator it = quests.begin(); it != quests.end(); ++it) {
        if (IS_THING_ALIVE(*it)) {
            const CTCQuestCard* quest_card = 0;
            (*it)->PeekTC(&quest_card);
            if (quest_card->IsCore() || (quest_card->IsOptional() && !quest_card->GetPDef()->IsVignette))
                goto quest_list;
        }
    }
    {
        CCountedPointer<CComponent> text(manager->CreateComponent("UI_TEXT_INSIDE_CONTAINER_TEMPLATE", false));
        description->AddChild(text);
        static_cast<NUISystem::CText*>(text.Get())->SetValue(CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_CHECKGUILD"), true, true);
    }
    goto quest_list_done;
quest_list:
        for (std::list<CIntelligentPointer<CThing> >::const_reverse_iterator it = quests.rbegin(); it != quests.rend(); ++it) {
            if (IS_THING_ALIVE(*it)) {
                const CTCQuestCard* quest_card = 0;
                (*it)->PeekTC(&quest_card);
                bool show = quest_card->IsCore() || (quest_card->IsOptional() && !quest_card->GetPDef()->IsVignette);
                if (show) {
                    CCountedPointer<CComponent> entry;
                    if (quest_card->IsCore())
                        entry.Reset(manager->CreateComponent("UI_CORE_ICON_AND_NAME", false));
                    else
                        entry.Reset(manager->CreateComponent("UI_OPTIONAL_ICON_AND_NAME", false));
                    NUISystem::CText* name = static_cast<NUISystem::CText*>(entry->FindChild("UI_QUEST_NAME"));
                    if (name) {
                        bool no_objective = quest_card->GetQuestObjective() == L"";
                        if (no_objective)
                            name->SetValue(quest_card->GetQuestName() + misc_def->ColonSeparator + misc_def->SpaceSeparator + quest_card->GetQuestSummary(), true, true);
                        else
                            name->SetValue(quest_card->GetQuestName() + misc_def->ColonSeparator + misc_def->SpaceSeparator + quest_card->GetQuestObjective(), true, true);
                        description->AddChild(entry);
                    }
                }
            }
        }
quest_list_done:
    CCountedPointer<CComponent> scroll_viewport(manager->CreateComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA", false));
    CViewport* viewport = static_cast<CViewport*>(scroll_viewport.Get());
    viewport->AddFakeChild(description.Get());
    static_cast<CList*>(top_level)->AddNonScrollingChild(description);
    static_cast<CList*>(top_level)->AddNonScrollingChild(scroll_viewport);

    CCountedPointer<CAction> back_menu_screen(new CAction);
    back_menu_screen->ActionType = UI_ACTION_TYPE_CHANGESTATE;
    back_menu_screen->UIActionParams.push_back(top_level->FindChild("UI_TOP_LEVEL_MENU_SCREEN"));
    back_menu_screen->UIActionParams.push_back(new CActionParamState(0));
    CCountedPointer<CAction> back_table_text(new CAction);
    back_table_text->ActionType = UI_ACTION_TYPE_CHANGESTATE;
    back_table_text->UIActionParams.push_back(top_level->FindChild("UI_TABLE_TEXT_LEFT"));
    back_table_text->UIActionParams.push_back(new CActionParamState(0));
    CCountedPointer<CAction> back_thumb_stick(new CAction);
    back_thumb_stick->ActionType = UI_ACTION_TYPE_CHANGESTATE;
    back_thumb_stick->UIActionParams.push_back(top_level->FindChild("UI_THUMB_STICK_UP_DOWN"));
    back_thumb_stick->UIActionParams.push_back(new CActionParamState(0));
    CCountedPointer<CAction> back_bottom_backdrop(new CAction);
    back_bottom_backdrop->ActionType = UI_ACTION_TYPE_CHANGESTATE;
    back_bottom_backdrop->UIActionParams.push_back(top_level->FindChild("UI_BOTTOM_BACKDROP"));
    back_bottom_backdrop->UIActionParams.push_back(new CActionParamState(0));
    CCountedPointer<CAction> back_screen_backdrop(new CAction);
    back_screen_backdrop->ActionType = UI_ACTION_TYPE_CHANGE_ALPHA;
    back_screen_backdrop->UIActionParams.push_back(new CActionParamSLONG(0));
    back_screen_backdrop->UIActionParams.push_back(top_level->FindChild("UI_SCREEN_BACKDROP"));

    if (map_tc) {
        CComponent* map_entry = top_level->FindChild("UI_MENU_ENTRY_MAP");
        if (CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature()->IsChild()) {
            if (map_entry) {
                map_entry->MakeUnavailable();
                CComponent* arrow = map_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
                if (arrow) {
                    CDeletion deletion;
                    deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                    arrow->SetDeletion(deletion);
                }
            }
        } else {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_CONSTRUCT_MAP_LIST;
            action->UIActionParams.push_back(map_entry);
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
            map_entry->AddPreAction(action);
        }
        CComponent* quests_entry = top_level->FindChild("UI_MENU_ENTRY_QUESTS");
        {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_CONSTRUCT_QUESTS_LIST;
            action->UIActionParams.push_back(quests_entry);
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
            quests_entry->AddPreAction(action);
        }
        CComponent* logbook_entry = top_level->FindChild("UI_MENU_ENTRY_LOGBOOK");
        if (logbook_entry) {
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = UI_ACTION_TYPE_CONSTRUCT_LOGBOOK_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*logbook_entry));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
            logbook_entry->AddPreAction(action);
        }
    }
    CComponent* skills_entry = top_level->FindChild("UI_MENU_ENTRY_SKILLS");
    if (abilities_tc) {
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_CONSTRUCT_SKILLS_LIST;
        action->UIActionParams.push_back(skills_entry);
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        skills_entry->AddPreAction(action);
    } else if (skills_entry) {
        skills_entry->MakeUnavailable();
        CComponent* arrow = skills_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
    }

    // count the hero's items per menu category
    const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    const CTCInventoryWeapons* hero_weapons = 0;
    std::vector<std::pair<long, long> > items;
    GetAllItems(items);
    for (std::vector<std::pair<long, long> >::iterator item = items.begin(); item != items.end(); ) {
        if (CTCInventoryItem::IsBook(item->first))
            item = items.erase(item);
        else
            ++item;
    }
    hero->PeekTC(TCI_INVENTORY_WEAPONS, &hero_weapons);
    ((const NInventory::CTCInventoryBase*)hero_weapons)->GetAllItems(items);
    if (clothing_tc)
        clothing_tc->GetAllItems(items);
    long weapons_count = 0;
    long items_count = 0;
    long clothing_count = 0;
    long facial_hair_count = 0;
    long hair_count = 0;
    long tattoo_count = 0;
    for (unsigned long i = 0; i < items.size(); ++i) {
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(items[i].first, &def);
        CDefPointer<const CAugmentationDef> augmentation;
        if (!def->GetSubDef(&augmentation)) {
        CDefPointer<const CHeroSuitDef> suit;
        CDefPointer<const CAppearanceModifierDef> appearance;
        if (CTCInventoryItem::IsWeapon(items[i].first)) {
            weapons_count += items[i].second;
        } else if (def->GetSubDef(&suit)) {
            for (std::map<EClothingSuitPart, long>::const_iterator part = suit->SuitParts.begin(); part != suit->SuitParts.end(); ++part) {
                if (clothing_tc->GetNumberOfItemsOfTypeInInventory(part->second) > 0) {
                    clothing_count += items[i].second;
                    break;
                }
            }
        } else if (def->GetSubDef(&appearance)) {
            if (appearance->SuitPart > 0 && appearance->SuitPart <= 5)
                clothing_count += items[i].second;
        } else if (!CTCInventoryItem::IsExpression(items[i].first)) {
            CDefPointer<const CHairCardDef> hair_card = CTCInventoryItem::GetHairCardDef(items[i].first);
            CDefPointer<const CTattooDef> tattoo_card = CTCInventoryItem::GetTattooCardDef(items[i].first);
            if (hair_card.IsNull()) {
                if (tattoo_card.IsNull())
                    items_count += items[i].second;
                else
                    tattoo_count += items[i].second;
            } else {
                CDefPointer<const CDefClassBase> hair_def = GetDefinitionManager().GetPDefFromGlobalIndex(hair_card->HairObject);
                if (!hair_def.IsNull()) {
                    CDefPointer<const CAppearanceModifierDef> hair_appearance;
                    hair_def->GetSubDef(&hair_appearance);
                    if (!hair_appearance.IsNull()) {
                        long flags = hair_appearance->CoversBodyAreaFlags;
                        if ((flags & 0x80) || (flags & 0x100))
                            hair_count += items[i].second;
                        else if (flags & 0x10)
                            facial_hair_count += items[i].second;
                    }
                }
            }
        }
        }
    }

    // weapons
    CComponent* weapons_entry = top_level->FindChild("UI_MENU_ENTRY_WEAPONS");
    viewport->AddFakeChild(weapons_entry->FindChild("UI_WEAPON_DESC"));
    if (weapons_tc && weapons_count > 0) {
        NUISystem::CText* text = static_cast<NUISystem::CText*>(weapons_entry->FindChild("UI_WEAPONS_BUTTON")->FindChild("UI_WEAPONS_TEXT"));
        CWideString label(text->GetValue());
        label += CTCInventoryStats::GetXString();
        label += GFIntToWideString(weapons_count);
        text->SetValue(label, true, true);
        NUISystem::CText* sub_title = static_cast<NUISystem::CText*>(weapons_entry->FindChild("UI_WEAPON_SUB_TITLE"));
        if (sub_title)
            sub_title->SetValue(label, true, true);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_CONSTRUCT_WEAPONS_LIST;
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*weapons_entry));
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        weapons_entry->AddPreAction(action);
    } else {
        weapons_entry->MakeUnavailable();
        CComponent* arrow = weapons_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
    }
    // items
    CComponent* items_entry = top_level->FindChild("UI_MENU_ENTRY_ITEMS");
    viewport->AddFakeChild(items_entry->FindChild("UI_ITEMS_DESC"));
    if (inventory_tc && items_count > 0) {
        NUISystem::CText* text = static_cast<NUISystem::CText*>(items_entry->FindChild("UI_ITEMS_BUTTON")->FindChild("UI_ITEMS_TEXT"));
        CWideString label(text->GetValue());
        label += CTCInventoryStats::GetXString();
        label += GFIntToWideString(items_count);
        text->SetValue(label, true, true);
        NUISystem::CText* sub_title = static_cast<NUISystem::CText*>(items_entry->FindChild("UI_ITEMS_SUB_TITLE"));
        if (sub_title)
            sub_title->SetValue(label, true, true);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_CONSTRUCT_ITEMS_LIST;
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*items_entry));
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        items_entry->AddPreAction(action);
    } else {
        CComponent* arrow = items_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
        items_entry->MakeUnavailable();
    }
    // clothing
    CComponent* clothing_entry = top_level->FindChild("UI_MENU_ENTRY_CLOTHING");
    viewport->AddFakeChild(clothing_entry->FindChild("UI_CLOTHING_DESC"));
    if (clothing_tc && clothing_count > 0) {
        NUISystem::CText* text = static_cast<NUISystem::CText*>(clothing_entry->FindChild("UI_CLOTHING_BUTTON")->FindChild("UI_CLOTHING_TEXT"));
        CWideString label(text->GetValue());
        label += CTCInventoryStats::GetXString();
        label += GFIntToWideString(clothing_count);
        text->SetValue(label, true, true);
        NUISystem::CText* sub_title = static_cast<NUISystem::CText*>(clothing_entry->FindChild("UI_CLOTHING_SUB_TITLE"));
        if (sub_title)
            sub_title->SetValue(label, true, true);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_CONSTRUCT_CLOTHING_LIST;
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*clothing_entry));
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        clothing_entry->AddPreAction(action);
    } else {
        clothing_entry->MakeUnavailable();
        CComponent* arrow = clothing_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            arrow->SetDeletion(deletion);
        }
    }
    // style cards
    CComponent* style_entry = top_level->FindChild("UI_MENU_ENTRY_STYLE_CARDS");
    viewport->AddFakeChild(style_entry->FindChild("UI_STYLE_CARDS_DESC"));
    long style_count = hair_count + tattoo_count + facial_hair_count;
    if (inventory_tc && style_count > 0) {
        NUISystem::CText* text = static_cast<NUISystem::CText*>(style_entry->FindChild("UI_STYLE_CARDS_BUTTON")->FindChild("UI_STYLE_CARDS_TEXT"));
        CWideString label(text->GetValue());
        label += CTCInventoryStats::GetXString();
        label += GFIntToWideString(style_count);
        text->SetValue(label, true, true);
        NUISystem::CText* sub_title = static_cast<NUISystem::CText*>(style_entry->FindChild("UI_STYLE_CARDS_SUB_TITLE"));
        if (sub_title)
            sub_title->SetValue(label, true, true);
        CCountedPointer<CAction> action(new CAction);
        action->ActionType = UI_ACTION_TYPE_CONSTRUCT_STYLE_CARDS_LIST;
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        action->UIActionParams.push_back(&static_cast<CActionParam&>(*style_entry));
        action->UIActionParams.push_back(new CActionParamSLONG(facial_hair_count));
        action->UIActionParams.push_back(new CActionParamSLONG(hair_count));
        action->UIActionParams.push_back(new CActionParamSLONG(tattoo_count));
        style_entry->AddPreAction(action);
    } else {
        style_entry->MakeUnavailable();
    }

    // stats: experience and personality
    if (stats_tc) {
        CComponent* stats_entry = top_level->FindChild("UI_MENU_ENTRY_STATS");
        CComponent* experience_entry = top_level->FindChild("UI_MENU_ENTRY_EXPERIENCE");
        CComponent* personality_entry = top_level->FindChild("UI_MENU_ENTRY_PERSONALITY");
        CComponent* desc = personality_entry->FindChild("UI_PERSONALITY_DESC");
        if (desc)
            viewport->AddFakeChild(desc);
        desc = experience_entry->FindChild("UI_EXPERIENCE_DESC");
        if (desc)
            viewport->AddFakeChild(desc);
        CCountedPointer<CComponent> ring_experience(CManager::GetInstance()->CreateComponent("UI_RING_PIC_EXP_STATS", false));
        experience_entry->AddChild(ring_experience);
        CCountedPointer<CComponent> ring_personality(CManager::GetInstance()->CreateComponent("UI_RING_PIC_PERSONALITY", false));
        personality_entry->AddChild(ring_personality);
        CCountedPointer<CAction> experience_action(new CAction);
        experience_action->ActionType = UI_ACTION_TYPE_CONSTRUCT_EXPERIENCE_LIST;
        experience_action->UIActionParams.push_back(&static_cast<CActionParam&>(*experience_entry));
        experience_action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        experience_entry->AddPreAction(experience_action);
        experience_entry->AddAction(back_table_text);
        experience_entry->AddAction(back_thumb_stick);
        experience_entry->AddAction(back_bottom_backdrop);
        experience_entry->AddAction(back_screen_backdrop);
        CCountedPointer<CAction> personality_action(new CAction);
        personality_action->ActionType = UI_ACTION_TYPE_CONSTRUCT_PERSONALITY_LIST;
        personality_action->UIActionParams.push_back(&static_cast<CActionParam&>(*personality_entry));
        personality_action->UIActionParams.push_back(&static_cast<CActionParam&>(*top_level));
        personality_entry->AddPreAction(personality_action);
        personality_entry->AddAction(back_menu_screen);
    } else {
        CComponent* experience_entry = top_level->FindChild("UI_MENU_ENTRY_EXPERIENCE");
        CComponent* personality_entry = top_level->FindChild("UI_MENU_ENTRY_PERSONALITY");
        CComponent* experience_arrow = experience_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        CComponent* desc = personality_entry->FindChild("UI_PERSONALITY_DESC");
        if (desc)
            viewport->AddFakeChild(desc);
        desc = experience_entry->FindChild("UI_EXPERIENCE_DESC");
        if (desc)
            viewport->AddFakeChild(desc);
        CCountedPointer<CComponent> ring_experience(CManager::GetInstance()->CreateComponent("UI_RING_PIC_EXP_STATS", false));
        experience_entry->AddChild(ring_experience);
        CCountedPointer<CComponent> ring_personality(CManager::GetInstance()->CreateComponent("UI_RING_PIC_PERSONALITY", false));
        personality_entry->AddChild(ring_personality);
        if (experience_arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            experience_arrow->SetDeletion(deletion);
        }
        experience_entry->MakeUnavailable();
        CComponent* personality_arrow = personality_entry->FindChild("UI_SELECTION_ARROW_RIGHT");
        if (personality_arrow) {
            CDeletion deletion;
            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
            personality_arrow->SetDeletion(deletion);
        }
        personality_entry->MakeUnavailable();
    }

    Components.push_back(top_level);
    for (unsigned long i = 0; i < Components.size(); ++i) {
        Components[i]->Initialise();
        Components[i]->ChangeState(5);
    }
}
