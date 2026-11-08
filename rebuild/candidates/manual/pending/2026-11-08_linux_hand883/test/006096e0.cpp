// Behaviour test for CTCInventoryMap::ConstructLogbookListPC (0x006096E0): the same control flow as the source over a small
// model of the NUISystem component tree (named components with children, scroll actions, position / fake children, sub-lists)
// and of the hero's logbook and inventory, checked on a logbook with entries and books and on an empty one.
#include <cstdio>
#include <map>
#include <string>
#include <vector>

enum ECategory { CATEGORY_QUEST, CATEGORY_STORY, CATEGORY_TUTORIAL, CATEGORY_BASICS, CATEGORY_OBJECTS, CATEGORY_TOWNS,
                 CATEGORY_HERO, CATEGORY_COMBAT };
enum { UI_TYPE_GENERIC = 0, UI_TYPE_SPRITE = 3 };
enum { UI_EVENT_MOUSE_MOVEMENT = 25 };
enum { UI_ACTION_TYPE_OBSERVE_EVENT = 0xF5, UI_ACTION_TYPE_IGNORE_EVENT = 0xF9 };

static int g_fail = 0;
static void check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++g_fail; } }

struct Component;
struct Action { long Type; Component* Target; unsigned long Event; };

struct Component {
    std::string Name;
    int Type;
    std::vector<Component*> Children, PositionChildren, FakeChildren, Scrolling, NonScrolling;
    std::vector<Action> OnSelected, OnInactive;
    bool Unavailable;
    unsigned long Offsets, DefaultChild, MeshIndex, AnimationIndex, GraphicIndex;
    long MeshType, Orientation;
    std::wstring Value;
    Component(const std::string& name, int type) : Name(name), Type(type), Unavailable(false), Offsets(0),
        DefaultChild(0xFFFFFFFF), MeshIndex(0), AnimationIndex(0), GraphicIndex(0), MeshType(0), Orientation(0) {}
    Component* Add(Component* c) { Children.push_back(c); return c; }
    Component* FindChild(const std::string& name)
    {
        for (size_t i = 0; i < Children.size(); ++i) {
            if (Children[i]->Name == name) return Children[i];
            Component* found = Children[i]->FindChild(name);
            if (found) return found;
        }
        return 0;
    }
};

static std::vector<Component*> g_all;
static Component* New(const std::string& name, int type) { Component* c = new Component(name, type); g_all.push_back(c); return c; }
static const char* const g_cats[7] = { "BASICS", "OBJECTS", "TOWNS", "HERO", "COMBAT", "QUESTS", "STORY" };

struct Manager {
    Component* CreateComponent(const std::string& name)
    {
        Component* c = New(name, name == "PC_ICON_INDEPENDANT" ? UI_TYPE_SPRITE : UI_TYPE_GENERIC);
        if (name == "PC_LOGBOOK_CATEGORIES") {               // the list template: frame, titles, rings, descriptions, entries
            Component* frame = c->Add(New("PC_UI_FRAME_LOGBOOK", UI_TYPE_GENERIC));
            frame->Add(New("SCROLL_LIST_UP", 0)); frame->Add(New("SCROLL_LIST_DOWN", 0));
            frame->Add(New("SCROLL_DESCRIPTION_UP", 0)); frame->Add(New("SCROLL_DESCRIPTION_DOWN", 0));
            c->Add(New("PC_LOGBOOK_CATEGORIES_TITLE", 0));
            for (int i = 0; i < 8; ++i) {
                std::string cat = i < 7 ? g_cats[i] : "BOOKS";
                c->Add(New("PC_RING_PIC_LOGBOOK_" + cat, 0)); c->Add(New("PC_LOGBOOK_" + cat + "_SUBTITLE", 0));
                c->Add(New("PC_LOGBOOK_" + cat + "_DESC", 0)); c->Add(New("PC_MENU_ENTRY_LOGBOOK_" + cat, 0));
            }
        }
        return c;
    }
    Component* CreateMenuEntryComponent(const std::wstring& text) { Component* c = New("PC_MENU_ENTRY_TEMPLATE", 0); c->Value = text; return c; }
};

struct LogEntry {
    std::wstring Name, AbbreviatedName, Content;
    std::wstring GetName() const { return Name; }
    std::wstring GetAbbreviatedName() const { return AbbreviatedName; }
    std::wstring GetContent() const { return Content; }
};
typedef std::map<ECategory, std::vector<LogEntry> > LogEntries;

// inventory item data (CTCInventoryItem statics and the item's thing object def)
static bool IsBook(long index) { return index >= 100; }
static std::wstring Number(long n) { std::wstring s; do { s.insert(s.begin(), (wchar_t)(L'0' + n % 10)); n /= 10; } while (n); return s; }
static std::wstring GetDescription(long index) { return L"Book" + Number(index); }
static std::wstring GetDetails(long index) { return L"Text" + Number(index); }
static unsigned long GetEngineGraphicBank(long index) { return (index & 1) ? (unsigned long)index * 10 : 0; }
static long GetStartingRightHandedSet(long index) { return index + 1000; }
static unsigned long DefMeshIndex(long index) { return (unsigned long)index * 3; }
static bool DefIsAnimated(long index) { return index % 3 == 0; }       // Graphic.Type == 4
static unsigned long DefaultAnim(long index) { return (unsigned long)index * 7; }

static Component* GetMenuEntryFromCategoryPC(ECategory category, Component* list)
{
    static const char* const names[8] = { "QUESTS", "STORY", "", "BASICS", "OBJECTS", "TOWNS", "HERO", "COMBAT" };
    return list->FindChild(std::string("PC_MENU_ENTRY_LOGBOOK_") + names[category]);
}

static void AddScrollActions(Component* list, Component* up, Component* down)
{
    Action ignoreUp = { UI_ACTION_TYPE_IGNORE_EVENT, up, UI_EVENT_MOUSE_MOVEMENT };
    Action observeUp = { UI_ACTION_TYPE_OBSERVE_EVENT, up, UI_EVENT_MOUSE_MOVEMENT };
    list->OnSelected.push_back(ignoreUp); list->OnInactive.push_back(observeUp);
    Action ignoreDown = { UI_ACTION_TYPE_IGNORE_EVENT, down, UI_EVENT_MOUSE_MOVEMENT };
    Action observeDown = { UI_ACTION_TYPE_OBSERVE_EVENT, down, UI_EVENT_MOUSE_MOVEMENT };
    list->OnSelected.push_back(ignoreDown); list->OnInactive.push_back(observeDown);
}

// the source's control flow
static Component* ConstructLogbookListPC(Manager* manager, std::map<ECategory, Component*>& categoriesList, const LogEntries& logEntries,
                                         const std::vector<long>& items, Component* parent)
{
    LogEntries entries = logEntries;
    Component* list = manager->CreateComponent("PC_LOGBOOK_CATEGORIES");
    Component* frame = list->FindChild("PC_UI_FRAME_LOGBOOK");
    AddScrollActions(list, parent->FindChild("SCROLL_LIST_UP"), parent->FindChild("SCROLL_LIST_DOWN"));
    AddScrollActions(list, frame->FindChild("SCROLL_LIST_UP"), frame->FindChild("SCROLL_LIST_DOWN"));
    AddScrollActions(list, parent->FindChild("SCROLL_DESCRIPTION_UP"), parent->FindChild("SCROLL_DESCRIPTION_DOWN"));
    AddScrollActions(list, frame->FindChild("SCROLL_DESCRIPTION_UP"), frame->FindChild("SCROLL_DESCRIPTION_DOWN"));
    frame->PositionChildren.push_back(list->FindChild("PC_LOGBOOK_CATEGORIES_TITLE"));
    for (int i = 0; i < 8; ++i) {
        std::string cat = i < 7 ? g_cats[i] : "BOOKS";
        frame->PositionChildren.push_back(list->FindChild("PC_RING_PIC_LOGBOOK_" + cat));
        frame->PositionChildren.push_back(list->FindChild("PC_LOGBOOK_" + cat + "_SUBTITLE"));
        frame->PositionChildren.push_back(list->FindChild("PC_LOGBOOK_" + cat + "_DESC"));
    }
    Component* viewport = parent->FindChild("PC_DESCRIPTION_VIEWPORT");
    if (viewport)
        for (int i = 0; i < 8; ++i) {
            Component* description = list->FindChild(std::string("PC_LOGBOOK_") + (i < 7 ? g_cats[i] : "BOOKS") + "_DESC");
            if (description) viewport->FakeChildren.push_back(description);
        }
    categoriesList.clear();
    unsigned long counts[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for (LogEntries::iterator it = entries.begin(); it != entries.end(); ++it)
        if (it->first != CATEGORY_TUTORIAL) counts[it->first] = (unsigned long)it->second.size();
    std::vector<long> books;
    for (size_t i = 0; i < items.size(); ++i)
        if (IsBook(items[i])) books.push_back(items[i]);
    if (books.empty()) {
        Component* booksEntry = list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS");
        if (booksEntry) booksEntry->Unavailable = true;
    }
    Component* booksList = manager->CreateComponent("PC_SUB_LIST");
    booksList->Offsets = (unsigned long)books.size();
    for (size_t b = 0; b < books.size(); ++b) {
        long index = books[b];
        std::wstring description = GetDescription(index);
        Component* entry = manager->CreateMenuEntryComponent(description);
        Component* model = manager->CreateComponent("PC_MODEL");
        frame->PositionChildren.push_back(model);
        model->MeshIndex = DefMeshIndex(index);
        model->Orientation = GetStartingRightHandedSet(index);
        if (DefIsAnimated(index)) { model->AnimationIndex = DefaultAnim(index); model->MeshType = 4; }
        entry->Add(model);
        Component* details = manager->CreateComponent("PC_DESCRIPTION");
        frame->PositionChildren.push_back(details);
        details->Value = GetDetails(index);
        entry->Add(details);
        if (viewport) viewport->FakeChildren.push_back(details);
        Component* subtitle = manager->CreateComponent("PC_SUBTITLE_INDEPENDANT");
        frame->PositionChildren.push_back(subtitle);
        subtitle->Value = description;
        entry->Add(subtitle);
        if (GetEngineGraphicBank(index) != 0) {
            Component* icon = manager->CreateComponent("PC_ICON_INDEPENDANT");
            frame->PositionChildren.push_back(icon);
            if (icon->Type == UI_TYPE_SPRITE) icon->GraphicIndex = GetEngineGraphicBank(index);
            entry->Add(icon);
        }
        booksList->Scrolling.push_back(entry);
    }
    Component* title = manager->CreateComponent("PC_BOOKS_CATEGORIES_TITLE");
    frame->PositionChildren.push_back(title);
    booksList->NonScrolling.push_back(title);
    Component* booksEntry = list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS");
    if (booksEntry) booksEntry->Add(booksList);
    static const ECategory order[7] = { CATEGORY_BASICS, CATEGORY_OBJECTS, CATEGORY_TOWNS, CATEGORY_HERO, CATEGORY_COMBAT,
                                        CATEGORY_QUEST, CATEGORY_STORY };
    for (int i = 0; i < 7; ++i)
        if (counts[order[i]]) {
            Component* subList = manager->CreateComponent("PC_SUB_LIST");
            subList->Offsets = counts[order[i]];
            Component* t = manager->CreateComponent(std::string("PC_") + g_cats[i] + "_CATEGORIES_TITLE");
            frame->PositionChildren.push_back(t); subList->NonScrolling.push_back(t);
            Component* ring = manager->CreateComponent(std::string("PC_RING_PIC_LOGBOOK_") + g_cats[i] + "_FOR_LIST");
            frame->PositionChildren.push_back(ring); subList->NonScrolling.push_back(ring);
            categoriesList[order[i]] = subList;
        }
    for (LogEntries::iterator it = entries.begin(); it != entries.end(); it++) {
        Component* subList = categoriesList[it->first];
        subList->DefaultChild = (unsigned long)it->second.size() - 1;
        for (unsigned long i = 0; i < it->second.size(); ++i) {
            LogEntry logEntry = it->second[i];
            Component* entry = manager->CreateMenuEntryComponent(logEntry.GetName());
            Component* content = manager->CreateComponent("PC_DESCRIPTION");
            frame->PositionChildren.push_back(content);
            if (viewport) viewport->FakeChildren.push_back(content);
            content->Value = logEntry.GetContent();
            entry->Add(content);
            Component* subtitle = manager->CreateComponent("PC_SUBTITLE_INDEPENDANT");
            frame->PositionChildren.push_back(subtitle);
            subtitle->Value = logEntry.GetAbbreviatedName();
            entry->Add(subtitle);
            subList->Scrolling.push_back(entry);
        }
    }
    for (long category = CATEGORY_QUEST; category <= CATEGORY_COMBAT; ++category) {
        const ECategory key = (ECategory)category;
        if (key == CATEGORY_TUTORIAL) continue;
        std::map<ECategory, Component*>::iterator found = categoriesList.find(key);
        if (found != categoriesList.end()) GetMenuEntryFromCategoryPC((ECategory)category, list)->Add(found->second);
        else GetMenuEntryFromCategoryPC((ECategory)category, list)->Unavailable = true;
    }
    return list;
}

static Component* MakeParent()
{
    Component* parent = New("PC_LOGBOOK_SCREEN", 0);
    parent->Add(New("SCROLL_LIST_UP", 0)); parent->Add(New("SCROLL_LIST_DOWN", 0));
    parent->Add(New("SCROLL_DESCRIPTION_UP", 0)); parent->Add(New("SCROLL_DESCRIPTION_DOWN", 0));
    parent->Add(New("PC_DESCRIPTION_VIEWPORT", 0));
    return parent;
}

static LogEntry Entry(const wchar_t* name, const wchar_t* abbreviated, const wchar_t* content)
{
    LogEntry e; e.Name = name; e.AbbreviatedName = abbreviated; e.Content = content; return e;
}

int main()
{
    Manager manager;
    {   // two categories with entries, two books (one with an icon, one animated), one non-book item
        Component* parent = MakeParent();
        LogEntries log;
        log[CATEGORY_BASICS].push_back(Entry(L"Moving", L"Mv", L"How to move"));
        log[CATEGORY_BASICS].push_back(Entry(L"Talking", L"Tk", L"How to talk"));
        log[CATEGORY_QUEST].push_back(Entry(L"Wasps", L"Ws", L"Kill the wasps"));
        std::vector<long> items; items.push_back(7); items.push_back(101); items.push_back(102);
        std::map<ECategory, Component*> categories;
        categories[CATEGORY_HERO] = 0;                       // stale entry: cleared by the build
        Component* list = ConstructLogbookListPC(&manager, categories, log, items, parent);
        Component* frame = list->FindChild("PC_UI_FRAME_LOGBOOK");
        check(list->OnSelected.size() == 8 && list->OnInactive.size() == 8, "eight ignore and eight observe actions");
        check(list->OnSelected[0].Target == parent->FindChild("SCROLL_LIST_UP") && list->OnSelected[0].Type == UI_ACTION_TYPE_IGNORE_EVENT,
              "first ignore action on the screen's list-up button");
        check(list->OnInactive[3].Target == frame->FindChild("SCROLL_LIST_DOWN") && list->OnInactive[3].Type == UI_ACTION_TYPE_OBSERVE_EVENT
              && list->OnInactive[3].Event == UI_EVENT_MOUSE_MOVEMENT, "fourth observe action on the frame's list-down button");
        check(list->OnSelected[7].Target == frame->FindChild("SCROLL_DESCRIPTION_DOWN"), "last action on the frame's description-down button");
        check(frame->PositionChildren[0]->Name == "PC_LOGBOOK_CATEGORIES_TITLE" && frame->PositionChildren[24]->Name == "PC_LOGBOOK_BOOKS_DESC",
              "25 template children positioned by the frame");
        Component* viewport = parent->FindChild("PC_DESCRIPTION_VIEWPORT");
        check(viewport->FakeChildren.size() == 8 + 2 + 3, "descriptions, book details and entry contents in the viewport");
        check(categories.size() == 2 && categories.count(CATEGORY_HERO) == 0, "sub-lists only for the categories with entries");
        Component* basics = categories[CATEGORY_BASICS];
        check(basics->Offsets == 2 && basics->Scrolling.size() == 2 && basics->DefaultChild == 1, "basics sub-list of two");
        check(basics->Scrolling[1]->Value == L"Talking" && basics->Scrolling[1]->Children[0]->Value == L"How to talk"
              && basics->Scrolling[1]->Children[1]->Value == L"Tk", "entry name, content and abbreviated subtitle");
        check(basics->NonScrolling.size() == 2 && basics->NonScrolling[1]->Name == "PC_RING_PIC_LOGBOOK_BASICS_FOR_LIST", "title and ring");
        check(GetMenuEntryFromCategoryPC(CATEGORY_BASICS, list)->Children.back() == basics, "basics sub-list under its menu entry");
        check(GetMenuEntryFromCategoryPC(CATEGORY_QUEST, list)->Children.back() == categories[CATEGORY_QUEST], "quest sub-list under its entry");
        check(GetMenuEntryFromCategoryPC(CATEGORY_STORY, list)->Unavailable && GetMenuEntryFromCategoryPC(CATEGORY_COMBAT, list)->Unavailable
              && !GetMenuEntryFromCategoryPC(CATEGORY_BASICS, list)->Unavailable, "empty categories unavailable");
        Component* booksEntry = list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS");
        check(!booksEntry->Unavailable && booksEntry->Children.size() == 1, "books entry available, holding the books list");
        Component* books = booksEntry->Children[0];
        check(books->Offsets == 2 && books->Scrolling.size() == 2 && books->NonScrolling[0]->Name == "PC_BOOKS_CATEGORIES_TITLE", "two books");
        Component* odd = books->Scrolling[0];
        check(odd->Value == L"Book101" && odd->Children.size() == 4 && odd->Children[3]->GraphicIndex == 1010, "book with an icon");
        check(odd->Children[0]->MeshIndex == 303 && odd->Children[0]->Orientation == 1101 && odd->Children[0]->MeshType == 0, "still model");
        Component* even = books->Scrolling[1];
        check(even->Children.size() == 3 && even->Children[1]->Value == L"Text102" && even->Children[2]->Value == L"Book102", "book without icon");
        check(even->Children[0]->AnimationIndex == 714 && even->Children[0]->MeshType == 4, "animated model");
    }
    {   // no entries, no books
        Component* parent = MakeParent();
        LogEntries log;
        std::vector<long> items; items.push_back(3);
        std::map<ECategory, Component*> categories;
        Component* list = ConstructLogbookListPC(&manager, categories, log, items, parent);
        check(categories.empty(), "no sub-lists");
        check(list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS")->Unavailable, "books entry unavailable");
        check(list->FindChild("PC_MENU_ENTRY_LOGBOOK_BOOKS")->Children[0]->Scrolling.empty(), "empty books list still attached");
        int unavailable = 0;
        for (long c = CATEGORY_QUEST; c <= CATEGORY_COMBAT; ++c)
            if (c != CATEGORY_TUTORIAL && GetMenuEntryFromCategoryPC((ECategory)c, list)->Unavailable) ++unavailable;
        check(unavailable == 7, "all seven categories unavailable");
    }
    for (size_t i = 0; i < g_all.size(); ++i) delete g_all[i];
    if (g_fail == 0) std::printf("006096e0_TEST PASS\n");
    return g_fail != 0;
}
