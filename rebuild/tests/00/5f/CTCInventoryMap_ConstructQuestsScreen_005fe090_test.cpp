// Behaviour test for 0x005FE090 CTCInventoryMap::ConstructQuestsScreen: creates the UI_QUESTS_MENU list, InitialiseOffsets(2), adds the
// UI_SCROLLING_VIEWPORT_TEXT_AREA as a non-scrolling child, then one menu entry for the current quests (text TEXT_GUI_MENU_CURRENT_QUESTS,
// layout 0 / 1 / 0) and one for the past quests (TEXT_GUI_MENU_PAST_QUESTS, layout 25 / 2/3 / 5). An entry whose list builder returned a
// list uses the BRANCH template and gets the list as its first child; otherwise the plain template and MakeUnavailable. Each entry then
// gets the guide description (also a fake child of the viewport), its subtitle and its ring picture; both entries are added with
// AddScrollingChildAndSetOffsets (current first) and the menu is returned. Counted pointers keep every component alive exactly as long
// as a holder references it (the counted-pointer model: count block {RefCount, DeleteFunc, Data}, dtor releases, Reset releases first).
#include <cstdio>
#include <cstring>

static int g_alive = 0;

struct Component;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };

struct Ptr {                                  // CCountedPointer<CComponent>
    Component* Data; Info* Info_;
    Ptr() : Data(0), Info_(0) { }
    explicit Ptr(Component* d);
    Ptr(const Ptr& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    ~Ptr() { Release(); }
    void Release();
    void Reset(Component* d) { Release(); Data = d; if (d) { Info_ = new Info; Info_->RefCount = 1; Info_->DeleteFunc = DeleteData; Info_->Data = d; } }
    static void DeleteData(void* d);
    bool IsNull() const { return Data == 0; }
private:
    Ptr& operator=(const Ptr&);
};

struct Component {
    char Name[64];
    float A, B, C;                            // menu-entry layout floats (entries only)
    bool Unavailable;
    unsigned long Offsets;
    int ChildCount; Ptr Children[8];          // AddChild / AddScrollingChildAndSetOffsets / AddNonScrollingChild (shared ownership)
    int FakeCount; Component* Fake[4];        // CViewport::AddFakeChild (not owning)
    char Text[64];                            // CText::SetValue
    Component(const char* name) : A(0), B(0), C(0), Unavailable(false), Offsets(0), ChildCount(0), FakeCount(0)
    { std::strcpy(Name, name); Text[0] = 0; ++g_alive; }
    ~Component() { --g_alive; }
};

Ptr::Ptr(Component* d) : Data(d) { if (Data) { Info_ = new Info; Info_->RefCount = 1; Info_->DeleteFunc = DeleteData; Info_->Data = Data; } else Info_ = 0; }
void Ptr::Release()
{
    if (Info_) {
        --Info_->RefCount;
        if (Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; }
    }
    Data = 0; Info_ = 0;
}
void Ptr::DeleteData(void* d) { delete static_cast<Component*>(d); }

static void Add(Component* parent, const Ptr& child)
{
    Ptr& slot = parent->Children[parent->ChildCount++];
    slot.Data = child.Data; slot.Info_ = child.Info_;
    if (slot.Info_) ++slot.Info_->RefCount;
}

static Component* CreateComponent(const char* name) { return new Component(name); }
static Component* CreateMenuEntry(const char* text, float a, float b, const char* templ, float c)
{
    Component* entry = new Component(templ);
    std::strcpy(entry->Text, text);
    entry->A = a; entry->B = b; entry->C = c;
    return entry;
}

// The row's logic over the model (same statement order as the source).
static Component* ConstructQuestsScreen(Component* (*pastList)(), Component* (*reviewList)())
{
    Component* menu = CreateComponent("UI_QUESTS_MENU");
    const char* pastQuestsText = "TEXT_GUI_MENU_PAST_QUESTS";
    Ptr pastQuestsEntry;
    Ptr pastQuestsList(pastList());
    menu->Offsets = 2;
    Ptr textArea(CreateComponent("UI_SCROLLING_VIEWPORT_TEXT_AREA"));
    Component* viewport = textArea.Data;
    Add(menu, textArea);
    Ptr description;
    Ptr subtitle;
    Ptr ringPicture;
    const char* currentQuestsText = "TEXT_GUI_MENU_CURRENT_QUESTS";
    Ptr currentQuestsEntry;
    Ptr currentQuestsList(reviewList());
    if (!currentQuestsList.IsNull()) {
        currentQuestsEntry.Reset(CreateMenuEntry(currentQuestsText, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 0.0f));
        Add(currentQuestsEntry.Data, currentQuestsList);
    } else {
        currentQuestsEntry.Reset(CreateMenuEntry(currentQuestsText, 0.0f, 1.0f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW", 0.0f));
        currentQuestsEntry.Data->Unavailable = true;
    }
    description.Reset(CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF"));
    std::strcpy(description.Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS");
    viewport->Fake[viewport->FakeCount++] = description.Data;
    Add(currentQuestsEntry.Data, description);
    subtitle.Reset(CreateComponent("UI_CURRENTQUESTS_SUBTITLE"));
    Add(currentQuestsEntry.Data, subtitle);
    ringPicture.Reset(CreateComponent("UI_RING_PIC_CURRENT_QUESTS"));
    Add(currentQuestsEntry.Data, ringPicture);
    if (!pastQuestsList.IsNull()) {
        pastQuestsEntry.Reset(CreateMenuEntry(pastQuestsText, 25.0f, 0.6666667f, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW", 5.0f));
        Add(pastQuestsEntry.Data, pastQuestsList);
    } else {
        pastQuestsEntry.Reset(CreateMenuEntry(pastQuestsText, 25.0f, 0.6666667f, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW", 5.0f));
        pastQuestsEntry.Data->Unavailable = true;
    }
    description.Reset(CreateComponent("UI_TEXT_WEAPONS_DESCRIPTION_TEMPLATE_NEW_ONE_OFF"));
    std::strcpy(description.Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS");
    viewport->Fake[viewport->FakeCount++] = description.Data;
    Add(pastQuestsEntry.Data, description);
    subtitle.Reset(CreateComponent("UI_PASTQUESTS_SUBTITLE"));
    Add(pastQuestsEntry.Data, subtitle);
    ringPicture.Reset(CreateComponent("UI_RING_PIC_PAST_QUESTS"));
    Add(pastQuestsEntry.Data, ringPicture);
    Add(menu, currentQuestsEntry);
    Add(menu, pastQuestsEntry);
    return menu;
}

static Component* NoList() { return 0; }
static Component* PastList() { return new Component("UI_SUB_LIST_PAST"); }
static Component* ReviewList() { return new Component("UI_SUB_LIST_CURRENT"); }

static int Fail(int line) { std::printf("005fe090_TEST FAIL (%d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return Fail(__LINE__); } while (0)
static bool Near(float a, float b) { float d = a - b; return d < 1e-5f && d > -1e-5f; }

static void Destroy(Component* c) { delete c; }

int main()
{
    // both lists present
    Component* menu = ConstructQuestsScreen(PastList, ReviewList);
    CHECK(!std::strcmp(menu->Name, "UI_QUESTS_MENU") && menu->Offsets == 2 && menu->ChildCount == 3);
    Component* viewport = menu->Children[0].Data;
    CHECK(!std::strcmp(viewport->Name, "UI_SCROLLING_VIEWPORT_TEXT_AREA") && viewport->FakeCount == 2);
    Component* current = menu->Children[1].Data;
    Component* past = menu->Children[2].Data;
    CHECK(!std::strcmp(current->Name, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW") && !std::strcmp(current->Text, "TEXT_GUI_MENU_CURRENT_QUESTS"));
    CHECK(Near(current->A, 0.0f) && Near(current->B, 1.0f) && Near(current->C, 0.0f) && !current->Unavailable);
    CHECK(current->ChildCount == 4 && !std::strcmp(current->Children[0].Data->Name, "UI_SUB_LIST_CURRENT"));
    CHECK(!std::strcmp(current->Children[1].Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS"));
    CHECK(!std::strcmp(current->Children[2].Data->Name, "UI_CURRENTQUESTS_SUBTITLE"));
    CHECK(!std::strcmp(current->Children[3].Data->Name, "UI_RING_PIC_CURRENT_QUESTS"));
    CHECK(!std::strcmp(past->Name, "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_NEW") && !std::strcmp(past->Text, "TEXT_GUI_MENU_PAST_QUESTS"));
    CHECK(Near(past->A, 25.0f) && Near(past->B, 2.0f / 3.0f) && Near(past->C, 5.0f) && past->ChildCount == 4);
    CHECK(!std::strcmp(past->Children[0].Data->Name, "UI_SUB_LIST_PAST") && !std::strcmp(past->Children[3].Data->Name, "UI_RING_PIC_PAST_QUESTS"));
    CHECK(viewport->Fake[0] == current->Children[1].Data && viewport->Fake[1] == past->Children[1].Data);
    // every child held once by its parent after the locals are gone
    CHECK(menu->Children[1].Info_->RefCount == 1 && current->Children[0].Info_->RefCount == 1 && past->Children[1].Info_->RefCount == 1);
    CHECK(g_alive == 12);
    Destroy(menu);
    CHECK(g_alive == 0);

    // no completed / no active quests: plain template, unavailable, no list child
    menu = ConstructQuestsScreen(NoList, NoList);
    current = menu->Children[1].Data; past = menu->Children[2].Data;
    CHECK(!std::strcmp(current->Name, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW") && current->Unavailable && current->ChildCount == 3);
    CHECK(!std::strcmp(past->Name, "UI_WEAPON_MENU_ENTRY_TEMPLATE_NEW") && past->Unavailable && past->ChildCount == 3);
    CHECK(!std::strcmp(past->Children[0].Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS"));
    Destroy(menu);
    CHECK(g_alive == 0);

    std::printf("005fe090_TEST PASS\n");
    return 0;
}