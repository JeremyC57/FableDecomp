// CTCInventory::OpenPCInventoryMenu() -- open the PC inventory screen: drop the old menu components, reuse an open PC_INVENTORY_LIST or build one (frame children, gold amount, the clothing / weapons / items / style cards entries with their counts and construct actions), observe the scroll / back / close clickables
// Leaf: ?OpenPCInventoryMenu@CTCInventory@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058807D, 4997 bytes, thiscall `leave ; ret`
//   Identity: Ego_r.pdb / FableWin.pdb ?OpenPCInventoryMenu@CTCInventory@@QAEXXZ; callers CTCInventory::OpenInventoryShortcut 0x0058E99F
//   (call 0x0058EA07) and CWorld::EAOpenPCInventoryMenu 0x0062C700 (call 0x0062C747); the "PC_INVENTORY_LIST" literal; CurrentOpenMenu 3.
//   Size-optimised TU with the biased frame pointer c2 emits by itself for a large frame (`lea ebp,[esp-78h]`): optimize("s", on)
//   only (optimize("y", off) gives `mov ebp,esp`); no cookie, no EH.
// Callees: DeleteMenuComponents 0x00587A41; NUISystem::CManager::GetInstance 0x0041E5F2 / CreateComponent 0x0041DB1D; inline
//   CCharString::operator==(const char*) (0x004115A0); CPlayerManager::GetMainPlayer 0x00449960, CPlayer::GetPControlledCreature
//   0x00487DC0 / PeekPControlledCreature 0x00487DD0; inline CThing::GetTC / PeekTC (bitset test 0x00410DE0, LowerBound 0x0040F020 /
//   0x004365B0); GFIntToWideString 0x00405F00; CTCInventoryBase::GetAllItems 0x005BF482 / GetNumberOfItemsOfTypeInInventory 0x005BDF08;
//   CTCInventoryItem::IsBook 0x005D9970 / IsWeapon 0x005D97F0 / IsExpression 0x005D9830 / GetHairCardDef 0x005D9A10 / GetTattooCardDef
//   0x005D9AA0; vector erase 0x00593139; CTCBase::GetDefinitionManager 0x006869D0; CDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C /
//   <CThingObjectDef> 0x0042B040, GetDefGlobalIndexFromName 0x009AD410, GetPDefFromGlobalIndex 0x009AD9E0; CDefClassBase::GetSubDef
//   <CAugmentationDef> 0x0042B248 / <CHeroSuitDef> 0x005935C4 / <CAppearanceModifierDef> 0x0057FF92 / <CAbilityDef> 0x005936C2; map
//   iterator increment 0x004292C0; CTattooDef::IsInvalidCustomTattoo 0x00581765; CTCInventoryStats::GetXString 0x005CC772; CAction()
//   0x0042BE50; CCountedPointer<CAction>(T*) 0x0042AA29 / ReleaseData 0x00429160; list push_back 0x0042B646; Components push_back
//   0x00486024; CComponent / CViewport / CText virtuals as the landed twin OpenPCSkillsMenu 0x00587BFF.
// Shape: the landed twin 0x00587BFF (find-or-create, frame children, tail) and the landed item row 0x005A8C66 (GetAllItems / erase /
//   card defs / GetMoney); CThingPlayerCreature and CAbilityDef stand-ins as those landed rows (0x005A8C66, 0x00584B86).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_VECTOR_ERASE_OUTOFLINE
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CTCINVENTORYCLOTHING_CALCULATE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CAUGMENTATIONDEF
#define FABLE_RETAIL_CHEROSUITDEF_STATGETTYPENAME
#define FABLE_RETAIL_CAPPEARANCEMODIFIERDEF_STATGETTYPENAME
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryStats.h"
#include "engine/CTCInventoryClothing.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCHeroStats.h"
#include "engine/CTattooDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CHeroSuitDef.h"
#include "engine/CAppearanceModifierDef.h"
#include "engine/CAugmentationDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingObjectDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/CCountedPointer.h"

using NUISystem::CViewport;
using NUISystem::CText;
using NUISystem::CAction;
using NUISystem::CActionParam;
using NUISystem::CActionParamSLONG;

class CThingPlayerCreature : public CThing { };

// CAbilityDef has no shared header: RTTI .?AVCAbilityDef@@ : CSubDefClassBase
class CAbilityDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CAbilityDef", -1); }
};

CWideString FABLE_FASTCALL GFIntToWideString(long value);          // 0x00405F00 ?GFIntToWideString@@YI?AVCWideString@@J@Z

void CTCInventory::OpenPCInventoryMenu()
{
    DeleteMenuComponents();
    CurrentOpenMenu = (CURRENT_OPEN_MENU)3;

    NUISystem::CComponent* list = 0;
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    for (unsigned long i = 0; i < Components.size(); ++i) {
        if (list)
            break;
        if (Components[i]->GetName() == "PC_INVENTORY_LIST")
            list = Components[i];
    }

    if (!list) {
        list = manager->CreateComponent("PC_INVENTORY_LIST", false);
        NUISystem::CComponent* frame = list->FindChild("PC_UI_FRAME");
        if (frame) {
            NUISystem::CComponent* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");
            if (viewport)
                frame->AddPositionChild(viewport);
            NUISystem::CComponent* child = list->FindChild("PC_TITLE_INVENTORY");
            if (child)
                frame->AddPositionChild(child);
            NUISystem::CComponent* gold = list->FindChild("PC_GOLD_AMOUNT_TRADE_SCREEN_TOP_LEVEL");
            if (gold) {
                CThingPlayerCreature* creature = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
                CTCHeroStats* stats = 0;
                creature->GetTC(TCI_HERO_STATS, &stats);
                static_cast<CText*>(gold)->SetValue(GFIntToWideString(stats->GetMoney()), true, true);
                frame->AddPositionChild(gold);
            }
            child = list->FindChild("PC_GOLD_COIN_SPRITE_TOP_LEVEL");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_WEAPONS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_WEAPON_DESC");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_WEAPON_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_ITEMS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_ITEMS_DESC");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_ITEMS_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_CLOTHING");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_CLOTHING_DESC");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_CLOTHING_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_STYLE_CARDS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_STYLE_CARDS_DESC");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_STYLE_CARDS_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_STYLE_CARDS_TITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_HAIRCUTS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_HAIRCUTS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                static_cast<CViewport*>(viewport)->AddFakeChild(child);
            }
            child = list->FindChild("PC_HAIRCUTS_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_FACIAL_HAIR");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_FACIAL_HAIR_DESC");
            if (child) {
                frame->AddPositionChild(child);
                static_cast<CViewport*>(viewport)->AddFakeChild(child);
            }
            child = list->FindChild("PC_FACIAL_HAIR_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_RING_PIC_TATTOOS");
            if (child)
                frame->AddPositionChild(child);
            child = list->FindChild("PC_TATTOOS_DESC");
            if (child) {
                frame->AddPositionChild(child);
                static_cast<CViewport*>(viewport)->AddFakeChild(child);
            }
            child = list->FindChild("PC_TATTOOS_SUBTITLE");
            if (child)
                frame->AddPositionChild(child);
        }

        CTCInventoryWeapons* weapons = 0;
        CTCInventory* inventory = 0;
        CTCInventoryClothing* clothing = 0;
        ParentThing->GetTC(TCI_INVENTORY_WEAPONS, &weapons);
        ParentThing->GetTC(TCI_INVENTORY, &inventory);
        ParentThing->GetTC(TCI_INVENTORY_CLOTHING, &clothing);
        const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
        const CTCInventoryWeapons* heroWeapons = 0;
        std::vector<std::pair<long, long> > items;
        GetAllItems(items);
        for (std::vector<std::pair<long, long> >::iterator it = items.begin(); it != items.end(); ) {
            if (CTCInventoryItem::IsBook(it->first))
                it = items.erase(it);
            else
                ++it;
        }
        hero->PeekTC(TCI_INVENTORY_WEAPONS, &heroWeapons);
        heroWeapons->GetAllItems(items);
        if (clothing)
            clothing->GetAllItems(items);

        long weaponsCount = 0;
        long itemsCount = 0;
        long clothingCount = 0;
        long haircutCount = 0;
        long facialHairCount = 0;
        long tattooCount = 0;
        for (unsigned long i = 0; i < items.size(); ++i) {
            CDefPointer<const CThingBaseDef> thingDef;
            GetDefinitionManager().CDefinitionManager::GetDef(items[i].first, &thingDef);
            CDefPointer<const CAugmentationDef> aug;
            if (thingDef->GetSubDef(&aug))
                continue;
            CDefPointer<const CHeroSuitDef> suit;
            CDefPointer<const CAppearanceModifierDef> appearance;
            if (CTCInventoryItem::IsWeapon(items[i].first)) {
                weaponsCount += items[i].second;
            } else if (thingDef->GetSubDef(&suit)) {
                for (std::map<EClothingSuitPart, long>::const_iterator it = suit->SuitParts.begin(); it != suit->SuitParts.end(); ++it) {
                    if (clothing->GetNumberOfItemsOfTypeInInventory(it->second) > 0) {
                        if (items[i].first == GetDefinitionManager().GetDefGlobalIndexFromName("HERO_SUIT_NAKED")
                            || (it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_HAT")
                                && it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_SHIRT")
                                && it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_GLOVES")
                                && it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_TROUSERS")
                                && it->second != GetDefinitionManager().GetDefGlobalIndexFromName("OBJECT_HERO_NO_BOOTS"))) {
                            clothingCount += items[i].second;
                            break;
                        }
                    }
                }
            } else if (thingDef->GetSubDef(&appearance)) {
                if (appearance->SuitPart > 0 && appearance->SuitPart <= 5)
                    clothingCount += items[i].second;
            } else if (!CTCInventoryItem::IsExpression(items[i].first)) {
                CDefPointer<const CHairCardDef> hair = CTCInventoryItem::GetHairCardDef(items[i].first);
                CDefPointer<const CTattooDef> tattoo = CTCInventoryItem::GetTattooCardDef(items[i].first);
                if (hair.IsNull() && tattoo.IsNull()) {
                    CDefPointer<const CThingObjectDef> objDef;
                    CMainGameComponent::Get()->PeekDefinitionManager().CDefinitionManager::GetDef(items[i].first, &objDef);
                    CDefPointer<const CAbilityDef> ability;
                    if (objDef.IsNull() || !objDef->GetSubDef(&ability))
                        itemsCount += items[i].second;
                } else if (!hair.IsNull()) {
                    CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(hair->HairObject);
                    CDefPointer<const CAppearanceModifierDef> mod;
                    if (!def.IsNull())
                        def->GetSubDef(&mod);
                    if (!mod.IsNull()) {
                        if ((mod->CoversBodyAreaFlags & 0x80) || (mod->CoversBodyAreaFlags & 0x100))
                            facialHairCount += items[i].second;
                        else if (mod->CoversBodyAreaFlags & 0x10)
                            haircutCount += items[i].second;
                    }
                } else if (!tattoo->IsInvalidCustomTattoo()) {
                    tattooCount += items[i].second;
                }
            }
        }

        CViewport* viewport = static_cast<CViewport*>(list->FindChild("PC_DESCRIPTION_VIEWPORT"));

        NUISystem::CComponent* entry = list->FindChild("PC_MENU_ENTRY_CLOTHING");
        NUISystem::CComponent* description = entry->FindChild("PC_CLOTHING_DESC");
        viewport->AddFakeChild(description);
        if (clothing && clothingCount > 0) {
            CText* text = static_cast<CText*>(entry->FindChild("PC_CLOTHING_BUTTON")->FindChild("PC_TEXT_CLOTHING"));
            CWideString label(text->GetValue());
            label += CTCInventoryStats::GetXString();
            label += GFIntToWideString(clothingCount);
            text->SetValue(label, true, true);
            CText* subtitle = static_cast<CText*>(entry->FindChild("PC_CLOTHING_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(label, true, true);
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_CLOTHING_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*entry));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            entry->AddPreAction(action);
        } else {
            entry->MakeUnavailable();
        }

        entry = list->FindChild("PC_MENU_ENTRY_WEAPONS");
        description = entry->FindChild("PC_WEAPON_DESC");
        viewport->AddFakeChild(description);
        if (weapons && weaponsCount > 0) {
            CText* text = static_cast<CText*>(entry->FindChild("PC_WEAPONS_BUTTON")->FindChild("PC_TEXT_WEAPONS"));
            CWideString label(text->GetValue());
            label += CTCInventoryStats::GetXString();
            label += GFIntToWideString(weaponsCount);
            text->SetValue(label, true, true);
            CText* subtitle = static_cast<CText*>(entry->FindChild("PC_WEAPON_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(label, true, true);
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_WEAPONS_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*entry));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            entry->AddPreAction(action);
        } else {
            entry->MakeUnavailable();
        }

        entry = list->FindChild("PC_MENU_ENTRY_ITEMS");
        description = entry->FindChild("PC_ITEMS_DESC");
        viewport->AddFakeChild(description);
        if (inventory && itemsCount > 0) {
            CText* text = static_cast<CText*>(entry->FindChild("PC_ITEMS_BUTTON")->FindChild("PC_TEXT_ITEMS"));
            CWideString label(text->GetValue());
            label += CTCInventoryStats::GetXString();
            label += GFIntToWideString(itemsCount);
            text->SetValue(label, true, true);
            CText* subtitle = static_cast<CText*>(entry->FindChild("PC_ITEMS_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(label, true, true);
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_ITEMS_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*entry));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            entry->AddPreAction(action);
        } else {
            entry->MakeUnavailable();
        }

        entry = list->FindChild("PC_MENU_ENTRY_STYLE_CARDS");
        description = entry->FindChild("PC_STYLE_CARDS_DESC");
        viewport->AddFakeChild(description);
        long styleCount = facialHairCount + tattooCount + haircutCount;
        if (inventory && styleCount > 0) {
            CText* text = static_cast<CText*>(entry->FindChild("PC_STYLE_CARDS_BUTTON")->FindChild("PC_TEXT_STYLE_CARDS"));
            CWideString label(text->GetValue());
            label += CTCInventoryStats::GetXString();
            label += GFIntToWideString(styleCount);
            text->SetValue(label, true, true);
            CText* subtitle = static_cast<CText*>(entry->FindChild("PC_STYLE_CARDS_SUBTITLE"));
            if (subtitle)
                subtitle->SetValue(label, true, true);
            CCountedPointer<CAction> action(new CAction);
            action->ActionType = NUISystem::UI_ACTION_TYPE_CONSTRUCT_PC_STYLE_CARDS_LIST;
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*list));
            action->UIActionParams.push_back(&static_cast<CActionParam&>(*entry));
            action->UIActionParams.push_back(new CActionParamSLONG(haircutCount));
            action->UIActionParams.push_back(new CActionParamSLONG(facialHairCount));
            action->UIActionParams.push_back(new CActionParamSLONG(tattooCount));
            entry->AddPreAction(action);
        } else {
            entry->MakeUnavailable();
        }

        list->Initialise();
        Components.push_back(list);
    }

    NUISystem::CComponent* scroll = list->FindChild("SCROLL_LIST_UP");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_DESCRIPTION_UP");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_LIST_DOWN");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    scroll = list->FindChild("SCROLL_DESCRIPTION_DOWN");
    if (scroll)
        scroll->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    NUISystem::CComponent* clickable = list->FindChild("PC_BACK_CLICKABLE");
    if (clickable) {
        NUISystem::CManager::GetInstance()->AddObserver(clickable);
        clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    clickable = list->FindChild("PC_CLOSE_CLICKABLE");
    if (clickable) {
        NUISystem::CManager::GetInstance()->AddObserver(clickable);
        clickable->ObserveEvent(NUISystem::UI_EVENT_MOUSE_MOVEMENT);
    }
    list->ChangeState(5);
}
