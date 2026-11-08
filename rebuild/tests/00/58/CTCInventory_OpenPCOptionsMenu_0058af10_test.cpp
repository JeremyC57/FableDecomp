// Behaviour test for CTCInventory::OpenPCOptionsMenu (0x0058AF10): the same control flow as the source over a small
// model of the NUISystem component tree (named components with children, states, actions and observers), checked on a
// freshly built options list and on a list that is still open.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <string>
#include <vector>
#include <list>

enum { UI_TYPE_GENERIC = 0, UI_TYPE_TEXT = 6 };
enum { UI_EVENT_SELECT = 4, UI_EVENT_BACK = 5, UI_EVENT_SLIDER_LEFT = 20, UI_EVENT_SLIDER_RIGHT = 21, UI_EVENT_MOUSE_MOVEMENT = 25 };
enum {
    UI_ACTION_TYPE_SEND_OWNED_EVENT = 230, UI_ACTION_TYPE_SEND_EVENT = 231, UI_ACTION_TYPE_OBSERVE_EVENT = 245,
    UI_ACTION_TYPE_IGNORE_EVENT = 249, UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UNTIL_CHILD_CONTAINING = 272,
    UI_ACTION_TYPE_RESTORE_DEFAULTS_GAMEPLAY = 324, UI_ACTION_TYPE_RESTORE_DEFAULTS_VIDEO = 325,
    UI_ACTION_TYPE_RESTORE_DEFAULTS_AUDIO = 326, UI_ACTION_TYPE_DEACTIVATE_EDITBOX = 3019, UI_ACTION_TYPE_SET_EDITBOX_VALUES = 3020
};

struct Component;
struct Action {
    long Type;
    std::vector<Component*> Components;   // AddParam(component)
    std::vector<long> Numbers;            // SLONG / ULONG / EBOOL params
};

struct Component {
    std::string Name;
    int Type;
    Component* Parent;
    std::vector<Component*> Children;
    std::map<unsigned long, unsigned char> StateAlpha;
    unsigned char Alpha;
    bool PreventObserving, Unavailable, Initialised;
    unsigned long State;
    std::vector<unsigned long> Observed;
    std::vector<Action> OnSelected, OnInactive, OnHovered, OnLeftUnclicked;
    std::vector<Component*> PositionChildren, FakeChildren;
    std::wstring Value;
    float MinX, MaxX;          // CSlider
    long MaxChild;             // CTextSlider
    Component* AcceptButton;   // SetPAcceptButton / CRedefinerList::SetAcceptButton
    Component(const std::string& name, int type = UI_TYPE_GENERIC)
        : Name(name), Type(type), Parent(0), Alpha(255), PreventObserving(false), Unavailable(false), Initialised(false),
          State(0), MinX(0), MaxX(1), MaxChild(1), AcceptButton(0) { StateAlpha[0] = 200; StateAlpha[1] = 100; }
    Component* Add(Component* c) { c->Parent = this; Children.push_back(c); return c; }
    Component* FindChild(const std::string& name) {
        for (size_t i = 0; i < Children.size(); ++i) {
            if (Children[i]->Name == name) return Children[i];
            Component* found = Children[i]->FindChild(name);
            if (found) return found;
        }
        return 0;
    }
    void FindAllChildren(const std::string& name, std::list<Component*>& out) {
        for (size_t i = 0; i < Children.size(); ++i) {
            if (Children[i]->Name == name) out.push_back(Children[i]);
            Children[i]->FindAllChildren(name, out);
        }
    }
    void FindAllChildrenOfType(int type, std::list<Component*>& out) {
        for (size_t i = 0; i < Children.size(); ++i) {
            if (Children[i]->Type == type) out.push_back(Children[i]);
            Children[i]->FindAllChildrenOfType(type, out);
        }
    }
    Component* FindFirstChildOfTypeNonRecursive(int type) {
        for (size_t i = 0; i < Children.size(); ++i) if (Children[i]->Type == type) return Children[i];
        return 0;
    }
};

static std::vector<Component*> g_all;
static Component* New(const std::string& name, int type = UI_TYPE_GENERIC) { Component* c = new Component(name, type); g_all.push_back(c); return c; }

struct Manager {
    std::vector<Component*> Observers;
    long Created;
    Manager() : Created(0) {}
    void AddObserver(Component* c) { Observers.push_back(c); }
    Component* CreateComponent(const std::string& name);
};

static Component* Page(const std::string& name, bool redefiner)
{
    Component* page = New(name);
    page->Add(New("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER"))->Add(New("PC_OPTIONS_ACCEPT_CHANGES"));
    page->Add(New("PC_OPTIONS_BACK_CONTAINER"))->Add(New("PC_OPTIONS_BACK"));
    page->Add(New("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER"))->Add(New("PC_OPTIONS_RESTORE_DEFAULTS"));
    if (redefiner) page->Add(New("PC_UI_REDEFINER_LIST"));
    return page;
}

// a slider entry: whole -> { left arrow, right arrow, label text, slider }; a (numeric) detail slider sits one container deeper
static Component* Whole(Component* page, const std::string& whole, const std::string& slider, float minX, float maxX, long maxChild, bool nested = false)
{
    Component* w = page->Add(New(whole));
    w->Add(New("PC_OPTIONS_LEFT_ARROW"));
    w->Add(New("PC_OPTIONS_RIGHT_ARROW"));
    w->Add(New(whole + "_LABEL", UI_TYPE_TEXT));
    Component* s = (nested ? w->Add(New(whole + "_BAR")) : w)->Add(New(slider));
    s->MinX = minX; s->MaxX = maxX; s->MaxChild = maxChild;
    return s;
}

Component* Manager::CreateComponent(const std::string& name)
{
    ++Created;
    if (name == "PC_OPTIONS_TEXT_IN_TEXT_SLIDER_TEMPLATE") return New(name, UI_TYPE_TEXT);
    Component* list = New(name);
    Component* frame = list->Add(New("PC_UI_FADING_FRAME"));
    frame->Add(New("PC_BACK_CLICKABLE"));
    frame->Add(New("PC_CLOSE_CLICKABLE"));
    list->Add(New("PC_DESCRIPTION_VIEWPORT"));
    list->Add(New("PC_TITLE_OPTIONS"));
    list->Add(New("PC_SAVE_DESC"));
    list->Add(New("PC_RING_PIC_LOAD"));
    Component* quit = list->Add(New("PC_MENU_ENTRY_QUIT"));
    quit->Add(New("PC_RING_PIC_QUIT"));
    quit->Add(New("PC_RING_PIC_QUIT"));
    quit->Add(New("PC_QUIT_DESC"));
    Component* gameplay = list->Add(Page("PC_GAMEPLAY_PREFS_LIST", false));
    list->Add(Page("PC_AUDIO_PREFS_LIST", false));
    Component* video = list->Add(Page("PC_VIDEO_PREFS_LIST", false));
    list->Add(Page("PC_REDEFINE_KEYS_LIST", true));
    Whole(gameplay, "PC_OPTIONS_TEXT_SLIDER_WHOLE_CAMERA_UP_DOWN", "PC_OPTIONS_CAMERA_UP_DOWN_TEXT_SLIDER", 0, 1, 1);
    Whole(video, "PC_OPTIONS_TEXT_SLIDER_WHOLE_RESOLUTION", "PC_OPTIONS_RESOLUTION_TEXT_SLIDER", 0, 1, 1);
    Whole(video, "PC_OPTIONS_TEXT_SLIDER_WHOLE_REFRESH_RATE", "PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER", 0, 1, 0);
    Whole(video, "PC_OPTIONS_TEXT_SLIDER_WHOLE_ANTIALIASING", "PC_OPTIONS_ANTIALIASING_TEXT_SLIDER", 0, 1, 1);
    Whole(video, "PC_OPTIONS_SLIDER_WHOLE_TEXTURE_DETAIL", "PC_SLIDER_TEXTURE_DETAIL", 1, 1, 1, true);   // no range: dimmed
    Whole(video, "PC_OPTIONS_SLIDER_WHOLE_MESH_DETAIL", "PC_SLIDER_MESH_DETAIL", 0, 2, 1, true);
    Component* refresh = list->FindChild("PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER");
    refresh->Add(New("RATE_60", UI_TYPE_TEXT))->Value = L"60";
    refresh->Add(New("RATE_75", UI_TYPE_TEXT))->Value = L"75";
    list->Add(New("SCROLL_LIST_UP"));
    list->Add(New("SCROLL_DESCRIPTION_DOWN"));
    return list;
}

struct DisplayMode { long X, Y, BitsPerPixel, RefreshRate; };

struct World {
    Manager UI;
    std::vector<Component*> Components;
    long CurrentOpenMenu;
    long DeleteCalls;
    Component* MousePointer;
    Component* InGameMenuMouse;
    Component* InGameMenuList;
    std::vector<DisplayMode> Modes;
    std::vector<long> AntiAliasing;
    World() : CurrentOpenMenu(0), DeleteCalls(0), MousePointer(New("MOUSE")), InGameMenuMouse(0), InGameMenuList(0) {}
};

static std::wstring IntToWide(long v) { char b[32]; std::sprintf(b, "%ld", v); std::wstring w; for (char* p = b; *p; ++p) w += (wchar_t)*p; return w; }
static Action MakeAction(long type) { Action a; a.Type = type; a.Numbers.clear(); return a; }

static void DimArrows(Component* whole)
{
    static const char* const arrows[2] = { "PC_OPTIONS_LEFT_ARROW", "PC_OPTIONS_RIGHT_ARROW" };
    for (int k = 0; k < 2; ++k) {
        Component* arrow = whole->FindChild(arrows[k]);
        if (arrow) {
            arrow->PreventObserving = true;
            for (std::map<unsigned long, unsigned char>::iterator s = arrow->StateAlpha.begin(); s != arrow->StateAlpha.end(); ++s)
                s->second /= 2;
            unsigned char alpha = arrow->Alpha;
            alpha /= 2;
            arrow->Alpha = alpha;
        }
    }
    Component* text = whole->FindFirstChildOfTypeNonRecursive(UI_TYPE_TEXT);
    if (text) text->Unavailable = true;
}

static void PageActions(Component* page, long restoreType)
{
    Component* accept = page->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER")->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
    Component* back = page->FindChild("PC_OPTIONS_BACK_CONTAINER")->FindChild("PC_OPTIONS_BACK");
    Component* restore = page->FindChild("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER")->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
    Action observeAccept = MakeAction(UI_ACTION_TYPE_OBSERVE_EVENT);   // built and never attached, as retail
    observeAccept.Components.push_back(accept); observeAccept.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT);
    Component* observed[2] = { back, restore };
    for (int k = 0; k < 2; ++k) {
        Action a = MakeAction(UI_ACTION_TYPE_OBSERVE_EVENT); a.Components.push_back(observed[k]); a.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT);
        page->OnSelected.push_back(a);
    }
    Component* ignored[3] = { accept, back, restore };
    for (int k = 0; k < 3; ++k) {
        Action a = MakeAction(UI_ACTION_TYPE_IGNORE_EVENT); a.Components.push_back(ignored[k]); a.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT);
        page->OnInactive.push_back(a);
    }
    Action sendSelect = MakeAction(UI_ACTION_TYPE_SEND_EVENT); sendSelect.Numbers.push_back(UI_EVENT_SELECT); accept->OnLeftUnclicked.push_back(sendSelect);
    Action sendBack = MakeAction(UI_ACTION_TYPE_SEND_EVENT); sendBack.Numbers.push_back(UI_EVENT_BACK); back->OnLeftUnclicked.push_back(sendBack);
    restore->OnLeftUnclicked.push_back(MakeAction(restoreType));
}

static void OpenPCOptionsMenu(World& w)
{
    ++w.DeleteCalls;
    w.CurrentOpenMenu = 4;
    Component* list = 0;
    for (unsigned long i = 0; i < w.Components.size(); ++i) {
        if (list) break;
        if (w.Components[i]->Name == "PC_OPTIONS_LIST") list = w.Components[i];
    }
    if (!list) {
        list = w.UI.CreateComponent("PC_OPTIONS_LIST");
        Component* frame = list->FindChild("PC_UI_FADING_FRAME");
        Component* description = list->FindChild("PC_DESCRIPTION_VIEWPORT");
        Component* viewport = description;
        if (frame) {
            const char* clickables[2] = { "PC_BACK_CLICKABLE", "PC_CLOSE_CLICKABLE" };
            for (int k = 0; k < 2; ++k) { Component* c = frame->FindChild(clickables[k]); w.UI.AddObserver(c); c->Observed.push_back(UI_EVENT_MOUSE_MOVEMENT); }
            static const char* const children[] = { "PC_TITLE_OPTIONS", "PC_RING_PIC_SAVE", "PC_SAVE_DESC", "PC_SAVE_SUB", "PC_RING_PIC_LOAD",
                "PC_LOAD_DESC", "PC_LOAD_SUB", "PC_RING_SCOREBOARD_PREFS", "PC_SCOREBOARD_PREFS_DESC", "PC_SCOREBOARD_PREFS_TITLE" };
            for (size_t k = 0; k < sizeof(children) / sizeof(children[0]); ++k) {
                Component* child = list->FindChild(children[k]);
                if (child) {
                    frame->PositionChildren.push_back(child);
                    if (std::strstr(children[k], "_DESC") && viewport) viewport->FakeChildren.push_back(child);
                }
            }
            if (description) frame->PositionChildren.push_back(description);
            Component* quit = list->FindChild("PC_MENU_ENTRY_QUIT");
            std::list<Component*> pictures;
            quit->FindAllChildren("PC_RING_PIC_QUIT", pictures);
            for (std::list<Component*>::iterator it = pictures.begin(), last = pictures.end(); it != last; ++it) frame->PositionChildren.push_back(*it);
            Component* child = quit->FindChild("PC_QUIT_DESC");
            if (child) { frame->PositionChildren.push_back(child); if (viewport) viewport->FakeChildren.push_back(child); }
            child = quit->FindChild("PC_QUIT_SUB");
            if (child) frame->PositionChildren.push_back(child);
        }
        w.InGameMenuMouse = w.MousePointer; w.InGameMenuList = list;

        std::list<Component*> pages;
        Component* gameplay = list->FindChild("PC_GAMEPLAY_PREFS_LIST");
        Component* audio = list->FindChild("PC_AUDIO_PREFS_LIST");
        Component* video = list->FindChild("PC_VIDEO_PREFS_LIST");
        Component* redefine = list->FindChild("PC_REDEFINE_KEYS_LIST");
        PageActions(gameplay, UI_ACTION_TYPE_RESTORE_DEFAULTS_GAMEPLAY);
        PageActions(audio, UI_ACTION_TYPE_RESTORE_DEFAULTS_AUDIO);
        PageActions(video, UI_ACTION_TYPE_RESTORE_DEFAULTS_VIDEO);
        {
            Component* accept = redefine->FindChild("PC_OPTIONS_ACCEPT_CHANGES_CONTAINER")->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
            Component* back = redefine->FindChild("PC_OPTIONS_BACK_CONTAINER")->FindChild("PC_OPTIONS_BACK");
            Component* restore = redefine->FindChild("PC_OPTIONS_RESTORE_DEFAULTS_CONTAINER")->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
            restore->Alpha = 0;
            for (std::map<unsigned long, unsigned char>::iterator s = restore->StateAlpha.begin(); s != restore->StateAlpha.end(); ++s) s->second = 0;
            redefine->OnSelected.push_back(MakeAction(UI_ACTION_TYPE_SET_EDITBOX_VALUES));
            Action observeBack = MakeAction(UI_ACTION_TYPE_OBSERVE_EVENT); observeBack.Components.push_back(back); observeBack.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT);
            redefine->OnSelected.push_back(observeBack);
            Component* ignored[2] = { accept, back };
            for (int k = 0; k < 2; ++k) {
                Action a = MakeAction(UI_ACTION_TYPE_IGNORE_EVENT); a.Components.push_back(ignored[k]); a.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT);
                redefine->OnInactive.push_back(a);
            }
            Action deactivate = MakeAction(UI_ACTION_TYPE_DEACTIVATE_EDITBOX); deactivate.Numbers.push_back(1); accept->OnLeftUnclicked.push_back(deactivate);
            Action sendSelect = MakeAction(UI_ACTION_TYPE_SEND_EVENT); sendSelect.Numbers.push_back(UI_EVENT_SELECT); accept->OnLeftUnclicked.push_back(sendSelect);
            back->OnLeftUnclicked.push_back(MakeAction(UI_ACTION_TYPE_DEACTIVATE_EDITBOX));
            Action sendBack = MakeAction(UI_ACTION_TYPE_SEND_EVENT); sendBack.Numbers.push_back(UI_EVENT_BACK); back->OnLeftUnclicked.push_back(sendBack);
            redefine->FindChild("PC_UI_REDEFINER_LIST")->AcceptButton = accept;
        }
        for (int k = 0; k < 10; ++k) pages.push_back(gameplay);
        for (int k = 0; k < 3; ++k) pages.push_back(audio);
        for (int k = 0; k < 10; ++k) pages.push_back(video);

        static const char* const names[23][2] = {
            { "PC_OPTIONS_TEXT_SLIDER_WHOLE_CAMERA_UP_DOWN", "PC_OPTIONS_CAMERA_UP_DOWN_TEXT_SLIDER" }, { "S2", "s2" }, { "S3", "s3" },
            { "S4", "s4" }, { "S5", "s5" }, { "S6", "s6" }, { "S7", "s7" }, { "S8", "s8" }, { "S9", "s9" }, { "S10", "s10" },
            { "S11", "s11" }, { "S12", "s12" }, { "S13", "s13" },
            { "PC_OPTIONS_TEXT_SLIDER_WHOLE_RESOLUTION", "PC_OPTIONS_RESOLUTION_TEXT_SLIDER" },
            { "PC_OPTIONS_TEXT_SLIDER_WHOLE_REFRESH_RATE", "PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER" },
            { "PC_OPTIONS_TEXT_SLIDER_WHOLE_ANTIALIASING", "PC_OPTIONS_ANTIALIASING_TEXT_SLIDER" },
            { "PC_OPTIONS_SLIDER_WHOLE_TEXTURE_DETAIL", "PC_SLIDER_TEXTURE_DETAIL" }, { "PC_OPTIONS_SLIDER_WHOLE_MESH_DETAIL", "PC_SLIDER_MESH_DETAIL" },
            { "S19", "s19" }, { "S20", "s20" }, { "S21", "s21" }, { "S22", "s22" }, { "S23", "s23" } };
        std::list<std::pair<Component*, Component*> > sliders;
        for (int k = 0; k < 23; ++k) {
            Component* whole = list->FindChild(names[k][0]);
            if (whole) { Component* slider = whole->FindChild(names[k][1]); if (slider) sliders.push_back(std::make_pair(whole, slider)); }
        }
        // the page list is walked in step with the FOUND sliders (retail pairs them by position, not by slider name)
        std::list<std::pair<Component*, Component*> >::iterator entry = sliders.begin();
        for (std::list<Component*>::iterator page = pages.begin(); page != pages.end() && entry != sliders.end(); ++entry, ++page) {
            Component* sliderPage = *page;
            if (!sliderPage || !entry->first || !entry->second) continue;
            Component* whole = entry->first;
            Component* slider = entry->second;
            Component* accept = sliderPage->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
            if (accept) slider->AcceptButton = accept;
            static const char* const arrows[2] = { "PC_OPTIONS_LEFT_ARROW", "PC_OPTIONS_RIGHT_ARROW" };
            for (int k = 0; k < 2; ++k) {
                Component* arrow = whole->FindChild(arrows[k]);
                if (!arrow) continue;
                Action scrollTo = MakeAction(UI_ACTION_TYPE_SCROLL_ACTIVE_LIST_UNTIL_CHILD_CONTAINING); scrollTo.Components.push_back(whole); arrow->OnHovered.push_back(scrollTo);
                Action slide = MakeAction(UI_ACTION_TYPE_SEND_OWNED_EVENT); slide.Numbers.push_back(k ? UI_EVENT_SLIDER_RIGHT : UI_EVENT_SLIDER_LEFT); slide.Components.push_back(slider);
                arrow->OnLeftUnclicked.push_back(slide);
                Action observe = MakeAction(UI_ACTION_TYPE_OBSERVE_EVENT); observe.Components.push_back(arrow); observe.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT); sliderPage->OnSelected.push_back(observe);
                Action ignore = MakeAction(UI_ACTION_TYPE_IGNORE_EVENT); ignore.Components.push_back(arrow); ignore.Numbers.push_back(UI_EVENT_MOUSE_MOVEMENT); sliderPage->OnInactive.push_back(ignore);
            }
        }
        Component* resolution = list->FindChild("PC_OPTIONS_RESOLUTION_TEXT_SLIDER");
        for (size_t m = 0; m < w.Modes.size(); ++m) {
            Component* text = w.UI.CreateComponent("PC_OPTIONS_TEXT_IN_TEXT_SLIDER_TEMPLATE");
            if (text) {
                text->Value = IntToWide(w.Modes[m].X) + L"X" + IntToWide(w.Modes[m].Y) + L"X" + IntToWide(w.Modes[m].BitsPerPixel);
                resolution->Add(text); ++resolution->MaxChild;
            }
        }
        Component* antialiasing = list->FindChild("PC_OPTIONS_ANTIALIASING_TEXT_SLIDER");
        for (size_t m = 0; m < w.AntiAliasing.size(); ++m) {
            Component* text = w.UI.CreateComponent("PC_OPTIONS_TEXT_IN_TEXT_SLIDER_TEMPLATE");
            if (text) {
                long level = w.AntiAliasing[m];
                if (level >= 2 && level <= 16) text->Value = IntToWide(level) + L"X";
                else text->Value = L"TEXT_GUI_MENU_OFF";
                antialiasing->Add(text); ++antialiasing->MaxChild;
            }
        }
        list->Initialised = true;
        w.Components.push_back(list);
    }
    static const char* const scrolls[4] = { "SCROLL_LIST_UP", "SCROLL_DESCRIPTION_UP", "SCROLL_LIST_DOWN", "SCROLL_DESCRIPTION_DOWN" };
    for (int k = 0; k < 4; ++k) {
        Component* scroll = list->FindChild(scrolls[k]);
        if (scroll) { w.UI.AddObserver(scroll); scroll->Observed.push_back(UI_EVENT_MOUSE_MOVEMENT); }
    }
    static const char* const details[4] = { "PC_SLIDER_TEXTURE_DETAIL", "PC_SLIDER_MESH_DETAIL", "PC_SLIDER_EFFECTS_DETAIL", "PC_SLIDER_SHADOW_DETAIL" };
    for (int k = 0; k < 4; ++k) {
        Component* slider = list->FindChild(details[k]);
        if (slider && slider->MinX == slider->MaxX && slider->Parent && slider->Parent->Parent) DimArrows(slider->Parent->Parent);
    }
    static const char* const texts[3] = { "PC_OPTIONS_RESOLUTION_TEXT_SLIDER", "PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER", "PC_OPTIONS_ANTIALIASING_TEXT_SLIDER" };
    for (int k = 0; k < 3; ++k) {
        Component* slider = list->FindChild(texts[k]);
        if (slider && slider->MaxChild == 0 && slider->Parent) DimArrows(slider->Parent);
    }
    std::list<Component*> rates;
    list->FindChild("PC_OPTIONS_REFRESH_RATE_TEXT_SLIDER")->FindAllChildrenOfType(UI_TYPE_TEXT, rates);
    for (std::list<Component*>::iterator rate = rates.begin(); rate != rates.end(); ++rate)
        if (*rate) (*rate)->Value = (*rate)->Value + L" " + L"Hz";
    list->State = 5;
}

static int g_failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("check failed: %s\n", what); ++g_failures; } }

int main()
{
    World w;
    DisplayMode a = { 800, 600, 32, 60 }; DisplayMode b = { 1024, 768, 16, 75 };
    w.Modes.push_back(a); w.Modes.push_back(b);
    w.AntiAliasing.push_back(0); w.AntiAliasing.push_back(2); w.AntiAliasing.push_back(4); w.AntiAliasing.push_back(1);
    OpenPCOptionsMenu(w);

    Check(w.CurrentOpenMenu == 4 && w.DeleteCalls == 1, "menu state");
    Check(w.Components.size() == 1 && w.Components[0]->Name == "PC_OPTIONS_LIST", "list registered");
    Component* list = w.Components[0];
    Check(list->Initialised && list->State == 5, "initialised, state 5");
    Check(w.InGameMenuList == list && w.InGameMenuMouse == w.MousePointer, "in-game menu PC entries");
    Component* frame = list->FindChild("PC_UI_FADING_FRAME");
    Component* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");
    // title, save desc, load picture, description viewport, two quit pictures, quit desc
    Check(frame->PositionChildren.size() == 7, "frame position children");
    Check(viewport->FakeChildren.size() == 2 && viewport->FakeChildren[1]->Name == "PC_QUIT_DESC", "viewport fake children");
    Check(frame->FindChild("PC_BACK_CLICKABLE")->Observed.size() == 1, "back clickable observed");

    Component* gameplay = list->FindChild("PC_GAMEPLAY_PREFS_LIST");
    Component* gAccept = gameplay->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
    Check(gAccept->OnLeftUnclicked.size() == 1 && gAccept->OnLeftUnclicked[0].Numbers[0] == UI_EVENT_SELECT, "gameplay accept sends select");
    Check(gameplay->FindChild("PC_OPTIONS_RESTORE_DEFAULTS")->OnLeftUnclicked[0].Type == UI_ACTION_TYPE_RESTORE_DEFAULTS_GAMEPLAY, "gameplay restore");
    Check(list->FindChild("PC_AUDIO_PREFS_LIST")->FindChild("PC_OPTIONS_RESTORE_DEFAULTS")->OnLeftUnclicked[0].Type == UI_ACTION_TYPE_RESTORE_DEFAULTS_AUDIO, "audio restore");
    // back + restore observed, three ignored, plus two arrows for each of the six sliders found: the page list is
    // consumed in step with the FOUND sliders, so all six pair with the first (gameplay) pages
    Check(gameplay->OnSelected.size() == 14 && gameplay->OnInactive.size() == 15, "gameplay page actions");
    Check(list->FindChild("PC_VIDEO_PREFS_LIST")->OnSelected.size() == 2, "video page actions");
    Check(gameplay->OnSelected[0].Components[0]->Name == "PC_OPTIONS_BACK", "observe back first");

    Component* redefine = list->FindChild("PC_REDEFINE_KEYS_LIST");
    Component* rAccept = redefine->FindChild("PC_OPTIONS_ACCEPT_CHANGES");
    Component* rRestore = redefine->FindChild("PC_OPTIONS_RESTORE_DEFAULTS");
    Check(rRestore->Alpha == 0 && rRestore->StateAlpha[0] == 0 && rRestore->StateAlpha[1] == 0, "redefine restore hidden");
    Check(redefine->OnSelected.size() == 2 && redefine->OnSelected[0].Type == UI_ACTION_TYPE_SET_EDITBOX_VALUES, "redefine selected actions");
    Check(rAccept->OnLeftUnclicked.size() == 2 && rAccept->OnLeftUnclicked[0].Type == UI_ACTION_TYPE_DEACTIVATE_EDITBOX
          && rAccept->OnLeftUnclicked[0].Numbers[0] == 1, "redefine accept deactivates the edit box");
    Check(redefine->FindChild("PC_UI_REDEFINER_LIST")->AcceptButton == rAccept, "redefiner accept button");

    Component* camera = list->FindChild("PC_OPTIONS_CAMERA_UP_DOWN_TEXT_SLIDER");
    Component* cameraWhole = camera->Parent;
    Component* left = cameraWhole->FindChild("PC_OPTIONS_LEFT_ARROW");
    Check(camera->AcceptButton == gAccept, "slider accept button");
    Check(left->OnHovered.size() == 1 && left->OnHovered[0].Components[0] == cameraWhole, "arrow scrolls to its slider");
    Check(left->OnLeftUnclicked[0].Type == UI_ACTION_TYPE_SEND_OWNED_EVENT && left->OnLeftUnclicked[0].Numbers[0] == UI_EVENT_SLIDER_LEFT
          && left->OnLeftUnclicked[0].Components[0] == camera, "left arrow slides");

    Component* resolution = list->FindChild("PC_OPTIONS_RESOLUTION_TEXT_SLIDER");
    Check(resolution->Children.size() == 2 && resolution->Children[0]->Value == L"800X600X32" && resolution->Children[1]->Value == L"1024X768X16", "resolution entries");
    Component* aa = list->FindChild("PC_OPTIONS_ANTIALIASING_TEXT_SLIDER");
    Check(aa->Children.size() == 4 && aa->Children[0]->Value == L"TEXT_GUI_MENU_OFF" && aa->Children[1]->Value == L"2X"
          && aa->Children[2]->Value == L"4X" && aa->Children[3]->Value == L"TEXT_GUI_MENU_OFF", "anti-aliasing entries");

    Component* texture = list->FindChild("PC_OPTIONS_SLIDER_WHOLE_TEXTURE_DETAIL");
    Component* tLeft = texture->FindChild("PC_OPTIONS_LEFT_ARROW");
    Check(tLeft->PreventObserving && tLeft->Alpha == 127 && tLeft->StateAlpha[0] == 100 && tLeft->StateAlpha[1] == 50, "no-range detail slider dimmed");
    Check(texture->FindFirstChildOfTypeNonRecursive(UI_TYPE_TEXT)->Unavailable, "detail label unavailable");
    Check(!list->FindChild("PC_OPTIONS_SLIDER_WHOLE_MESH_DETAIL")->FindChild("PC_OPTIONS_LEFT_ARROW")->PreventObserving, "ranged slider untouched");
    Component* refreshWhole = list->FindChild("PC_OPTIONS_TEXT_SLIDER_WHOLE_REFRESH_RATE");
    Check(refreshWhole->FindChild("PC_OPTIONS_RIGHT_ARROW")->PreventObserving, "single-entry text slider dimmed");
    Check(list->FindChild("RATE_60")->Value == L"60 Hz" && list->FindChild("RATE_75")->Value == L"75 Hz", "refresh rates suffixed");
    Check(list->FindChild("SCROLL_LIST_UP")->Observed.size() == 1, "scroll button observed");

    // a second open finds the list still registered and only re-runs the tail
    long created = w.UI.Created;
    OpenPCOptionsMenu(w);
    Check(w.UI.Created == created && w.Components.size() == 1 && w.DeleteCalls == 2, "open list reused");
    Check(list->FindChild("RATE_60")->Value == L"60 Hz Hz" && list->FindChild("SCROLL_LIST_UP")->Observed.size() == 2, "tail re-run on reuse");

    for (size_t i = 0; i < g_all.size(); ++i) delete g_all[i];
    if (g_failures) { std::printf("0058af10_TEST FAIL\n"); return 1; }
    std::printf("0058af10_TEST PASS\n");
    return 0;
}