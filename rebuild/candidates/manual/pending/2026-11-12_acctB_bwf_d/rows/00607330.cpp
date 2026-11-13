// CTCInventoryMap::ConstructLogbookList(NUISystem::CComponent* parent) -- builds the console logbook screen: copies the hero's
// logbook entries, hooks the eight category descriptions into the text viewport, counts the entries per category, collects the
// books in the hero's inventory (disabling the BOOKS menu entry when there are none, else building its list of book models with
// details/name/sprite children), builds one sub-list per non-empty category with every entry's menu entry, and attaches the lists
// to (or disables) the category menu entries. Returns the UI_LOGBOOK_CATEGORIES component.
// Retail 0x00607330, 9124 bytes (0x23A4). Callees: CManager::GetInstance 0x0041E5F2, CManager::CreateComponent 0x0041DB1D,
// CManager::CreateMenuEntryComponent 0x00428494, CPlayerManager::GetMainPlayer 0x00449960, map<ECategory,vector<CHeroLogBookEntry>>
// copy ctor 0x00619C50 / _M_erase 0x0047C6A0, CategoriesList _M_erase 0x00613AE0 / operator[] 0x00618BE0, CVectorMap::LowerBound
// 0x0040F020, CTCInventoryBase::GetAllItems 0x005BF482, CTCInventoryItem::IsBook 0x005D9970 / GetDescription 0x005D95C0 /
// GetDetails 0x005D9630 / GetEngineGraphic 0x005D96A0 / GetStartingRightHandedSet 0x005D9730, vector<long>::_M_insert_overflow
// 0x00440990, list<CComponent*>::~list 0x0042ABCA / _M_insert_dispatch 0x0042B4C3, CTCBase::GetDefinitionManager 0x006869D0,
// CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0, CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CAnimationSet::GetAnim
// 0x005DC340, CTCInventoryMap::GetMenuEntryFromCategory 0x005FAA90, CCharString / CWideString ctors and dtors, operator new/delete,
// malloc/free.
// Shape notes: GetDef<T>(long, ...) is the inline CDefinitionManager template (expanded in retail); GetSubDef<T>'s body is visible
// but kept out of line (retail calls 0x0042B0A2); the books title/back block is its own `if (!books.empty())` after the book loop
// (scope-based slot packing gives retail's 0x10C frame); the eight description lookups are eight named locals.
// Leaf: ?ConstructLogbookList@CTCInventoryMap@@QAEPAVCComponent@NUISystem@@PAV23@@Z
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_INSERT_DISPATCH_NOINLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYMAP_LOGBOOK
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS
#define FABLE_RETAIL_CTCINVENTORYITEM_DETAILS
#define FABLE_RETAIL_CTCINVENTORYITEM_STATICENGINEGRAPHIC
#define FABLE_RETAIL_CTCINVENTORYITEM_STARTINGRHSET
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CTEXT
#define FABLE_RETAIL_CTCINVENTORYBASE_BUILDINGTHING
#define FABLE_RETAIL_CHEROLOGBOOKENTRY_ACCESSORS
#define FABLE_RETAIL_CENGINEGRAPHIC_CTOR
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventoryMap.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CHeroLogBook.h"
#include "engine/CCountedPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/CAppearanceDef.h"
#include "engine/CAnimationSet.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CRightHandedSet.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CViewport.h"
#include "engine/NUISystem_CSprite.h"
#include "engine/NUISystem_CMesh.h"
#include "engine/CText.h"
#include "engine/CDefClassBase_GetSubDef.h"
using namespace NUISystem;

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

typedef CHeroLogBook::CHeroLogBookEntry CHeroLogBookEntry;

CComponent* CTCInventoryMap::ConstructLogbookList(CComponent* parent)
{
    CManager* manager = CManager::GetInstance();
    std::map<CHeroLogBookEntry::ECategory, std::vector<CHeroLogBookEntry> > log_entries =
        CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPlayerHeroLogBook().GetLogEntries();
    CComponent* categories = manager->CreateComponent("UI_LOGBOOK_CATEGORIES", false);
    CComponent* viewport = categories->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    if (viewport) {
        CComponent* basics_desc = categories->FindChild("UI_LOGBOOK_BASICS_DESC");
        if (basics_desc) static_cast<CViewport*>(viewport)->AddFakeChild(basics_desc);
        CComponent* objects_desc = categories->FindChild("UI_LOGBOOK_OBJECTS_DESC");
        if (objects_desc) static_cast<CViewport*>(viewport)->AddFakeChild(objects_desc);
        CComponent* towns_desc = categories->FindChild("UI_LOGBOOK_TOWNS_DESC");
        if (towns_desc) static_cast<CViewport*>(viewport)->AddFakeChild(towns_desc);
        CComponent* hero_desc = categories->FindChild("UI_LOGBOOK_HERO_DESC");
        if (hero_desc) static_cast<CViewport*>(viewport)->AddFakeChild(hero_desc);
        CComponent* combat_desc = categories->FindChild("UI_LOGBOOK_COMBAT_DESC");
        if (combat_desc) static_cast<CViewport*>(viewport)->AddFakeChild(combat_desc);
        CComponent* quests_desc = categories->FindChild("UI_LOGBOOK_QUESTS_DESC");
        if (quests_desc) static_cast<CViewport*>(viewport)->AddFakeChild(quests_desc);
        CComponent* story_desc = categories->FindChild("UI_LOGBOOK_STORY_DESC");
        if (story_desc) static_cast<CViewport*>(viewport)->AddFakeChild(story_desc);
        CComponent* books_desc = categories->FindChild("UI_LOGBOOK_BOOKS_DESC");
        if (books_desc) static_cast<CViewport*>(viewport)->AddFakeChild(books_desc);
    }
    CategoriesList.clear();

    long num_basics = 0, num_objects = 0, num_towns = 0, num_hero = 0, num_combat = 0, num_quests = 0, num_story = 0;
    std::map<CHeroLogBookEntry::ECategory, std::vector<CHeroLogBookEntry> >::iterator it;
    for (it = log_entries.begin(); it != log_entries.end(); ++it) {
        switch (it->first) {
        case CHeroLogBookEntry::CATEGORY_BASICS:  num_basics = it->second.size(); break;
        case CHeroLogBookEntry::CATEGORY_OBJECTS: num_objects = it->second.size(); break;
        case CHeroLogBookEntry::CATEGORY_TOWNS:   num_towns = it->second.size(); break;
        case CHeroLogBookEntry::CATEGORY_HERO:    num_hero = it->second.size(); break;
        case CHeroLogBookEntry::CATEGORY_COMBAT:  num_combat = it->second.size(); break;
        case CHeroLogBookEntry::CATEGORY_QUEST:   num_quests = it->second.size(); break;
        case CHeroLogBookEntry::CATEGORY_STORY:   num_story = it->second.size(); break;
        }
    }

    std::vector<long> books;
    NInventory::CTCInventoryBase* inventory;
    if (ParentThing->GetTC(TCI_INVENTORY, &inventory)) {
        std::vector<std::pair<long, long> > items;
        inventory->GetAllItems(items);
        for (std::vector<std::pair<long, long> >::iterator item = items.begin(); item != items.end(); ++item)
            if (CTCInventoryItem::IsBook(item->first))
                books.push_back(item->first);
    }
    if (books.empty()) {
        CComponent* entry = categories->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS");
        if (entry) {
            entry->MakeUnavailable();
            CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (entry && arrow) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }

    CCountedPointer<CComponent> books_list(CManager::GetInstance()->CreateComponent("UI_SUB_LIST", false));
    CList* books_as_list = static_cast<CList*>(books_list.Get());
    books_as_list->InitialiseOffsets(books.size());
    std::vector<long>::iterator book_it = books.begin();
    if (!books.empty()) {
        for (; book_it != books.end(); ++book_it) {
            long book = *book_it;
            CWideString description = CTCInventoryItem::GetDescription(book);
            CCountedPointer<CComponent> entry;
            entry.Reset(manager->CreateMenuEntryComponent(description, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE", 0.0f, "", 0.0f,
                                                          "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> component(manager->CreateComponent("UI_ITEM_MODEL_TEMPLATE_NEW", false));
            CCountedPointer<CMesh> mesh;
            mesh.SetStaticCast(component);
            CDefPointer<const CThingObjectDef> def;
            GetDefinitionManager().GetDef(book, &def);
            mesh->SetMeshIndex(def->Graphic.BankIndex);
            CRightHandedSet orientation = CTCInventoryItem::GetStartingRightHandedSet(book);
            mesh->SetOrientation(orientation);
            if (def->Graphic.Type == 4) {
                CDefPointer<const CAppearanceDef> appearance;
                def->GetSubDef(&appearance);
                mesh->SetAnimationIndex(appearance->Animation.GetAnim("DEFAULT", 0, CAnimFilterNoGroup()).BankIndex);
                mesh->SetMeshType((EEngineGraphicType)4);
            }
            entry->AddChild(component);
            CCountedPointer<CComponent> details(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
            CCountedPointer<NUISystem::CText> details_text;
            details_text.SetStaticCast(details);
            details_text->SetValue(CTCInventoryItem::GetDetails(book), true, true);
            entry->AddChild(details);
            CComponent* text_viewport = parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
            if (text_viewport)
                static_cast<CViewport*>(text_viewport)->AddFakeChild(details.Get());
            if (CTCInventoryItem::GetEngineGraphic(book).BankIndex != 0) {
                CCountedPointer<CComponent> name(manager->CreateComponent("UI_TEXT_ITEM_NAME_TEMPLATE_NEW", false));
                CCountedPointer<NUISystem::CText> name_text;
                name_text.SetStaticCast(name);
                name_text->SetValue(description, true, true);
                entry->AddChild(name);
                CCountedPointer<CComponent> sprite(manager->CreateComponent("UI_ITEM_SPRITE_TEMPLATE", false));
                if (sprite->GetType() == UI_TYPE_SPRITE)
                    static_cast<CSprite*>(sprite.Get())->SetGraphicIndex(CTCInventoryItem::GetEngineGraphic(book).BankIndex);
                entry->AddChild(sprite);
            } else {
                CCountedPointer<CComponent> name(manager->CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW", false));
                CCountedPointer<NUISystem::CText> name_text;
                name_text.SetStaticCast(name);
                name_text->SetValue(description, true, true);
                entry->AddChild(name);
            }
            books_as_list->AddScrollingChildAndSetOffsets(entry);
        }
    }
    if (!books.empty()) {
        CCountedPointer<CComponent> title(manager->CreateComponent("UI_BOOKS_CATEGORIES_TITLE", false));
        books_as_list->AddNonScrollingChild(title);
        CCountedPointer<CComponent> back(manager->CreateComponent("UI_BACK_FOR_LIST", false));
        books_as_list->AddNonScrollingChild(back);
        CComponent* books_entry = categories->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS");
        if (books_entry)
            books_entry->AddChild(books_list);
    }

#define LOGBOOK_CATEGORY_LIST(count, category, title_name, ring_name)                                              \
    if (count) {                                                                                                     \
        CList* list = static_cast<CList*>(manager->CreateComponent("UI_SUB_LIST", false));                          \
        list->InitialiseOffsets(count);                                                                              \
        CCountedPointer<CComponent> title(manager->CreateComponent(title_name, false));                             \
        list->AddNonScrollingChild(title);                                                                           \
        CCountedPointer<CComponent> back(manager->CreateComponent("UI_BACK_FOR_LIST", false));                      \
        list->AddNonScrollingChild(back);                                                                            \
        CCountedPointer<CComponent> ring(manager->CreateComponent(ring_name, false));                               \
        list->AddNonScrollingChild(ring);                                                                            \
        CategoriesList[category] = list;                                                                             \
    }
    LOGBOOK_CATEGORY_LIST(num_basics, CHeroLogBookEntry::CATEGORY_BASICS, "UI_BASICS_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_BASICS_FOR_LIST")
    LOGBOOK_CATEGORY_LIST(num_objects, CHeroLogBookEntry::CATEGORY_OBJECTS, "UI_OBJECTS_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_OBJECTS_FOR_LIST")
    LOGBOOK_CATEGORY_LIST(num_towns, CHeroLogBookEntry::CATEGORY_TOWNS, "UI_TOWNS_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_TOWNS_FOR_LIST")
    LOGBOOK_CATEGORY_LIST(num_hero, CHeroLogBookEntry::CATEGORY_HERO, "UI_HERO_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_HERO_FOR_LIST")
    LOGBOOK_CATEGORY_LIST(num_combat, CHeroLogBookEntry::CATEGORY_COMBAT, "UI_COMBAT_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_COMBAT_FOR_LIST")
    LOGBOOK_CATEGORY_LIST(num_quests, CHeroLogBookEntry::CATEGORY_QUEST, "UI_QUESTS_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_QUESTS_FOR_LIST")
    LOGBOOK_CATEGORY_LIST(num_story, CHeroLogBookEntry::CATEGORY_STORY, "UI_STORY_CATEGORIES_TITLE", "UI_RING_PIC_LOGBOOK_STORY_FOR_LIST")
#undef LOGBOOK_CATEGORY_LIST

    for (it = log_entries.begin(); it != log_entries.end(); ++it) {
        CList* list = static_cast<CList*>(CategoriesList[it->first]);
        list->SetDefaultChild((long)it->second.size() - 1);
        for (unsigned long i = 0; i < it->second.size(); ++i) {
            CHeroLogBookEntry entry = it->second[i];
            CWideString abbreviated_name = entry.GetAbbreviatedName();
            CCountedPointer<CComponent> menu_entry(manager->CreateMenuEntryComponent(abbreviated_name, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW",
                                                                                     0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CComponent> content(manager->CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW", false));
            CCountedPointer<NUISystem::CText> content_text;
            static_cast<CViewport*>(viewport)->AddFakeChild(content.Get());
            content_text.SetStaticCast(content);
            content_text->SetValue(entry.GetContent(), true, true);
            menu_entry->AddChild(content);
            CCountedPointer<CComponent> title(manager->CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW", false));
            CCountedPointer<NUISystem::CText> title_text;
            title_text.SetStaticCast(title);
            title_text->SetValue(entry.GetName(), true, true);
            menu_entry->AddChild(title);
            list->AddScrollingChildAndSetOffsets(menu_entry);
        }
    }

    for (long category = CHeroLogBookEntry::CATEGORY_QUEST; category <= CHeroLogBookEntry::CATEGORY_COMBAT; ++category) {
        if (category == CHeroLogBookEntry::CATEGORY_TUTORIAL)
            continue;
        std::map<CHeroLogBookEntry::ECategory, CComponent*>::iterator found = CategoriesList.find((CHeroLogBookEntry::ECategory)category);
        if (found != CategoriesList.end()) {
            CCountedPointer<CComponent> list(found->second);
            GetMenuEntryFromCategory((CHeroLogBookEntry::ECategory)category, categories)->AddChild(list);
        } else {
            CComponent* entry = GetMenuEntryFromCategory((CHeroLogBookEntry::ECategory)category, categories);
            entry->MakeUnavailable();
            CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) {
                CDeletion deletion;
                deletion.Method = CDeletion::UI_DELETE_THIS_INSTANCE;
                arrow->SetDeletion(deletion);
            }
        }
    }
    return categories;
}
