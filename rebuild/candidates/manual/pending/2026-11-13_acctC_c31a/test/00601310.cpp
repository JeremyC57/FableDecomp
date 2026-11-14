// Behaviour test for 0x00601310 CTCInventoryMap::ConstructQuestsScreenPC: creates the PC_QUESTS_MENU list; for the scroll buttons of
// the screen (SCROLL_LIST_*, SCROLL_DESCRIPTION_*) adds an IGNORE mouse-movement action on selected and an OBSERVE one on inactive,
// for the same buttons of the menu's PC_UI_FRAME_QUESTS frame the reverse (OBSERVE on selected, IGNORE on inactive); positions the
// title in the frame; InitialiseOffsets(2); builds the current-quests entry (BRANCH template + list child + name CURRENT_QUESTS_ENTRY
// when the review-list builder returned a list, else the plain template made unavailable) and the past-quests entry (same, no name),
// each with a PC_DESCRIPTION (positioned in the frame, guide text set, fake child of the screen's PC_DESCRIPTION_VIEWPORT), a
// subtitle and a ring picture (both positioned in the frame); adds current then past as scrolling children; returns the menu.
// Model: counted pointers as in the landed twin's test (0x005FE090); the row's statements in source order.
#include <cstdio>
#include <cstring>
static int g_alive = 0;
struct Component;
struct Info { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
template<class T> struct Ptr {
    T* Data; Info* Info_;
    Ptr() : Data(0), Info_(0) { }
    explicit Ptr(T* d) : Data(d) { if (Data) { Info_ = new Info; Info_->RefCount = 1; Info_->DeleteFunc = DeleteData; Info_->Data = Data; } else Info_ = 0; }
    Ptr(const Ptr& o) : Data(o.Data), Info_(o.Info_) { if (Info_) ++Info_->RefCount; }
    ~Ptr() { Release(); }
    void Release() { if (Info_) { if (--Info_->RefCount == 0) { Info_->DeleteFunc(Info_->Data); delete Info_; } } Data = 0; Info_ = 0; }
    void Reset(T* d) { Release(); Data = d; if (d) { Info_ = new Info; Info_->RefCount = 1; Info_->DeleteFunc = DeleteData; Info_->Data = d; } }
    void Share(const Ptr& o) { Data = o.Data; Info_ = o.Info_; if (Info_) ++Info_->RefCount; }
    static void DeleteData(void* d) { delete static_cast<T*>(d); }
    bool IsNull() const { return Data == 0; }
    T* operator->() const { return Data; }
private:
    Ptr& operator=(const Ptr&);
};
enum { ACTION_NONE = 0, ACTION_OBSERVE_EVENT = 1, ACTION_IGNORE_EVENT = 2, EVENT_MOUSE_MOVEMENT = 7 };
struct Action {
    int ActionType; Component* Target; Action* Self; unsigned long Params[4]; int ParamCount;
    Action() : ActionType(ACTION_NONE), Target(0), Self(0), ParamCount(0) { ++g_alive; }
    ~Action() { --g_alive; }
    void AddParam(Component* c, Ptr<Action>& self) { Target = c; Self = self.Data; }
};
struct Component {
    char Name[64]; char Template[64]; char Text[64];
    bool Unavailable; unsigned long Offsets;
    int ChildCount; Ptr<Component> Children[8];        // AddChild / AddScrollingChildAndSetOffsets
    int NamedCount; Component* Named[8];               // children FindChild can see (owned by the fixture)
    int PosCount; Component* Pos[8];                   // AddPositionChild
    int FakeCount; Component* Fake[4];                 // CViewport::AddFakeChild
    int SelCount; Ptr<Action> Selected[8];             // AddActionOnSelected
    int InaCount; Ptr<Action> Inactive[8];             // AddActionOnInactive
    Component(const char* name) : Unavailable(false), Offsets(0), ChildCount(0), NamedCount(0), PosCount(0), FakeCount(0), SelCount(0), InaCount(0)
    { std::strcpy(Name, name); std::strcpy(Template, name); Text[0] = 0; ++g_alive; }
    ~Component() { for (int i = 0; i < NamedCount; ++i) delete Named[i]; --g_alive; }
    Component* FindChild(const char* name) { for (int i = 0; i < NamedCount; ++i) if (!std::strcmp(Named[i]->Name, name)) return Named[i]; return 0; }
    Component* AddNamed(const char* name) { return Named[NamedCount++] = new Component(name); }
    void AddChild(const Ptr<Component>& c) { Children[ChildCount++].Share(c); }
    void AddActionOnSelected(const Ptr<Action>& a) { Selected[SelCount++].Share(a); }
    void AddActionOnInactive(const Ptr<Action>& a) { Inactive[InaCount++].Share(a); }
};
static Component* CreateComponent(const char* name)
{
    Component* c = new Component(name);
    if (!std::strcmp(name, "PC_QUESTS_MENU")) {
        Component* frame = c->AddNamed("PC_UI_FRAME_QUESTS");
        frame->AddNamed("SCROLL_LIST_UP"); frame->AddNamed("SCROLL_LIST_DOWN");
        frame->AddNamed("SCROLL_DESCRIPTION_UP"); frame->AddNamed("SCROLL_DESCRIPTION_DOWN");
        c->AddNamed("PC_TITLE_QUESTS_MENU");
    }
    return c;
}
static Component* CreateMenuEntry(const char* text, const char* templ)
{
    Component* entry = new Component(templ);
    std::strcpy(entry->Text, text);
    return entry;
}
static int g_pastArgsOk = 0, g_reviewArgsOk = 0;
static Component* g_screen = 0;
// One scroll-button block of the row: observe / ignore actions for `up`, then for `down`.
static void ScrollActions(Component* menu, Component* owner, const char* upName, const char* downName, bool observeOnSelected)
{
    Component* up = owner->FindChild(upName);
    Component* down = owner->FindChild(downName);
    Ptr<Action> observeUp(new Action);
    observeUp->ActionType = ACTION_OBSERVE_EVENT;
    observeUp->AddParam(up, observeUp);
    unsigned long observeUpParam = EVENT_MOUSE_MOVEMENT;
    observeUp->Params[observeUp->ParamCount++] = observeUpParam;
    Ptr<Action> ignoreUp(new Action);
    ignoreUp->ActionType = ACTION_IGNORE_EVENT;
    ignoreUp->AddParam(up, ignoreUp);
    unsigned long ignoreUpParam = EVENT_MOUSE_MOVEMENT;
    ignoreUp->Params[ignoreUp->ParamCount++] = ignoreUpParam;
    if (observeOnSelected) { menu->AddActionOnSelected(observeUp); menu->AddActionOnInactive(ignoreUp); }
    else { menu->AddActionOnSelected(ignoreUp); menu->AddActionOnInactive(observeUp); }
    Ptr<Action> observeDown(new Action);
    observeDown->ActionType = ACTION_OBSERVE_EVENT;
    observeDown->AddParam(down, observeDown);
    unsigned long observeDownParam = EVENT_MOUSE_MOVEMENT;
    observeDown->Params[observeDown->ParamCount++] = observeDownParam;
    Ptr<Action> ignoreDown(new Action);
    ignoreDown->ActionType = ACTION_IGNORE_EVENT;
    ignoreDown->AddParam(down, ignoreDown);
    unsigned long ignoreDownParam = EVENT_MOUSE_MOVEMENT;
    ignoreDown->Params[ignoreDown->ParamCount++] = ignoreDownParam;
    if (observeOnSelected) { menu->AddActionOnSelected(observeDown); menu->AddActionOnInactive(ignoreDown); }
    else { menu->AddActionOnSelected(ignoreDown); menu->AddActionOnInactive(observeDown); }
}
// The row's logic over the model (same statement order as the source).
static Component* ConstructQuestsScreenPC(Component* screen, Component* (*pastListPC)(Component*, Component*), Component* (*reviewListPC)(Component*, Component*))
{
    Component* menu = CreateComponent("PC_QUESTS_MENU");
    Component* frame = menu->FindChild("PC_UI_FRAME_QUESTS");
    ScrollActions(menu, screen, "SCROLL_LIST_UP", "SCROLL_LIST_DOWN", false);
    ScrollActions(menu, screen, "SCROLL_DESCRIPTION_UP", "SCROLL_DESCRIPTION_DOWN", false);
    ScrollActions(menu, frame, "SCROLL_LIST_UP", "SCROLL_LIST_DOWN", true);
    ScrollActions(menu, frame, "SCROLL_DESCRIPTION_UP", "SCROLL_DESCRIPTION_DOWN", true);
    Component* title = menu->FindChild("PC_TITLE_QUESTS_MENU");
    frame->Pos[frame->PosCount++] = title;
    const char* pastText = "TEXT_GUI_MENU_PAST_QUESTS";
    Ptr<Component> pastEntry;
    Ptr<Component> pastList(pastListPC(screen, menu));
    menu->Offsets = 2;
    Component* viewport = screen->FindChild("PC_DESCRIPTION_VIEWPORT");
    Ptr<Component> description;
    Ptr<Component> subtitle;
    Ptr<Component> ringPic;
    const char* curText = "TEXT_GUI_MENU_CURRENT_QUESTS";
    Ptr<Component> curEntry;
    Ptr<Component> curList(reviewListPC(screen, menu));
    if (!curList.IsNull()) {
        curEntry.Reset(CreateMenuEntry(curText, "PC_MENU_ENTRY_BRANCH_TEMPLATE"));
        curEntry->AddChild(curList);
        std::strcpy(curEntry->Name, "CURRENT_QUESTS_ENTRY");
    } else {
        curEntry.Reset(CreateMenuEntry(curText, "PC_MENU_ENTRY_TEMPLATE"));
        curEntry->Unavailable = true;
    }
    description.Reset(CreateComponent("PC_DESCRIPTION"));
    frame->Pos[frame->PosCount++] = description.Data;
    Component* descriptionText = description.Data;
    std::strcpy(descriptionText->Text, "TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS");
    viewport->Fake[viewport->FakeCount++] = descriptionText;
    curEntry->AddChild(description);
    subtitle.Reset(CreateComponent("PC_CURRENTQUESTS_SUBTITLE"));
    frame->Pos[frame->PosCount++] = subtitle.Data;
    curEntry->AddChild(subtitle);
    ringPic.Reset(CreateComponent("PC_RING_PIC_CURRENT_QUESTS"));
    frame->Pos[frame->PosCount++] = ringPic.Data;
    curEntry->AddChild(ringPic);
    if (!pastList.IsNull()) {
        pastEntry.Reset(CreateMenuEntry(pastText, "PC_MENU_ENTRY_BRANCH_TEMPLATE"));
        pastEntry->AddChild(pastList);
    } else {
        pastEntry.Reset(CreateMenuEntry(pastText, "PC_MENU_ENTRY_TEMPLATE"));
        pastEntry->Unavailable = true;
    }
    description.Reset(CreateComponent("PC_DESCRIPTION"));
    frame->Pos[frame->PosCount++] = description.Data;
    descriptionText = description.Data;
    std::strcpy(descriptionText->Text, "TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS");
    viewport->Fake[viewport->FakeCount++] = descriptionText;
    pastEntry->AddChild(description);
    subtitle.Reset(CreateComponent("PC_PASTQUESTS_SUBTITLE"));
    frame->Pos[frame->PosCount++] = subtitle.Data;
    pastEntry->AddChild(subtitle);
    ringPic.Reset(CreateComponent("PC_RING_PIC_PAST_QUESTS"));
    frame->Pos[frame->PosCount++] = ringPic.Data;
    pastEntry->AddChild(ringPic);
    menu->AddChild(curEntry);
    menu->AddChild(pastEntry);
    return menu;
}
static Component* NoList(Component*, Component*) { return 0; }
static Component* PastList(Component* screen, Component* menu) { g_pastArgsOk = screen == g_screen && !std::strcmp(menu->Name, "PC_QUESTS_MENU"); return new Component("PC_SUB_LIST_PAST"); }
static Component* ReviewList(Component* screen, Component* menu) { g_reviewArgsOk = screen == g_screen && menu->Offsets == 2; return new Component("PC_SUB_LIST_CURRENT"); }
static int Fail(int line) { std::printf("00601310_TEST FAIL (%d)\n", line); return 1; }
#define CHECK(c) do { if (!(c)) return Fail(__LINE__); } while (0)
static Component* MakeScreen()
{
    Component* screen = new Component("PC_QUESTS_SCREEN");
    screen->AddNamed("SCROLL_LIST_UP"); screen->AddNamed("SCROLL_LIST_DOWN");
    screen->AddNamed("SCROLL_DESCRIPTION_UP"); screen->AddNamed("SCROLL_DESCRIPTION_DOWN");
    screen->AddNamed("PC_DESCRIPTION_VIEWPORT");
    return screen;
}
int main()
{
    Component* screen = g_screen = MakeScreen();
    int base = g_alive;
    Component* menu = ConstructQuestsScreenPC(screen, PastList, ReviewList);
    Component* frame = menu->FindChild("PC_UI_FRAME_QUESTS");
    CHECK(!std::strcmp(menu->Name, "PC_QUESTS_MENU") && menu->Offsets == 2 && menu->ChildCount == 2);
    CHECK(g_pastArgsOk && g_reviewArgsOk);                       // past list is built before InitialiseOffsets(2), review list after
    // sixteen actions: eight on selected, eight on inactive, each kept alive only by the menu
    CHECK(menu->SelCount == 8 && menu->InaCount == 8);
    static const char* const names[4] = { "SCROLL_LIST_UP", "SCROLL_LIST_DOWN", "SCROLL_DESCRIPTION_UP", "SCROLL_DESCRIPTION_DOWN" };
    for (int i = 0; i < 8; ++i) {
        Component* owner = i < 4 ? screen : frame;
        Action* sel = menu->Selected[i].Data; Action* ina = menu->Inactive[i].Data;
        CHECK(sel->ActionType == (i < 4 ? ACTION_IGNORE_EVENT : ACTION_OBSERVE_EVENT));
        CHECK(ina->ActionType == (i < 4 ? ACTION_OBSERVE_EVENT : ACTION_IGNORE_EVENT));
        CHECK(sel->Target == owner->FindChild(names[i % 4]) && ina->Target == sel->Target && sel->Target != 0);
        CHECK(sel->Self == sel && ina->Self == ina && sel != ina);
        CHECK(sel->ParamCount == 1 && sel->Params[0] == EVENT_MOUSE_MOVEMENT && ina->ParamCount == 1 && ina->Params[0] == EVENT_MOUSE_MOVEMENT);
        CHECK(menu->Selected[i].Info_->RefCount == 1 && menu->Inactive[i].Info_->RefCount == 1);
    }
    Component* viewport = screen->FindChild("PC_DESCRIPTION_VIEWPORT");
    Component* current = menu->Children[0].Data;
    Component* past = menu->Children[1].Data;
    CHECK(!std::strcmp(current->Template, "PC_MENU_ENTRY_BRANCH_TEMPLATE") && !std::strcmp(current->Text, "TEXT_GUI_MENU_CURRENT_QUESTS"));
    CHECK(!std::strcmp(current->Name, "CURRENT_QUESTS_ENTRY") && !current->Unavailable && current->ChildCount == 4);
    CHECK(!std::strcmp(current->Children[0].Data->Name, "PC_SUB_LIST_CURRENT"));
    CHECK(!std::strcmp(current->Children[1].Data->Name, "PC_DESCRIPTION") && !std::strcmp(current->Children[1].Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_CURRENTQUESTS"));
    CHECK(!std::strcmp(current->Children[2].Data->Name, "PC_CURRENTQUESTS_SUBTITLE") && !std::strcmp(current->Children[3].Data->Name, "PC_RING_PIC_CURRENT_QUESTS"));
    CHECK(!std::strcmp(past->Template, "PC_MENU_ENTRY_BRANCH_TEMPLATE") && !std::strcmp(past->Text, "TEXT_GUI_MENU_PAST_QUESTS"));
    CHECK(!std::strcmp(past->Name, "PC_MENU_ENTRY_BRANCH_TEMPLATE") && !past->Unavailable && past->ChildCount == 4);   // no SetName for the past entry
    CHECK(!std::strcmp(past->Children[0].Data->Name, "PC_SUB_LIST_PAST") && !std::strcmp(past->Children[1].Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS"));
    CHECK(!std::strcmp(past->Children[2].Data->Name, "PC_PASTQUESTS_SUBTITLE") && !std::strcmp(past->Children[3].Data->Name, "PC_RING_PIC_PAST_QUESTS"));
    CHECK(viewport->FakeCount == 2 && viewport->Fake[0] == current->Children[1].Data && viewport->Fake[1] == past->Children[1].Data);
    // frame positions: title, then description / subtitle / ring picture of each entry
    CHECK(frame->PosCount == 7 && frame->Pos[0] == menu->FindChild("PC_TITLE_QUESTS_MENU"));
    CHECK(frame->Pos[1] == current->Children[1].Data && frame->Pos[2] == current->Children[2].Data && frame->Pos[3] == current->Children[3].Data);
    CHECK(frame->Pos[4] == past->Children[1].Data && frame->Pos[5] == past->Children[2].Data && frame->Pos[6] == past->Children[3].Data);
    CHECK(menu->Children[0].Info_->RefCount == 1 && current->Children[0].Info_->RefCount == 1 && past->Children[1].Info_->RefCount == 1);
    // menu + frame + 4 frame buttons + title, 16 actions, 2 entries, 2 lists, 2 descriptions, 2 subtitles, 2 ring pictures
    CHECK(g_alive == base + 7 + 16 + 10);
    for (int k = 0; k < 8; ++k) { menu->Selected[k].Release(); menu->Inactive[k].Release(); }
    for (int k = 0; k < menu->ChildCount; ++k) menu->Children[k].Release();
    delete menu;
    CHECK(g_alive == base);
    // no active / no completed quests: plain template, unavailable, no list child, current entry keeps the template name
    menu = ConstructQuestsScreenPC(screen, NoList, NoList);
    current = menu->Children[0].Data; past = menu->Children[1].Data;
    CHECK(!std::strcmp(current->Template, "PC_MENU_ENTRY_TEMPLATE") && current->Unavailable && current->ChildCount == 3);
    CHECK(std::strcmp(current->Name, "CURRENT_QUESTS_ENTRY") != 0);
    CHECK(!std::strcmp(past->Template, "PC_MENU_ENTRY_TEMPLATE") && past->Unavailable && past->ChildCount == 3);
    CHECK(!std::strcmp(past->Children[0].Data->Text, "TEXT_GUI_MENUGUIDE_QUESTS_PASTQUESTS") && viewport->FakeCount == 4);
    CHECK(g_alive == base + 7 + 16 + 8);
    for (int k = 0; k < 8; ++k) { menu->Selected[k].Release(); menu->Inactive[k].Release(); }
    for (int k = 0; k < menu->ChildCount; ++k) menu->Children[k].Release();
    delete menu;
    // only the review list present
    menu = ConstructQuestsScreenPC(screen, NoList, ReviewList);
    current = menu->Children[0].Data; past = menu->Children[1].Data;
    CHECK(!current->Unavailable && current->ChildCount == 4 && past->Unavailable && past->ChildCount == 3);
    for (int k = 0; k < 8; ++k) { menu->Selected[k].Release(); menu->Inactive[k].Release(); }
    for (int k = 0; k < menu->ChildCount; ++k) menu->Children[k].Release();
    delete menu;
    delete screen;
    CHECK(g_alive == 0);
    std::printf("00601310_TEST PASS\n");
    return 0;
}
