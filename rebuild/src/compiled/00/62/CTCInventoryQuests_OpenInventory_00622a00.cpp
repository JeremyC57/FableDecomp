// CTCInventoryQuests::OpenInventory() -- opens the quest screen: hands the main player's creature to the UI manager, forgets the previous hero-info screen; when not already the inventory screen fades the music / sound / ambient groups, hands over the hero, turns 3D scene rendering off, counts the menu open and remembers the current hero-info screen (else selects this screen's type); records the guild validity, rebuilds the quest card list and clamps the selected index, opens the base inventory, and outside the inventory screen builds the quest selection menu from the building's hero-centre quest cards (live ones only)
// Leaf: ?OpenInventory@CTCInventoryQuests@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00622A00, 877 bytes, thiscall `ret`; speed TU (no frame pointer, `sub esp,24h`). Identity: Ego_r.pdb /
//   FableWin.pdb ?OpenInventory@CTCInventoryQuests@@UAEXXZ, CTCInventoryQuests vtable 0x012585B4 slot 30.
// Callees: NUISystem::CManager::GetInstance 0x0041E5F2 / SetPlayerThing (slot 26) / SetQuestInventory (slot 17) / CreateComponent 0x0041DB1D /
//   OnOpenMenus 0x0041D122; CMainGameComponent::Get (PMainGameComponent 0x013B86A0) -> GetPlayerManager (inline, +0x1C) ->
//   CPlayerManager::GetMainPlayer 0x00449970 -> CPlayer::GetPControlledCreature 0x00487DC0; GetDisplayEngine / Get3DEngine (inline) ->
//   CIEngine::Enable3DSceneRendering (slot 76); PSoundManager 0x013B8394 CASoundManager::SetGainFader (slot 46); NHeroInformationScreens::CBase
//   GetScreenType (slot 2) / IsInventoryScreen (slot 3) through the +0x130 base; CPlayerGui::Get (PClass 0x013B8790) ->
//   GetPHeroInfoScreensManager 0x00437CD0 -> NHeroInformationScreens::CManager::GetCurrentScreen 0x0066E7BD / SelectScreenOfType 0x0066E799;
//   CTCInventoryBase GetInventoryGuildValidity (slot 49), GetSelectedCategoryIndex 0x005BC4B2 / SetSelectedCategoryIndex 0x005BC4B6,
//   GetBuildingThing 0x005BC4C4, OpenInventory 0x005BD4EA (qualified); CTCInventoryQuests::InitialiseQuestCardList 0x00621A20 /
//   ConstructQuestList 0x0061B610; CThing::GetTC<CTCHeroCentre> (expanded: LowerBound 0x0040F020); CTCHeroCentre::GetQuestCards 0x0077DBC0;
//   CBaseIntelligentPointer::GetPItem 0x00A01B50; vector<CThing*> _M_insert_overflow 0x00436AC0, vector<CComponent*> 0x00485DBC;
//   _Rb_tree<CThing*,..>::_M_erase 0x00623560 (AvailableQuests.clear() inline); vector<CIntelligentPointer<CThing> >::erase 0x004AC0F0
//   (DisplayQuests.clear()); free 0x00BFEA14.
// Layout: Ego_r.pdb members at the retail offsets: IsGuildValid +0x14C, QuestCardCount +0x150, ActiveTestQuestMode +0x157,
//   PreviousScreenType +0x168, AvailableQuests +0x16C, DisplayQuests +0x178 (overlay FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY).
// Shape: the menu is held as a CList* (ConstructQuestList's parameter type): the push_back into Components then copies it into a
//   temporary (`mov [esp+18h],ebx`), where a CComponent* local is itself address-taken and spilled. The four GetPItem calls per card are
//   retail's (the third result unused).
// Headers: overlay FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY, FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX,
//   FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS (hdr/README.md). Locals manager, list, quests, building, centre, cards are descriptive.
// Same-TU callee: retail defines ConstructQuestList 0x0061B610 earlier in this TU, and c2 sees from that body that it keeps no reference to
//   the vector: the local quest list's _M_finish then stays in ebp across the card loop (retail 0x00622C49 `cmp ebp,[esp+30h]` ...
//   `add ebp,4`); with a declaration only the vector escapes at the call and every push_back reloads it. The body below is the landed
//   row's text, unchanged (rebuild/src/compiled/00/61/CTCInventoryQuests_ConstructQuestList_0061b610.cpp, byte-proven; this file also
//   grades RELOCATION_MATCH against 0x0061B610), with that row's own opt-ins and includes.
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDCATEGORYINDEX
#define FABLE_RETAIL_CTCINVENTORYQUESTS_OPENINVENTORY
#define FABLE_RETAIL_CTCHEROCENTRE_GETQUESTCARDS
#define FABLE_RETAIL_CMANAGER_MENUCOUNT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
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
#include "engine/NUISystem_CDeletion.h"
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
#include "engine/ETCInterfaceType.h"
#include "engine/CTCHeroCentre.h"
#include "engine/CASoundManager.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CPlayerManager.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/NHeroInformationScreens_CManager.h"

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
    while (*a != 0 || *b != 0) {
        if (*a < *b)
            return -1;
        if (*a > *b)
            return 1;
        ++a;
        ++b;
    }
    return 0;
}

// FableWin.pdb ?GFIntToWideString@@YI?AVCWideString@@J@Z (Ego_r.pdb 0x00404F90): inline in retail (the landed
// CTCQuickAccessMenu::ConstructItemCategoryList 0x00626E40 spells the same local body).
inline CWideString __fastcall GFIntToWideString(long value)
{
    CWideString text;
    text.Sprintf(L"%d", value);
    return text;
}
// Ego_r.pdb / FableWin.pdb ?IsQuestCardAvailable@CTCInventoryQuests@@QBE_NABVCTCQuestCard@@@Z (retail out-of-line copy 0x0061A260):
// inline in retail, expanded here at 0x0061D389..0x0061D3E2 (the started flag rides in bl across the string and def releases).
inline bool CTCInventoryQuests::IsQuestCardAvailable(const CTCQuestCard& quest_card) const
{
    if (CQuestManager::Get().IsActiveQuestStartedInRegion(quest_card.GetPDef()->RegionName.GetString()))
        return false;
    return true;
}
void CTCInventoryQuests::ConstructQuestList(const std::vector<CThing*>& quests, CList* list, QuestScreenType type, CList* parent)
{
    std::vector<CThing*> sorted_quests[3];
    CManager* manager = CManager::GetInstance();
    CCountedPointer<CComponent> back_for_list(manager->CreateComponent("UI_BACK_FOR_LIST", false));
    CCountedPointer<CComponent> select_for_list(manager->CreateComponent("UI_SELECT_FOR_LIST", false));
    unsigned long i = 0;
    if (quests.size() != 0) {
        for (; i != quests.size(); ++i) {
            const CTCQuestCard* quest_card;
            if (!quests[i]->PeekTC(&quest_card))
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

    CList* frame_parent = parent ? parent : list;
    CCountedPointer<CComponent> world_map(manager->CreateComponent("UI_WORLD_MAP", false));

    CZoomingComponent* zooming_map = static_cast<CZoomingComponent*>(world_map.Get());
    C2DVector zoom_center = misc_def->RingCenter;
    C2DVector min_zoom(0.1f, 0.1f);
    C2DVector max_zoom(2.0f, 2.0f);
    zooming_map->SetMaxZoom(max_zoom);
    zooming_map->SetMinZoom(min_zoom);
    zooming_map->SetZoomCenter(zoom_center);

    CComponent* description_viewport = frame_parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    list->InitialiseOffsets(quests.size());
    CCountedPointer<CComponent> mini_map_viewport(manager->CreateComponent("UI_MINI_MAP_VIEWPORT_FOR_MAP_SCREEN", false));
    list->AddNonScrollingChild(mini_map_viewport);

    unsigned long state_index = 0;
    for (long category = 0; category != 3; ++category) {
        unsigned long j = 0;
        std::vector<CThing*>& category_quests = sorted_quests[category];
        std::vector<CThing*>::iterator first = category_quests.begin();
        unsigned long count = category_quests.size();
        if (count != 0)
        for (; j != count; ++j) {
            CThing* thing = first[j];
            CTCQuestCard* quest_card = 0;
            thing->GetTC(&quest_card);
            CWideString title = L"   " + quest_card->GetQuestName();
            CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(title, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW",
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
            CCountedPointer<CComponent> mini_map(manager->CreateComponent("UI_QUEST_MENU_MINI_MAP_TEMPLATE", false));
            CViewport* viewport = static_cast<CViewport*>(mini_map_viewport.Get());
            CCountedPointer<CComponent> mini_map_sprite(manager->CreateComponent("UI_MAP_MENU_MINI_MAP_TEMPLATE_SPRITE", false));
            if (mini_map_sprite->GetType() == UI_TYPE_SPRITE)
                static_cast<CSprite*>(mini_map_sprite.Get())->SetGraphicIndex(GetMiniMapGraphic(*region));
            mini_map->AddChild(mini_map_sprite);
            viewport->AddFakeChild(mini_map.Get());
            CTCInventoryMap* inventory_map = 0;
            GetParentThing().GetTC(&inventory_map);
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
            list->AddNonScrollingChild(back_for_list);
            list->AddNonScrollingChild(select_for_list);

            CCountedPointer<CComponent> title_text(manager->CreateComponent("UI_TITLE_QUEST_TEXT", false));
            CCountedPointer<CText> title_as_text;
            title_as_text.SetStaticCast(title_text);
            title_as_text->SetValue(title, true, true);
            entry->AddChild(title_text);
            CCountedPointer<CComponent> quest_icon(manager->CreateComponent("UI_TITLE_QUEST_SPRITE", false));
            static_cast<CSprite*>(quest_icon.Get())->SetGraphicIndex(icon_index);
            entry->AddChild(quest_icon);
            CCountedPointer<CComponent> description(manager->CreateComponent("UI_COMPONENT_CONTAINER_DESCRIPTION_TEMPLATE", false));
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

            CCountedPointer<CComponent> sub_list(manager->CreateComponent("UI_SUB_LIST", false));
            CCountedPointer<CList> sub_list_as_list;
            sub_list_as_list.SetStaticCast(sub_list);
            entry->AddChild(sub_list);
            sub_list_as_list->AddNonScrollingChild(mini_map);
            sub_list_as_list->AddNonScrollingChild(back_for_list);
            if (type == QUEST_SCREEN_TYPE_AVAILABLE) {
                sub_list_as_list->AddNonScrollingChild(select_for_list);
                long renown_needed = 0;
                std::map<EQuestCardPrerequisite, long>::const_iterator it = quest_card->GetPDef()->Prerequisites.find(QUEST_PREREQUISITE_RENOWN);
                if (it != quest_card->GetPDef()->Prerequisites.end())
                    renown_needed = it->second;
                if (hero_stats->GetRenownLevel() < renown_needed || !IsQuestCardAvailable(*quest_card)) {
                        entry->MakeUnavailable();
                        entry->RemoveChild(entry->GetChildren().size() - 1);
                        CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
                        if (arrow) {
                            CDeletion deletion;
                            deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                            arrow->SetDeletion(deletion);
                        }
                } else {
                    CWideString take_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAKE_QUEST");
                    CCountedPointer<CComponent> take_entry(manager->CreateMenuEntryComponent(take_text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW",
                                                                                         0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
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
                    long num_boasts = quest_card->GetPDef()->NumBoasts;
                    long num_entries = (num_boasts != 0) + 1;
                    sub_list_as_list->InitialiseOffsets(num_entries);
                    sub_list_as_list->AddScrollingChildAndSetOffsets(take_entry);
                    CCountedPointer<CAction> delete_action(new CAction);
                    delete_action->ActionType = UI_ACTION_TYPE_DELETE;
                    delete_action->UIActionParams.push_back(entry.Get());
                    take_entry->AddAction(delete_action);
                    if (quest_card->GetPDef()->NumBoasts != 0) {
                        CWideString take_boast_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_TAKE_QUEST_BOAST");
                        CCountedPointer<CComponent> take_boast_entry(manager->CreateMenuEntryComponent(take_boast_text, 0.0f, 1.0f,
                                                                     "UI_WEAPON_MENU_ENTRY_ACTION_TEMPLATE_NEW", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                        sub_list_as_list->AddScrollingChildAndSetOffsets(take_boast_entry);
                        take_boast_entry->AddAction(take_boast_action);
                        CCountedPointer<CAction> teleport_action(new CAction);
                        teleport_action->ActionType = UI_ACTION_TYPE_TELEPORT_TO_BOAST;
                        take_boast_entry->AddAction(teleport_action);
                    }
                }
            } else if (type == QUEST_SCREEN_TYPE_CURRENT) {
                CWideString zoom_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
                CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                sub_list_as_list->AddScrollingChild(zoom_entry);
            } else if (type == QUEST_SCREEN_TYPE_OTHER) {
                CWideString zoom_text = text_bank.GetTextBySymbol("TEXT_GUI_MENU_ZOOMED_REGION");
                CCountedPointer<CComponent> zoom_entry(manager->CreateMenuEntryComponent(zoom_text, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
                sub_list_as_list->AddScrollingChild(zoom_entry);
            }

            C2DVector region_offset(0.0f, 0.0f);
            CCharString state_region_name = quest_card->GetRegionName();
            if (region_number > 0)
                region_offset = region->GetWorldMapOffset();
            CUIStateDef state_def;
            state_def.Position.X = 444.0f - region_offset.X;
            state_def.Position.Y = 186.0f - region_offset.Y;
            state_def.UpdateTime = 0.2f;
            std::pair<const unsigned long, CUIStateDef> state(state_index + 8, state_def);
            world_map->GetStates().insert(world_map->GetStates().begin(), state);
            ++state_index;
        }
    }

    if (type == QUEST_SCREEN_TYPE_AVAILABLE)
        list->SetDefaultChild(0L);
    else
        list->SetDefaultChild(quests.size() - 1);
    CTCInventoryMap* inventory_map;
    if (GetParentThing().GetTC(&inventory_map) && inventory_map) {
        std::set<long> regions;
        std::vector<CRegion> display_regions;
        inventory_map->GetRegionsToDisplay(regions);
        for (i = 0; i < quests.size(); ++i) {
            CTCQuestCard* quest_card = 0;
            quests[i]->GetTC(&quest_card);
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
    CCountedPointer<CComponent> map_viewport(manager->CreateComponent("UI_MAP_VIEWPORT", false));
    map_viewport->AddChild(world_map);
    list->AddNonScrollingChild(map_viewport);
}

void CTCInventoryQuests::OpenInventory()
{
    NUISystem::CManager::GetInstance()->SetPlayerThing(CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer().GetPControlledCreature());
    PreviousScreenType = NHeroInformationScreens::NULL_SCREEN_TYPE;
    if (!IsInventoryScreen()) {
        if (PSoundManager) {
            PSoundManager->SetGainFader(0x10, 0.0f, 500.0f);
            PSoundManager->SetGainFader(0x40, 0.0f, 500.0f);
            PSoundManager->SetGainFader(0x80, 0.0f, 500.0f);
        }
        NUISystem::CManager::GetInstance()->SetPlayerThing(ParentThing);
        CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().Enable3DSceneRendering(false);
        NUISystem::CManager::OnOpenMenus();
        PreviousScreenType = CPlayerGui::Get().GetPHeroInfoScreensManager()->GetCurrentScreen()->GetScreenType();
        ActiveTestQuestMode = false;
    } else {
        CPlayerGui& gui = CPlayerGui::Get();
        gui.GetPHeroInfoScreensManager()->SelectScreenOfType(GetScreenType());
        ActiveTestQuestMode = true;
    }
    IsGuildValid = GetInventoryGuildValidity();
    InitialiseQuestCardList();
    if (QuestCardCount > 0) {
        long last = QuestCardCount - 1;
        if (GetSelectedCategoryIndex() > last)
            SetSelectedCategoryIndex(last);
    }
    NInventory::CTCInventoryBase::OpenInventory();
    if (IsInventoryScreen())
        return;
    NUISystem::CManager::GetInstance()->SetQuestInventory(this);
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    NUISystem::CList* list = static_cast<NUISystem::CList*>(manager->CreateComponent("UI_QUESTS_SELECTION_MENU", false));
    std::vector<CThing*> quests;
    CThing* building = GetBuildingThing();
    if (building && !building->IsDead()) {
        CTCHeroCentre* centre = 0;
        building->GetTC(TCI_HERO_CENTRE, &centre);
        std::vector<CIntelligentPointer<CThing> > cards;
        centre->GetQuestCards(cards);
        for (std::vector<CIntelligentPointer<CThing> >::iterator it = cards.begin(); it != cards.end(); ++it) {
            if (it->Get() && !it->Get()->IsDead()) {
                it->Get();
                quests.push_back(it->Get());
            }
        }
    }
    AvailableQuests.clear();
    DisplayQuests.clear();
    ConstructQuestList(quests, list, (QuestScreenType)0, 0);
    list->Initialise();
    list->ChangeState(5);
    Components.push_back(list);
}