// CTCInventoryQuests::ConstructQuestListPC(NUISystem::CComponent* parent, const std::vector<CThing*>& quests, NUISystem::CList* list, QuestScreenType type, NUISystem::CList* frame_list) -- build the PC quest list: one menu entry per quest card (core, optional, vignette order) with its minimap, description text, rewards, boasts and take / delete actions, then the world-map zoom state and region markers
// Grade: DIFFER(12356v12356) -- retail size, 464/464 calls, frame 0x41C = retail, NO inline-budget device, NO assert, no flag (plain hdr overlay). What closed the budget: `CTCInventoryMap* inventory_map = 0;` at the mini-map site (retail `xor ecx,ecx` before the TC lookup, pointer kept in ecx; the uninitialised local was homed in memory and cost the six inline_depth devices). Residue: frame-slot order (55 of 101 frame symbols off retail slots: tools/fa.sh + fmis.py) and the register choices that follow (agentC26)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0061E6D0, 12,356 bytes, thiscall `ret 0x14`.
// Leaf: ?ConstructQuestListPC@CTCInventoryQuests@@QAEXPAVCComponent@NUISystem@@ABV?$vector@PAVCThing@@V?$allocator@PAVCThing@@@std@@@std@@PAVCList@3@W4QuestScreenType@@2@Z
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CTCINVENTORYQUESTS_QUESTLIST
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SMART_POINTER_ACCESSORS
#define FABLE_RETAIL_CDATABANK
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTCINVENTORYMAP_STATGETINTERFACETYPE
#define FABLE_RETAIL_CTCQUESTCARD_STATGETINTERFACETYPE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_SET
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CSPRITE
#define FABLE_RETAIL_CCOMPONENTCONTAINER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_INLINE
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR
#define FABLE_RETAIL_CUISCREENUNITCONVERTER_VECTOR_INLINE
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CUISTATEDEF_IMPLICIT_DTOR
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCHEROSTATS_GETRENOWNLEVEL
#define FABLE_RETAIL_CCHARSTRING_GETASUPPERCASE
#define FABLE_RETAIL_CTCQUESTCARD
#define FABLE_RETAIL_CTCQUESTCARD_ACCESSORS
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CQUESTMANAGER_COMPLETIONUI
#define FABLE_RETAIL_CQUESTMANAGER_REGIONSTARTED
#define FABLE_RETAIL_CTCINVENTORYBASE_MINIMAPGRAPHIC
#define FABLE_RETAIL_CTCINVENTORYMAP_QUESTMARKERS
#define FABLE_RETAIL_CZOOMINGCOMPONENT_ZOOM
#define FABLE_RETAIL_CBANKFILE_FINDINDEX
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "fable_game_text_bank.h"
#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CRegion.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CBankFile.h"
#include "engine/CGraphicDataBank.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/NUISystem_CUIScreenUnitConverter.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CUIStateDef.h"
#include "engine/NUISystem_CUIState.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CZoomingComponent.h"
#include "engine/NUISystem_CComponentContainer.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/CText.h"
#include "engine/CTCHeroStats.h"
#include "engine/CQuestCardDef.h"
#include "engine/CTCQuestCard.h"
#include "engine/CQuestManager.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CTCInventoryQuests.h"

using namespace NUISystem;

#pragma auto_inline(off)
// Same-TU body of CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) (retail calls the instance 0x0042B312; as the landed
// CTCInventoryMap::ConstructRegionDescriptionShops 0x006001E0): VC7.1 then sees the def pointer is not captured.
template<class T> bool CDefinitionManager::GetDef(long index,CDefPointer<const T>* result)const {
    if(index<=0)return false;
    CDefPointer<const CDefClassBase> definition=GetPDefFromGlobalIndex(index);
    if(!definition.IsNull()) { result->SetDynamicCast(definition);return true; }
    return false;
}
#pragma auto_inline(on)
// Same-TU body of the protected static CBasicString<char>::Compare (out of line at 0x00411570, landed as global 0x00411570): retail
// expands it inside the inlined CCharString::operator== of the boast loop (0x0061FB4A..0x0061FB7A).
template<class T> long __fastcall CBasicString<T>::Compare(const T* a, const T* b)
{
    for (;;) {
        T ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        T cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

// FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z (Ego_r.pdb 0x00404F90): inline in retail (the landed
// CTCQuickAccessMenu::ConstructItemCategoryList 0x00626E40 spells the same local body).
inline CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}

void CTCInventoryQuests::ConstructQuestListPC(CComponent* parent, const std::vector<CThing*>& quests, CList* list,
                                              QuestScreenType type, CList* frame_list)
{
    std::vector<CThing*> sorted_quests[3];
    CManager* manager = CManager::GetInstance();
    unsigned long i = 0;
    if (quests.size() != 0) {
        for (; i != quests.size(); ++i) {
            const CTCQuestCard* quest_card;
            if (!quests[i]->PeekTC(TCI_QUEST_CARD, &quest_card))
                continue;
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                if (quest_card->IsCore())
                    sorted_quests[0].push_back(quests[i]);
                else if (quest_card->IsOptional() && !quest_card->GetPDef()->IsVignette)
                    sorted_quests[1].push_back(quests[i]);
                else if (quest_card->GetPDef()->IsVignette)
                    sorted_quests[2].push_back(quests[i]);
                else
                    sorted_quests[0].push_back(quests[i]);
            } else {
                sorted_quests[0].push_back(quests[i]);
            }
        }
    }

    CDefPointer<const CUIMiscThingsDef> misc_def;
    long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("UI_MISC_THINGS_DEF", 1);
    CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &misc_def);
    const NGameText::CDataBank& text_bank = CMainGameComponent::Get()->GetTextBank();
    CTCHeroStats* hero_stats = 0;
    GetParentThing().GetTC(TCI_HERO_STATS, &hero_stats);

    CList* frame_parent = frame_list;
    if (frame_parent == 0)
        frame_parent = list;
    CComponent* frame;
    if (type == QUEST_SCREEN_TYPE_AVAILABLE)
        frame = frame_parent->FindChild("PC_UI_MAP_FRAME");
    else
        frame = frame_parent->FindChild("PC_UI_FRAME_QUESTS");
    CCountedPointer<CComponent> world_map(manager->CreateComponent("PC_WORLD_MAP", false));

    CZoomingComponent* zooming_map = static_cast<CZoomingComponent*>(world_map.Get());
    C2DVector zoom_center = CUIScreenUnitConverter::ConvertVector(misc_def->HeroDollFrameTL_PC);
    zoom_center.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->HeroDollFrameEmulateListOffset;
    zoom_center.X += CManager::GetInstance()->GetUIScale().X * misc_def->PCRingCenter.X;
    zoom_center.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->PCRingCenter.Y;
    C2DVector max_zoom(1.5f, 1.5f);
    C2DVector min_zoom(0.2f, 0.2f);
    zooming_map->SetMaxZoom(max_zoom);
    zooming_map->SetMinZoom(min_zoom);
    zooming_map->SetZoomCenter(zoom_center);

    CComponent* description_viewport = parent ? parent->FindChild("PC_DESCRIPTION_VIEWPORT")
                                              : frame_parent->FindChild("PC_DESCRIPTION_VIEWPORT");
    list->InitialiseOffsets(quests.size());
    CCountedPointer<CComponent> movie_viewport(manager->CreateComponent("PC_EXPERIENCE_SPENDING_MOVIE_VIEWPORT", false));
    list->AddNonScrollingChild(movie_viewport);
    frame->AddPositionChild(movie_viewport.Get());

    unsigned long state_index = 0;
    for (long category = 0; category != 3; ++category) {
        unsigned long j = 0;
        std::vector<CThing*>& category_quests = sorted_quests[category];
        std::vector<CThing*>::iterator first = category_quests.begin();
        unsigned long count = category_quests.size();
        for (; j != count; ++j) {
            CThing* thing = first[j];
            CTCQuestCard* quest_card = 0;
            thing->GetTC(TCI_QUEST_CARD, &quest_card);
            CWideString title = L"   " + quest_card->GetQuestName();
            CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f, "PC_MENU_ENTRY_BRANCH_TEMPLATE",
                                                                                0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                CCountedPointer<CAction> close_action(new CAction);
                close_action->ActionType = UI_ACTION_TYPE_CLOSE_MENU;
                entry->AddActionOnBack(close_action);
            }

            CWorldMap& world_map_data = CMainGameComponent::Get()->GetWorld().GetWorldMap();
            CCharString region_name = quest_card->GetRegionName();
            long region_number = world_map_data.GetRegionNumberFromName(region_name);
            if (region_number == 0)
                region_number = world_map_data.GetLoadedRegion();
            CRegion* region = &world_map_data.GetRegion(region_number);
            CCharString mini_map_name = "MINIMAP_" + region_name.GetAsUpperCase();
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE", false));
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("PC_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            frame->AddPositionChild(mini_map.Get());
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(*region));
            mini_map->AddChild(mini_map_sprite);
            static_cast<CViewport*>(movie_viewport.Get())->AddFakeChild(mini_map.Get());
            CTCInventoryMap* inventory_map = 0;                     // retail zeroes it before the TC lookup (`xor ecx,ecx` 0x0061F0A8) and keeps it in ecx
            GetParentThing().GetTC(TCI_INVENTORY_MAP, &inventory_map);
            inventory_map->CreateMiniMapMarkers(mini_map.Get(), *region);
            AvailableQuests.insert(std::map<CThing*, CComponent*>::value_type(thing, entry.Get()));
            DisplayQuests.push_back(CIntelligentPointer<CThing>(thing));

            CCountedPointer<CComponent> icon;
            unsigned long icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_CORE");
            if (quest_card->IsCore()) {
                icon.Reset(manager->CreateComponent("UI_QUEST_SPRITE_CORE", false));
                icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_CORE");
            } else if (quest_card->GetPDef()->IsVignette) {
                icon.Reset(manager->CreateComponent("UI_QUEST_SPRITE_VIGNETTE", false));
                icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_VIGNETTE");
            } else {
                icon.Reset(manager->CreateComponent("UI_QUEST_SPRITE_OPTIONAL", false));
                icon_index = manager->GetGraphicsBank()->FindIndexBySymbol("HUD_ORB_QUEST_OPTIONAL");
            }
            entry->AddChild(icon);
            list->AddScrollingChildAndSetOffsets(entry);

            CCountedPointer<CComponent> title_text(manager->CreateComponent("PC_TITLE_QUEST_TEXT", false));
            frame->AddPositionChild(title_text.Get());
            CCountedPointer<CText> title_as_text;
            title_as_text.SetStaticCast(title_text);
            title_as_text->SetValue(title, true, true);
            entry->AddChild(title_text);
            CCountedPointer<CComponent> quest_icon(manager->CreateComponent("PC_ICON_INDEPENDANT", false));
            frame->AddPositionChild(quest_icon.Get());
            static_cast<CSprite*>(quest_icon.Get())->SetGraphicIndex(icon_index);
            entry->AddChild(quest_icon);
            CCountedPointer<CComponent> description(manager->CreateComponent("PC_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE_INDEPENDANT", false));
            frame->AddPositionChild(description.Get());
            CComponentContainer* container = static_cast<CComponentContainer*>(description.Get());

            CWideString text;
            if (quest_card->GetPDef()->Prerequisites.size() != 0) {
                long renown_needed = 0;
                std::map<EQuestCardPrerequisite, long>::const_iterator it = quest_card->GetPDef()->Prerequisites.find(QUEST_PREREQUISITE_RENOWN);
                if (it != quest_card->GetPDef()->Prerequisites.end())
                    renown_needed = it->second;
                if (hero_stats->GetRenownLevel() < renown_needed) {
                    text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_NOT_RENOWNED_ENOUGH") + CWideString(L"\n");
                    container->AddTextLine(text, CRGBColour(0xc8, 0x14, 0x14, 0xff), "", 0);
                }
            }
            text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_MENU_QUEST_SUMMARY");
            container->AddTextLine(text, CRGBColour(0xc8, 0xc8, 0x14, 0xff), "", 0);
            text = quest_card->GetQuestSummary() + L"\n";
            container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
            if (type == QUEST_SCREEN_TYPE_CURRENT) {
                if (quest_card->GetQuestObjective().GetLength() != 0) {
                    text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_CURRENT_OBJECTIVE");
                    container->AddTextLine(text, CRGBColour(0xc8, 0xc8, 0x14, 0xff), "", 0);
                    text = quest_card->GetQuestObjective() + L"\n";
                    container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
                }
            }
            {
                std::list<CQuestManager::CBoast>& boasts = CQuestManager::Get().GetQuestBoasts();
                CCharString script_name = quest_card->GetScriptQuestName();
                CWideString boast_text;
                bool any_boast = false;
                std::list<CQuestManager::CBoast>::iterator boast = boasts.begin();
                while (boast != boasts.end()) {
                    if (boast->ScriptName == script_name) {
                        boast_text += CWideString(L" - ");
                        boast_text += boast->Text;
                        boast_text += CWideString(L" (");
                        boast_text += text_bank.GetTextBySymbol("TEXT_GUI_MENU_WAGER");
                        boast_text += CWideString(L": ");
                        boast_text += GFIntToWideString(boast->Cost);
                        boast_text += CWideString(L", ");
                        boast_text += text_bank.GetTextBySymbol("TEXT_GUI_MENU_REWARD");
                        boast_text += CWideString(L": ");
                        boast_text += GFIntToWideString(boast->Reward);
                        boast_text += CWideString(L")");
                        any_boast = true;
                        if (boast->Failed) {
                            boast_text += CWideString(L" ");
                            boast_text += text_bank.GetTextBySymbol("TEXT_GUI_MENU_FAILED");
                        }
                        boast_text += CWideString(L"\n");
                    }
                    ++boast;
                }
                if (any_boast) {
                    text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_BOASTS");
                    container->AddTextLine(text, CRGBColour(0xc8, 0xc8, 0x14, 0xff), "", 0);
                    container->AddTextLine(boast_text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
                }
            }
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_MONEY_REWARD") + CWideString(L": ") + GFIntToWideString(quest_card->GetGoldReward());
            container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "HUD_ICON_MONEY_BAG", 0);
            text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_RENOWN_REWARD") + CWideString(L": ") + GFIntToWideString(quest_card->GetRenownReward());
            container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "HUD_ORB_QUEST_CORE", 0);
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_NUM_BOASTS") + CWideString(L": ") + GFIntToWideString(quest_card->GetPDef()->NumBoasts);
                container->AddTextLine(text, CRGBColour(0xff, 0xff, 0xff, 0xff), "", 0);
            }
            entry->AddChild(description);
            static_cast<CViewport*>(description_viewport)->AddFakeChild(description.Get());

            CCountedPointer<CComponent> sub_list(manager->CreateComponent("PC_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            entry->AddChild(sub_list);
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                long renown_needed = 0;
                std::map<EQuestCardPrerequisite, long>::const_iterator it = quest_card->GetPDef()->Prerequisites.find(QUEST_PREREQUISITE_RENOWN);
                if (it != quest_card->GetPDef()->Prerequisites.end())
                    renown_needed = it->second;
                if (hero_stats->GetRenownLevel() >= renown_needed) {
                    bool started = CQuestManager::Get().IsActiveQuestStartedInRegion(quest_card->GetPDef()->RegionName.GetString());
                    if (!started) {
                        CWideString take_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAKE_QUEST");
                        CCountedPointer<CComponent> take_entry(manager->CreateMenuEntryComponent(take_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                             0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                        take_entry->AddChild(mini_map);
                        CCountedPointer<CAction> take_action(new CAction);
                        take_action->ActionType = UI_ACTION_TYPE_TAKE_QUEST;
                        take_action->ThingActionParams.push_back(thing->GetDefGlobalIndex());
                        take_entry->AddAction(take_action);
                        CCountedPointer<CAction> take_boast_action(new CAction);
                        take_boast_action->ActionType = UI_ACTION_TYPE_TAKE_QUEST_FOR_BOAST;
                        take_boast_action->ThingActionParams.push_back(thing->GetDefGlobalIndex());
                        CCountedPointer<CAction> pop_action(new CAction);
                        pop_action->ActionType = UI_ACTION_TYPE_POP_MAP_STATE;
                        pop_action->UIActionParams.push_back(world_map.Get());
                        take_entry->AddAction(pop_action);
                        pop_action->ThingActionParams.push_back(thing->GetDefGlobalIndex());
                        sub_list_as_list->InitialiseOffsets((quest_card->GetPDef()->NumBoasts != 0) + 1);
                        sub_list_as_list->AddScrollingChildAndSetOffsets(take_entry);
                        CAction* delete_action_object = new CAction;
                        CCountedPointer<CAction> delete_action(delete_action_object);
                        delete_action->ActionType = UI_ACTION_TYPE_DELETE;
                        delete_action->UIActionParams.push_back(entry.Get());
                        take_entry->AddAction(delete_action);
                        if (quest_card->GetPDef()->NumBoasts != 0) {
                            CWideString take_boast_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAKE_QUEST_BOAST");
                            CCountedPointer<CComponent> take_boast_entry(manager->CreateMenuEntryComponent(take_boast_text, 0.0f, 1.0f,
                                                                         "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                            take_boast_entry->AddChild(mini_map);
                            sub_list_as_list->AddScrollingChildAndSetOffsets(take_boast_entry);
                            take_boast_entry->AddAction(take_boast_action);
                            CCountedPointer<CAction> teleport_action(new CAction);
                            teleport_action->ActionType = UI_ACTION_TYPE_TELEPORT_TO_BOAST;
                            take_boast_entry->AddAction(teleport_action);
                        }
                    } else {
                        entry->MakeUnavailable();
                        entry->RemoveChild(entry->GetChildren().size() - 1);
                    }
                } else {
                    entry->MakeUnavailable();
                    entry->RemoveChild(entry->GetChildren().size() - 1);
                }
            } else if (type == QUEST_SCREEN_TYPE_CURRENT) {
                CWideString zoom_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
                CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                sub_list_as_list->AddScrollingChild(zoom_entry);
                zoom_entry->AddChild(mini_map);
            } else if (type == QUEST_SCREEN_TYPE_OTHER) {
                CWideString zoom_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
                CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                sub_list_as_list->AddScrollingChild(zoom_entry);
                zoom_entry->AddChild(mini_map);
            }

            CCharString state_region_name = quest_card->GetRegionName();
            C2DVector region_offset;
            if (region_number > 0)
                region_offset = region->GetWorldMapOffset();
            CUIStateDef state_def;
            C2DVector origin = CUIScreenUnitConverter::ConvertVector(misc_def->HeroDollFrameTL_PC);
            origin.Y += CManager::GetInstance()->GetUIScale().Y * misc_def->HeroDollFrameEmulateListOffset;
            state_def.Position = misc_def->PCRingCenter - region->GetWorldMapOffset();
            state_def.Position.X *= CManager::GetInstance()->GetUIScale().X;
            state_def.Position.Y *= CManager::GetInstance()->GetUIScale().Y;
            state_def.Position += origin;
            state_def.UpdateTime = 0.5f;
            state_def.StateChangeType = (EStateChangeType)3;
            state_def.StateChangeFlag = 0x67;
            world_map->GetStates().insert(world_map->GetStates().begin(),
                                          std::pair<const unsigned long, CUIStateDef>(state_index + 8, state_def));
            ++state_index;
        }
    }

    if (type == QUEST_SCREEN_TYPE_AVAILABLE)
        list->SetDefaultChild(0L);
    else
        list->SetDefaultChild(quests.size() - 1);
    CTCInventoryMap* inventory_map;
    if (GetParentThing().GetTC(TCI_INVENTORY_MAP, &inventory_map) && inventory_map) {
        std::set<long> regions;
        std::vector<CRegion> display_regions;
        inventory_map->GetRegionsToDisplay(regions);
        for (i = 0; i < quests.size(); ++i) {
            CTCQuestCard* quest_card = 0;
            quests[i]->GetTC(TCI_QUEST_CARD, &quest_card);
            long region_number = PeekWorld().PeekWorldMap().GetRegionNumberFromName(quest_card->GetRegionName());
            if (region_number > 0)
                regions.insert(region_number);
        }
        for (std::set<long>::iterator it = regions.begin(); it != regions.end(); ++it) {
            if (PeekWorld().PeekWorldMap().GetRegion(*it).IsOnWorldMap())
                display_regions.push_back(PeekWorld().PeekWorldMap().GetRegion(*it));
        }
        inventory_map->CreateMapMarkers(world_map.Get(), display_regions);
    }
    list->AddNonScrollingChild(world_map);
}
