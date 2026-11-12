// Behaviour test for CTCInventoryMap::ConstructLogbookList @ 0x00607330 (console logbook screen).
// Subject: a copy of the row's logic over a small mock UI (components with named children, sub-lists, menu entries, deletion
// method on the selection arrow), a mock hero logbook (category -> entries with name / abbreviated name / content) and a mock
// inventory (book ids). Checks, over two runs, what the screen ends up holding: the eight category descriptions faked into the
// text viewport, one sub-list per non-empty category (offsets = entry count, title / back / ring as non-scrolling children, one
// scrolling menu entry per logbook entry labelled with the ABBREVIATED name, content and name text children, default child =
// count - 1), the BOOKS list (one entry per book: model, details, name, and a sprite only for books with an engine graphic) and the
// disabled entries (MakeUnavailable + arrow deletion UI_DELETE_THIS_INSTANCE) for the books entry without books and every empty
// category except the tutorial.
#include <cstdio>
#include <string>
#include <vector>
#include <map>
typedef std::string Str;

enum ECategory { CATEGORY_QUEST, CATEGORY_STORY, CATEGORY_TUTORIAL, CATEGORY_BASICS, CATEGORY_OBJECTS, CATEGORY_TOWNS, CATEGORY_HERO, CATEGORY_COMBAT };
enum EDeletionMethod { UI_DO_NOT_DELETE = 0, UI_DELETE_THIS_INSTANCE = 1 };
struct CDeletion { EDeletionMethod Method; CDeletion() : Method(UI_DO_NOT_DELETE) {} };

struct CComponent {
    Str Name, Text;
    std::vector<CComponent*> Children, NonScrolling, Scrolling, Fake;
    std::map<Str, CComponent*> Named;
    bool Unavailable; int Deletion; long Offsets, DefaultChild, MeshIndex, GraphicIndex; bool IsSprite;
    CComponent(const Str& n) : Name(n), Unavailable(false), Deletion(0), Offsets(-1), DefaultChild(-99), MeshIndex(-1), GraphicIndex(-1), IsSprite(n == "UI_ITEM_SPRITE_TEMPLATE") {}
    CComponent* FindChild(const Str& n) { std::map<Str, CComponent*>::iterator it = Named.find(n); return it == Named.end() ? 0 : it->second; }
    void AddChild(CComponent* c) { Children.push_back(c); }
    void AddFakeChild(CComponent* c) { Fake.push_back(c); }
    void MakeUnavailable() { Unavailable = true; }
    void SetDeletion(const CDeletion& d) { Deletion = d.Method; }
    void InitialiseOffsets(long n) { Offsets = n; }
    void AddNonScrollingChild(CComponent* c) { NonScrolling.push_back(c); }
    void AddScrollingChildAndSetOffsets(CComponent* c) { Scrolling.push_back(c); }
    void SetDefaultChild(long n) { DefaultChild = n; }
    void SetValue(const Str& s) { Text = s; }
};
static std::vector<CComponent*> gAll;
static CComponent* New(const Str& n) { CComponent* c = new CComponent(n); gAll.push_back(c); return c; }
static const char* kCatEntry[] = { "UI_MENU_ENTRY_LOGBOOK_QUESTS", "UI_MENU_ENTRY_LOGBOOK_STORY", "UI_MENU_ENTRY_LOGBOOK_TUTORIAL",
    "UI_MENU_ENTRY_LOGBOOK_BASICS", "UI_MENU_ENTRY_LOGBOOK_OBJECTS", "UI_MENU_ENTRY_LOGBOOK_TOWNS", "UI_MENU_ENTRY_LOGBOOK_HERO", "UI_MENU_ENTRY_LOGBOOK_COMBAT" };
static const char* kDesc[] = { "UI_LOGBOOK_BASICS_DESC", "UI_LOGBOOK_OBJECTS_DESC", "UI_LOGBOOK_TOWNS_DESC", "UI_LOGBOOK_HERO_DESC",
    "UI_LOGBOOK_COMBAT_DESC", "UI_LOGBOOK_QUESTS_DESC", "UI_LOGBOOK_STORY_DESC", "UI_LOGBOOK_BOOKS_DESC" };
static CComponent* CreateComponent(const Str& n)
{
    CComponent* c = New(n);
    if (n == "UI_LOGBOOK_CATEGORIES") {
        c->Named["UI_SCROLLING_VIEWPORT_TEXT_AREA"] = New("viewport");
        for (int i = 0; i < 8; ++i) c->Named[kDesc[i]] = New(kDesc[i]);
        for (int k = 0; k < 8; ++k) { CComponent* e = New(kCatEntry[k]); e->Named["UI_SELECTION_ARROW_RIGHT"] = New("arrow"); c->Named[kCatEntry[k]] = e; }
        CComponent* b = New("UI_MENU_ENTRY_LOGBOOK_BOOKS"); b->Named["UI_SELECTION_ARROW_RIGHT"] = New("arrow"); c->Named["UI_MENU_ENTRY_LOGBOOK_BOOKS"] = b;
    }
    return c;
}
static CComponent* CreateMenuEntryComponent(const Str& label) { CComponent* c = New("menu_entry"); c->Text = label; return c; }

struct CHeroLogBookEntry { Str Name, AbbreviatedName, Content;
    const Str& GetName() const { return Name; } const Str& GetAbbreviatedName() const { return AbbreviatedName; } const Str& GetContent() const { return Content; } };
typedef std::map<ECategory, std::vector<CHeroLogBookEntry> > TLog;
// mock inventory: book ids are even; a book has an engine graphic when its id is above 10
static bool IsBook(long id) { return id % 2 == 0; }
static Str Num(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }
static Str GetDescription(long id) { return "book" + Num(id); }
static Str GetDetails(long id) { return "details" + Num(id); }
static long GetEngineGraphic(long id) { return id > 10 ? id : 0; }

struct CTCInventoryMap {
    std::map<ECategory, CComponent*> CategoriesList;
    CComponent* GetMenuEntryFromCategory(ECategory c, CComponent* parent) { return parent->FindChild(kCatEntry[c]); }
    CComponent* ConstructLogbookList(CComponent* parent, const TLog& log, const std::vector<std::pair<long, long> >* inventory);
};

CComponent* CTCInventoryMap::ConstructLogbookList(CComponent* parent, const TLog& log, const std::vector<std::pair<long, long> >* inventory)
{
    TLog log_entries = log;
    CComponent* categories = CreateComponent("UI_LOGBOOK_CATEGORIES");
    CComponent* viewport = categories->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    if (viewport)
        for (int d = 0; d < 8; ++d) { CComponent* desc = categories->FindChild(kDesc[d]); if (desc) viewport->AddFakeChild(desc); }
    CategoriesList.clear();
    long count[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    TLog::iterator it;
    for (it = log_entries.begin(); it != log_entries.end(); ++it)
        if (it->first != CATEGORY_TUTORIAL) count[it->first] = (long)it->second.size();
    std::vector<long> books;
    if (inventory)
        for (size_t k = 0; k < inventory->size(); ++k) if (IsBook((*inventory)[k].first)) books.push_back((*inventory)[k].first);
    if (books.empty()) {
        CComponent* entry = categories->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS");
        if (entry) { entry->MakeUnavailable(); CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (entry && arrow) { CDeletion deletion; deletion.Method = UI_DELETE_THIS_INSTANCE; arrow->SetDeletion(deletion); } }
    }
    CComponent* books_list = CreateComponent("UI_SUB_LIST");
    books_list->InitialiseOffsets((long)books.size());
    for (size_t b = 0; b < books.size(); ++b) {
        long book = books[b];
        Str description = GetDescription(book);
        CComponent* entry = CreateMenuEntryComponent(description);
        CComponent* model = CreateComponent("UI_ITEM_MODEL_TEMPLATE_NEW"); model->MeshIndex = book * 100;
        entry->AddChild(model);
        CComponent* details = CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW"); details->SetValue(GetDetails(book));
        entry->AddChild(details);
        CComponent* text_viewport = parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
        if (text_viewport) text_viewport->AddFakeChild(details);
        if (GetEngineGraphic(book) != 0) {
            CComponent* name = CreateComponent("UI_TEXT_ITEM_NAME_TEMPLATE_NEW"); name->SetValue(description); entry->AddChild(name);
            CComponent* sprite = CreateComponent("UI_ITEM_SPRITE_TEMPLATE");
            if (sprite->IsSprite) sprite->GraphicIndex = GetEngineGraphic(book);
            entry->AddChild(sprite);
        } else {
            CComponent* name = CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW"); name->SetValue(description); entry->AddChild(name);
        }
        books_list->AddScrollingChildAndSetOffsets(entry);
    }
    if (!books.empty()) {
        books_list->AddNonScrollingChild(CreateComponent("UI_BOOKS_CATEGORIES_TITLE"));
        books_list->AddNonScrollingChild(CreateComponent("UI_BACK_FOR_LIST"));
        CComponent* books_entry = categories->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS");
        if (books_entry) books_entry->AddChild(books_list);
    }
    static const ECategory kOrder[] = { CATEGORY_BASICS, CATEGORY_OBJECTS, CATEGORY_TOWNS, CATEGORY_HERO, CATEGORY_COMBAT, CATEGORY_QUEST, CATEGORY_STORY };
    for (int o = 0; o < 7; ++o) {
        ECategory c = kOrder[o];
        if (count[c]) {
            CComponent* list = CreateComponent("UI_SUB_LIST");
            list->InitialiseOffsets(count[c]);
            list->AddNonScrollingChild(CreateComponent("title"));
            list->AddNonScrollingChild(CreateComponent("UI_BACK_FOR_LIST"));
            list->AddNonScrollingChild(CreateComponent("ring"));
            CategoriesList[c] = list;
        }
    }
    for (it = log_entries.begin(); it != log_entries.end(); ++it) {
        CComponent* list = CategoriesList[it->first];
        list->SetDefaultChild((long)it->second.size() - 1);
        for (unsigned long i = 0; i < it->second.size(); ++i) {
            CHeroLogBookEntry entry = it->second[i];
            Str abbreviated_name = entry.GetAbbreviatedName();
            CComponent* menu_entry = CreateMenuEntryComponent(abbreviated_name);
            CComponent* content = CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW");
            viewport->AddFakeChild(content);
            content->SetValue(entry.GetContent());
            menu_entry->AddChild(content);
            CComponent* title = CreateComponent("UI_TEXT_WEAPONS_NAME_TEMPLATE_NEW");
            title->SetValue(entry.GetName());
            menu_entry->AddChild(title);
            list->AddScrollingChildAndSetOffsets(menu_entry);
        }
    }
    for (long category = CATEGORY_QUEST; category <= CATEGORY_COMBAT; ++category) {
        if (category == CATEGORY_TUTORIAL) continue;
        std::map<ECategory, CComponent*>::iterator found = CategoriesList.find((ECategory)category);
        if (found != CategoriesList.end()) {
            GetMenuEntryFromCategory((ECategory)category, categories)->AddChild(found->second);
        } else {
            CComponent* entry = GetMenuEntryFromCategory((ECategory)category, categories);
            entry->MakeUnavailable();
            CComponent* arrow = entry->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) { CDeletion deletion; deletion.Method = UI_DELETE_THIS_INSTANCE; arrow->SetDeletion(deletion); }
        }
    }
    return categories;
}

static int gFail = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("fail: %s\n", what); gFail = 1; } }
static CHeroLogBookEntry E(const Str& n, const Str& a, const Str& c) { CHeroLogBookEntry e; e.Name = n; e.AbbreviatedName = a; e.Content = c; return e; }

int main()
{
    TLog log;
    log[CATEGORY_QUEST].push_back(E("Quest One", "Q1", "quest one text"));
    log[CATEGORY_QUEST].push_back(E("Quest Two", "Q2", "quest two text"));
    log[CATEGORY_BASICS].push_back(E("Basics", "B", "basics text"));
    std::vector<std::pair<long, long> > inv;
    inv.push_back(std::make_pair(4L, 1L)); inv.push_back(std::make_pair(7L, 1L)); inv.push_back(std::make_pair(12L, 2L));
    CComponent* parent = New("parent"); parent->Named["UI_SCROLLING_VIEWPORT_TEXT_AREA"] = New("parent_viewport");

    // run 1: two books (one with an engine graphic), quests and basics populated
    CTCInventoryMap map1;
    CComponent* cats = map1.ConstructLogbookList(parent, log, &inv);
    CComponent* vp = cats->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA");
    Check(vp->Fake.size() == 8 + 3, "viewport: 8 descriptions + 3 entry contents");
    Check(vp->Fake[0]->Name == "UI_LOGBOOK_BASICS_DESC" && vp->Fake[7]->Name == "UI_LOGBOOK_BOOKS_DESC", "description order");
    Check(map1.CategoriesList.size() == 2, "two category lists");
    CComponent* ql = map1.CategoriesList[CATEGORY_QUEST];
    Check(ql->Offsets == 2 && ql->Scrolling.size() == 2 && ql->NonScrolling.size() == 3 && ql->DefaultChild == 1, "quest list shape");
    Check(ql->Scrolling[1]->Text == "Q2" && ql->Scrolling[1]->Children[0]->Text == "quest two text" && ql->Scrolling[1]->Children[1]->Text == "Quest Two", "quest entry texts");
    Check(map1.CategoriesList[CATEGORY_BASICS]->DefaultChild == 0, "basics default child");
    Check(cats->FindChild(kCatEntry[CATEGORY_QUEST])->Children.size() == 1 && !cats->FindChild(kCatEntry[CATEGORY_QUEST])->Unavailable, "quest entry gets its list");
    for (int k = CATEGORY_OBJECTS; k <= CATEGORY_COMBAT; ++k) {
        CComponent* e = cats->FindChild(kCatEntry[k]);
        Check(e->Unavailable && e->FindChild("UI_SELECTION_ARROW_RIGHT")->Deletion == UI_DELETE_THIS_INSTANCE, "empty category disabled");
    }
    Check(cats->FindChild(kCatEntry[CATEGORY_STORY])->Unavailable, "story disabled");
    Check(!cats->FindChild(kCatEntry[CATEGORY_TUTORIAL])->Unavailable, "tutorial untouched");
    CComponent* be = cats->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS");
    Check(!be->Unavailable && be->Children.size() == 1, "books entry gets the books list");
    CComponent* bl = be->Children[0];
    Check(bl->Offsets == 2 && bl->Scrolling.size() == 2 && bl->NonScrolling.size() == 2, "books list shape");
    Check(bl->Scrolling[0]->Text == "book4" && bl->Scrolling[0]->Children.size() == 3, "book without graphic: model, details, name");
    Check(bl->Scrolling[1]->Children.size() == 4 && bl->Scrolling[1]->Children[3]->GraphicIndex == 12, "book with graphic: + sprite");
    Check(parent->FindChild("UI_SCROLLING_VIEWPORT_TEXT_AREA")->Fake.size() == 2, "book details faked into the parent viewport");

    // run 2: no books, only a story entry
    TLog log2; log2[CATEGORY_STORY].push_back(E("Story", "S", "story text"));
    CTCInventoryMap map2;
    CComponent* cats2 = map2.ConstructLogbookList(parent, log2, 0);
    CComponent* be2 = cats2->FindChild("UI_MENU_ENTRY_LOGBOOK_BOOKS");
    Check(be2->Unavailable && be2->FindChild("UI_SELECTION_ARROW_RIGHT")->Deletion == UI_DELETE_THIS_INSTANCE && be2->Children.empty(), "no books: entry disabled");
    Check(map2.CategoriesList.size() == 1 && map2.CategoriesList[CATEGORY_STORY]->Scrolling[0]->Text == "S", "story list");
    Check(cats2->FindChild(kCatEntry[CATEGORY_QUEST])->Unavailable, "quests disabled in run 2");

    for (size_t i = 0; i < gAll.size(); ++i) delete gAll[i];
    if (gFail) { std::printf("00607330_TEST FAIL\n"); return 1; }
    std::printf("00607330_TEST PASS\n");
    return 0;
}
