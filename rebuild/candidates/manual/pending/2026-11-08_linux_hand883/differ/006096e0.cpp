// CTCInventoryMap::ConstructLogbookListPC(NUISystem::CComponent* parent) -- build the PC logbook screen: the category list with its scroll actions, one sub-list of the hero's books and one per non-empty logbook category, each entry a menu entry with its description in the description viewport
// Grade: DIFFER(12832v12832) -- call sequence and size identical, 17 of 591 call segments differ in register / const-0 choices: retail `xor ebx,ebx` before the CategoriesList clear call and the seven counter zero stores through ebx (ours: zero in eax after the call, the entries header first in ebx; c2prio: the counters' initial pieces take eax because ebx costs 600 there), the book icon branch (retail subtitleText.Info in ebx, manager reloaded; ours a 2-ref manager piece in ebx, prio 54 > Info 34), a zero ebx kept across the books-loop exit (`push ebx`), one `cmp [eax],ecx` in the COMBAT block; frame 0x124 vs 0x11C; alignment pads follow
// Next: find the source form that changes the counters' const-0 pieces in the count prologue (the first differing byte, 0x0060AFA1) -- c2prio --trace on block 295 (`costs: {'ebx': 600}` = a neighbour left with one register); then the icon-branch manager piece
// Inline-budget: /d2inlT71 -- plain C++ inlines the count loop's `++it` (retail calls 0x0047B3E0); T70 also outlines the CCountedPointer<CAction>(T*) of groups 1-3 (591 calls), T71 = T72 gives retail's 590 calls; /d2inlS<N> has no effect (S1..S8 identical)
// Leaf: ?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
// Flags: /d2inlT71
// Retail: Fable.exe (TLC, VC7.1) @ 0x006096E0, 12,832 bytes: code to `ret 4` at 0x0060C8DA, `lea ecx,[ecx]`, then the 8-entry jump
//   table 0x0060C8E0 of the category count switch. Speed TU, aligned frame (`and esp,-8 ; sub esp,11Ch`), no EH.
// Identity: Ego_r.pdb + FableWin.pdb ?ConstructLogbookListPC@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z: Ego_r donor
//   order ~CTCInventoryMap 0x00659190 < ConstructLogbookList 0x006595D0 < ConstructLogbookListPC 0x0065BBC0 < CTCInventoryMap()
//   0x0065F660 is retail's ~CTCInventoryMap 0x00606F80 < ConstructLogbookList 0x00607330 < this < 0x0060C900, and this body calls
//   the PC twin GetMenuEntryFromCategoryPC 0x005FAC20 (landed; its callers 0x0060C7BF / 0x0060C809 are here) with the PC_* strings.
//   The catalog's "_global::ConstructLogbookScreen" is descriptive.
// Shape: the position phase reuses one named `child` (the nested `frame->AddPositionChild(list->FindChild(..))` loads frame's
//   vptr before the FindChild call and loses list its edi); the book's starting orientation is a named CRightHandedSet (a
//   __fastcall hidden-pointer result in a named local); the final loop passes the loop variable, not the address-taken key copy
//   that find() takes, to GetMenuEntryFromCategoryPC (category in esi); contentText is declared before AddPositionChild in the
//   log-entry loop. Header opt-ins (hdr/README.md): STLport list `_Construct` + alloc-proxy rebind (the group-4 inline budget),
//   CEngineGraphic's PDB ctors (hidden result pointer in ecx), the GetDef<T> / GetSubDef<T> template bodies (retail's
//   out-of-line COMDATs 0x0042B040 / 0x0042B0A2; visible, they let VC7.1 drop the def handle's null test and dead store).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_BODY
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
#define FABLE_RETAIL_STLP_LIST_CREATE_CONSTRUCT
#define FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_COPY
#define FABLE_RETAIL_STLP_TREE_FIND_OUTOFLINE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CACTION_INLINE_CTOR
#define FABLE_RETAIL_CACTIONPARAMULONG
#define FABLE_RETAIL_CHEROLOGBOOK_LOGBOOKSCREEN
#define FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_UIDETAILS
#define FABLE_RETAIL_CTHINGOBJECTDEF_GRAPHIC
#define FABLE_RETAIL_CANIMFILTER
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CEngineGraphic.h"
#include "engine/CRightHandedSet.h"
#include "engine/CThing.h"
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
    CLogEntries entries = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPlayerHeroLogBook().LogEntries;
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
    for (CLogEntries::iterator it = entries.begin(); it != entries.end(); ++it) {
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
            if (icon->GetType() == NUISystem::UI_TYPE_SPRITE) {
                CSprite* sprite = static_cast<CSprite*>(icon.Get());
                sprite->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(index).BankIndex);
            }
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
    {
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
    for (CLogEntries::iterator it = entries.begin(); it != entries.end(); it++) {
        CList* subList = static_cast<CList*>(CategoriesList[it->first]);
        subList->SetDefaultChild(it->second.size() - 1);
        for (unsigned long i = 0; i < it->second.size(); ++i) {
            CHeroLogBookEntry logEntry = it->second[i];
            CWideString name = logEntry.GetName();
            CCountedPointer<CComponent> entry(manager->CreateMenuEntryComponent(name, 0.0f, 1.0f, "PC_MENU_ENTRY_TEMPLATE", 0.0f, "",
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
            subtitleText->SetValue(logEntry.GetAbbreviatedName(), true, true);
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
