// CTCInventoryMap::ConstructLogbookListPC(NUISystem::CComponent* parent) -- build the PC logbook screen: the category list with its four scroll-button action groups, the frame-positioned titles / ring pictures / descriptions, one sub-list of the hero's books and one per non-empty logbook category (each entry a menu entry with its description and subtitle), attached to (or disabling) the category menu entries. Returns PC_LOGBOOK_CATEGORIES.
// Leaf: ?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006096E0, 12,832 bytes: code to `ret 4` at 0x0060C8DA, `lea ecx,[ecx]`, then the 8-entry jump
//   table 0x0060C8E0 of the category count switch. Speed TU, aligned frame (`and esp,-8 ; sub esp,11Ch`), no EH.
// Identity: Ego_r.pdb + FableWin.pdb ?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z: Ego_r donor
//   order ~CTCInventoryMap 0x00659190 < ConstructLogbookList 0x006595D0 < ConstructLogbookListPC 0x0065BBC0 < CTCInventoryMap()
//   0x0065F660 is retail's ~CTCInventoryMap 0x00606F80 < ConstructLogbookList 0x00607330 < this < 0x0060C900, and this body calls
//   the PC twin GetMenuEntryFromCategoryPC 0x005FAC20 (landed; its callers 0x0060C7BF / 0x0060C809 are here) with the PC_* strings.
//   The catalog's "_global::ConstructLogbookScreen" is descriptive.
// Twins (statement idioms copied): landed console ConstructLogbookList 0x00607330 (the title block under its own
//   `if (!books.empty())` -- retail 0x0060B299 `je 0x0060B9C6` skips it; the menu entry built from GetAbbreviatedName() and the
//   subtitle from GetName()), and the same-TU PC builder ConstructQuestsScreenPC 0x00601310 (opt-in set; the two FRAME scroll
//   groups pass observe* to AddActionOnSelected and ignore* to AddActionOnInactive, the two screen groups the reverse).
// Shape: the position phase reuses one named `child`; the book's starting orientation is a named CRightHandedSet; the final loop
//   passes the loop variable, not the address-taken key copy that find() takes, to GetMenuEntryFromCategoryPC; contentText is
//   declared before AddPositionChild in the log-entry loop.
// FITTED (coordinator decision): the count loop's `if (it != entries.end())` in front of the `for`. c2 folds the test into the
//   loop's own entry test (no bytes); it halves the static frequency of the loop (f 5 -> 2.5), and only then does VC7.1 call the
//   tree iterator's operator++ 0x0047B3E0 at 0x0060B0DB instead of expanding it (inline trace: sz 62 pen 33 c1 89 f 5 score 72,
//   threshold 73; the iterator is address-taken and homed at [esp+24h] in retail because of that call). The same device is landed
//   in the console twin 0x00607330 (books loop guard, `entry && arrow`). No natural spelling found; without the guard the row is
//   DIFFER(12880v12832) with one call missing.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_CONSTRUCT_HELPER
#define FABLE_RETAIL_STLP_LIST_BASE_CONVERT_ALLOCATOR
#define FABLE_RETAIL_STLP_MALLOC_ALLOC_ALLOCATE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CHEROLOGBOOKENTRY_ACCESSORS
#define FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#define FABLE_RETAIL_CTCINVENTORYITEM_STARTINGRHSET
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CANIMFILTER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CEngineGraphic.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
#include "engine/CAnimationEntry.h"
class CRightHandedSet;
#include "engine/CTCInventoryItem.h"
#include "engine/CTCInventoryMap.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CHeroLogBook.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_CActionParamULONG.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMesh.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CText.h"
#include "engine/NUISystem_CViewport.h"

using NUISystem::CComponent;
using NUISystem::CAction;
using NUISystem::CActionParamULONG;
using NUISystem::CList;
using NUISystem::CMesh;
using NUISystem::CSprite;
using NUISystem::CText;
using NUISystem::CViewport;
typedef CHeroLogBook::CHeroLogBookEntry CHeroLogBookEntry;
typedef std::map<CHeroLogBookEntry::ECategory, std::vector<CHeroLogBookEntry> > CLogEntries;

// CategoriesList.clear(): retail expands map::clear and calls the tree's clear, an out-of-line COMDAT 0x006166E0, while the
// entries map's destructor expands the same body inline; declaring that one specialization out of line reproduces both.
namespace std {
template<> void _Rb_tree<CHeroLogBookEntry::ECategory, pair<const CHeroLogBookEntry::ECategory, NUISystem::CComponent*>,
    _Select1st<pair<const CHeroLogBookEntry::ECategory, NUISystem::CComponent*> >, less<CHeroLogBookEntry::ECategory>,
    allocator<pair<const CHeroLogBookEntry::ECategory, NUISystem::CComponent*> > >::clear();
}

NUISystem::CComponent* CTCInventoryMap::ConstructLogbookListPC(NUISystem::CComponent* parent)
{
    NUISystem::CManager* manager = NUISystem::CManager::GetInstance();
    CLogEntries entries = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPlayerHeroLogBook().GetLogEntries();
    CComponent* list = manager->CreateComponent("PC_LOGBOOK_CATEGORIES", false);
    CComponent* frame = list->FindChild("PC_UI_FRAME_LOGBOOK");

    // the list scrolls with the scroll-list buttons of the screen and of the frame: it ignores their mouse movement while
    // selected and observes it while inactive
    {
        CComponent* up = parent->FindChild("SCROLL_LIST_UP");
        CComponent* down = parent->FindChild("SCROLL_LIST_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreUp);
        list->AddActionOnInactive(observeUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreDown);
        list->AddActionOnInactive(observeDown);
    }
    {
        CComponent* up = frame->FindChild("SCROLL_LIST_UP");
        CComponent* down = frame->FindChild("SCROLL_LIST_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeUp);
        list->AddActionOnInactive(ignoreUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeDown);
        list->AddActionOnInactive(ignoreDown);
    }
    // the same for the description scroll buttons
    {
        CComponent* up = parent->FindChild("SCROLL_DESCRIPTION_UP");
        CComponent* down = parent->FindChild("SCROLL_DESCRIPTION_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreUp);
        list->AddActionOnInactive(observeUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(ignoreDown);
        list->AddActionOnInactive(observeDown);
    }
    {
        CComponent* up = frame->FindChild("SCROLL_DESCRIPTION_UP");
        CComponent* down = frame->FindChild("SCROLL_DESCRIPTION_DOWN");
        CCountedPointer<CAction> observeUp(new CAction);
        observeUp->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeUp->AddParam(up, observeUp);
        observeUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreUp(new CAction);
        ignoreUp->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreUp->AddParam(up, ignoreUp);
        ignoreUp->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeUp);
        list->AddActionOnInactive(ignoreUp);
        CCountedPointer<CAction> observeDown(new CAction);
        observeDown->ActionType = NUISystem::UI_ACTION_TYPE_OBSERVE_EVENT;
        observeDown->AddParam(down, observeDown);
        observeDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        CCountedPointer<CAction> ignoreDown(new CAction);
        ignoreDown->ActionType = NUISystem::UI_ACTION_TYPE_IGNORE_EVENT;
        ignoreDown->AddParam(down, ignoreDown);
        ignoreDown->UIActionParams.push_back(new CActionParamULONG(NUISystem::UI_EVENT_MOUSE_MOVEMENT));
        list->AddActionOnSelected(observeDown);
        list->AddActionOnInactive(ignoreDown);
    }

    // the titles and the per-category ring pictures, subtitles and descriptions are positioned by the frame
    CComponent* child = list->FindChild("PC_LOGBOOK_CATEGORIES_TITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_BASICS");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_BASICS_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_BASICS_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_OBJECTS");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_OBJECTS_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_OBJECTS_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_TOWNS");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_TOWNS_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_TOWNS_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_HERO");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_HERO_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_HERO_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_COMBAT");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_COMBAT_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_COMBAT_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_QUESTS");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_QUESTS_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_QUESTS_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_STORY");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_STORY_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_STORY_DESC");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_RING_PIC_LOGBOOK_BOOKS");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_BOOKS_SUBTITLE");
    frame->AddPositionChild(child);
    child = list->FindChild("PC_LOGBOOK_BOOKS_DESC");
    frame->AddPositionChild(child);

    // the category descriptions scroll in the description viewport
    CComponent* viewport = parent->FindChild("PC_DESCRIPTION_VIEWPORT");
    if (viewport) {
        CComponent* description = list->FindChild("PC_LOGBOOK_BASICS_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_OBJECTS_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_TOWNS_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_HERO_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_COMBAT_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_QUESTS_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_STORY_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
        description = list->FindChild("PC_LOGBOOK_BOOKS_DESC");
        if (description)
            static_cast<CViewport*>(viewport)->AddFakeChild(description);
    }

    // count the hero's log entries per category
    CategoriesList.clear();
    unsigned long basics = 0;
    unsigned long objects = 0;
    unsigned long towns = 0;
    unsigned long hero = 0;
    unsigned long combat = 0;
    unsigned long quests = 0;
    unsigned long story = 0;
    CLogEntries::iterator it = entries.begin();
    if (it != entries.end())
    for (; it != entries.end(); ++it) {
        switch (it->first) {
        case CHeroLogBookEntry::CATEGORY_BASICS:
            basics = it->second.size();
            break;
        case CHeroLogBookEntry::CATEGORY_OBJECTS:
            objects = it->second.size();
            break;
        case CHeroLogBookEntry::CATEGORY_TOWNS:
            towns = it->second.size();
            break;
        case CHeroLogBookEntry::CATEGORY_HERO:
            hero = it->second.size();
            break;
        case CHeroLogBookEntry::CATEGORY_COMBAT:
            combat = it->second.size();
            break;
        case CHeroLogBookEntry::CATEGORY_QUEST:
            quests = it->second.size();
            break;
        case CHeroLogBookEntry::CATEGORY_STORY:
            story = it->second.size();
            break;
        }
    }

    // the books the hero carries
    std::vector<long> books;
    NInventory::CTCInventoryBase* inventory;
    if (GetParentThing().GetTC(TCI_INVENTORY, &inventory)) {
        std::vector<std::pair<long, long> > items;
        inventory->GetAllItems(items);
        for (std::vector<std::pair<long, long> >::iterator item = items.begin(); item != items.end(); ++item)
            if (CTCInventoryItem::IsBook(item->first))
                books.push_back(item->first);
    }
    if (books.empty()) {
        CComponent* booksEntry = list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS");
        if (booksEntry)
            booksEntry->MakeUnavailable();
    }
    CCountedPointer<CComponent> booksList(NUISystem::CManager::GetInstance()->CreateComponent("PC_SUB_LIST", false));
    CList* booksSubList = static_cast<CList*>(booksList.Get());
    booksSubList->InitialiseOffsets(books.size());
    for (std::vector<long>::iterator book = books.begin(); book != books.end(); ++book) {
        long index = *book;
        CWideString description = CTCInventoryItem::GetDescription(index);
        CCountedPointer<CComponent> entry;
        entry.Reset(manager->CreateMenuEntryComponent(description, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                      "PC_BUTTON_BASTARD"));
        // the book's model, turned to its starting orientation and playing its default animation
        CCountedPointer<CComponent> model(manager->CreateComponent("PC_MODEL", false));
        CCountedPointer<CMesh> mesh;
        frame->AddPositionChild(model.Get());
        mesh.SetStaticCast(model);
        CDefPointer<const CThingObjectDef> def;
        GetDefinitionManager().CDefinitionManager::GetDef(index, &def);
        mesh->SetMeshIndex(def->Graphic.BankIndex);
        CRightHandedSet orientation = CTCInventoryItem::GetStartingRightHandedSet(index);
        mesh->SetOrientation(orientation);
        if (def->Graphic.Type == 4) {
            CDefPointer<const CAppearanceDef> appearance;
            def->GetSubDef(&appearance);
            mesh->SetAnimationIndex(appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup()).BankIndex);
            mesh->SetMeshType((EEngineGraphicType)4);
        }
        entry->AddChild(model);
        // its details text, scrolled in the description viewport
        CCountedPointer<CComponent> details(manager->CreateComponent("PC_DESCRIPTION", false));
        CCountedPointer<CText> detailsText;
        detailsText.SetStaticCast(details);
        frame->AddPositionChild(details.Get());
        detailsText->SetValue(CTCInventoryItem::GetDetails(index), true, true);
        entry->AddChild(details);
        if (viewport)
            static_cast<CViewport*>(viewport)->AddFakeChild(details.Get());
        // its name as the subtitle, with the book's icon when it has one
        if (CTCInventoryItem::GetEngineGraphic(index).BankIndex != 0) {
            CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
            frame->AddPositionChild(subtitle.Get());
            CCountedPointer<CText> subtitleText;
            subtitleText.SetStaticCast(subtitle);
            subtitleText->SetValue(description, true, true);
            entry->AddChild(subtitle);
            CCountedPointer<CComponent> icon(manager->CreateComponent("PC_ICON_INDEPENDANT", false));
            frame->AddPositionChild(icon.Get());
            if (icon->GetType() == NUISystem::UI_TYPE_SPRITE)
                static_cast<CSprite*>(icon.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(index).BankIndex);
            entry->AddChild(icon);
        } else {
            CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
            frame->AddPositionChild(subtitle.Get());
            CCountedPointer<CText> subtitleText;
            subtitleText.SetStaticCast(subtitle);
            subtitleText->SetValue(description, true, true);
            entry->AddChild(subtitle);
        }
        booksSubList->AddScrollingChildAndSetOffsets(entry);
    }
    if (!books.empty()) {
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_BOOKS_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        booksSubList->AddNonScrollingChild(title);
        CComponent* booksEntry = list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS");
        if (booksEntry)
            booksEntry->AddChild(booksList);
    }

    // one sub-list per non-empty category, with its title and ring picture
    if (basics) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(basics);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_BASICS_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_BASICS_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_BASICS] = subList;
    }
    if (objects) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(objects);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_OBJECTS_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_OBJECTS_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_OBJECTS] = subList;
    }
    if (towns) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(towns);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_TOWNS_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_TOWNS_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_TOWNS] = subList;
    }
    if (hero) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(hero);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_HERO_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_HERO_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_HERO] = subList;
    }
    if (combat) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(combat);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_COMBAT_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_COMBAT_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_COMBAT] = subList;
    }
    if (quests) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(quests);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_QUESTS_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_QUESTS_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_QUEST] = subList;
    }
    if (story) {
        CList* subList = static_cast<CList*>(manager->CreateComponent("PC_SUB_LIST", false));
        subList->InitialiseOffsets(story);
        CCountedPointer<CComponent> title(manager->CreateComponent("PC_STORY_CATEGORIES_TITLE", false));
        frame->AddPositionChild(title.Get());
        subList->AddNonScrollingChild(title);
        CCountedPointer<CComponent> ring(manager->CreateComponent("PC_RING_PIC_LOGBOOK_STORY_FOR_LIST", false));
        frame->AddPositionChild(ring.Get());
        subList->AddNonScrollingChild(ring);
        CategoriesList[CHeroLogBookEntry::CATEGORY_STORY] = subList;
    }

    // one menu entry per log entry in its category's sub-list: the name, the content in the description viewport, the
    // abbreviated name as the subtitle
    for (CLogEntries::iterator it = entries.begin(); it != entries.end(); ++it) {
        CList* subList = static_cast<CList*>(CategoriesList[it->first]);
        subList->SetDefaultChild(it->second.size() - 1);
        for (unsigned long i = 0; i < it->second.size(); ++i) {
            CHeroLogBookEntry logEntry = it->second[i];
            CWideString abbreviatedName = logEntry.GetAbbreviatedName();
            CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(abbreviatedName, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "",
                                                                              0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> content(manager->CreateComponent("PC_DESCRIPTION", false));
            CCountedPointer<CText> contentText;
            frame->AddPositionChild(content.Get());
            if (viewport)
                static_cast<CViewport*>(viewport)->AddFakeChild(content.Get());
            contentText.SetStaticCast(content);
            contentText->SetValue(logEntry.GetContent(), true, true);
            entry->AddChild(content);
            CCountedPointer<CComponent> subtitle(manager->CreateComponent("PC_SUBTITLE_INDEPENDANT", false));
            frame->AddPositionChild(subtitle.Get());
            CCountedPointer<CText> subtitleText;
            subtitleText.SetStaticCast(subtitle);
            subtitleText->SetValue(logEntry.GetName(), true, true);
            entry->AddChild(subtitle);
            subList->AddScrollingChildAndSetOffsets(entry);
        }
    }

    // hang each category's sub-list under its menu entry; a category with no entries is unavailable
    for (long category = CHeroLogBookEntry::CATEGORY_QUEST; category <= CHeroLogBookEntry::CATEGORY_COMBAT; ++category) {
        const CHeroLogBookEntry::ECategory key = (CHeroLogBookEntry::ECategory)category;
        if (key == CHeroLogBookEntry::CATEGORY_TUTORIAL)
            continue;
        std::map<CHeroLogBookEntry::ECategory, CComponent*>::iterator found = CategoriesList.find(key);
        if (found != CategoriesList.end()) {
            CCountedPointer<CComponent> subList(found->second);
            GetMenuEntryFromCategoryPC((CHeroLogBookEntry::ECategory)category, list)->AddChild(subList);
        } else
            GetMenuEntryFromCategoryPC((CHeroLogBookEntry::ECategory)category, list)->MakeUnavailable();
    }
    return list;
}
