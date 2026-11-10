// Behaviour test for 0x00587BFF CTCInventory::OpenPCSkillsMenu(): the menu state becomes 1; an open PC_SKILLS_LIST component is
// reused, else one is built (frame children positioned only when the frame exists, descriptions also added to the viewport when it
// exists, the viewport itself positioned last; the expressions / magic entries get a pre-action of type 235 / 243 whose two params
// are the entry and the list) and appended to the menu components; then the four scroll buttons observe event 25, the back / close
// clickables are added as manager observers and observe event 25, and the list goes to state 5.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name;
    std::map<std::string, Comp*> children;
    std::vector<Comp*> positioned, fake;
    std::vector<int> observed;
    std::vector<std::pair<int, std::pair<Comp*, Comp*> > > preActions;
    int state; bool initialised; bool managerObserver;
    Comp(const char* n) : name(n), state(0), initialised(false), managerObserver(false) {}
    Comp* FindChild(const char* n) { std::map<std::string, Comp*>::iterator it = children.find(n); return it == children.end() ? 0 : it->second; }
};
struct Inventory {
    int menu; std::vector<Comp*> components; Comp* built; int created;
    void OpenPCSkillsMenu()
    {
        menu = 1;
        Comp* list = 0;
        for (unsigned long i = 0; i < components.size(); ++i) {
            if (list) break;
            if (components[i]->name == "PC_SKILLS_LIST") list = components[i];
        }
        if (!list) {
            list = built; ++created;
            Comp* frame = list->FindChild("PC_UI_FRAME");
            Comp* description = list->FindChild("PC_DESCRIPTION_VIEWPORT");
            Comp* viewport = description;
            if (frame) {
                static const char* const names[] = { "PC_TITLE_SKILLS", "PC_RING_PIC_MAGIC", "PC_DESCRIPTION_MAGIC", "PC_SUBTITLE_MAGIC",
                    "PC_RING_PIC_EXPRESSIONS", "PC_DESCRIPTION_EXPRESSIONS", "PC_SUBTITLE_EXPRESSIONS" };
                for (int k = 0; k < 7; ++k) {
                    Comp* c = list->FindChild(names[k]);
                    if (c) {
                        frame->positioned.push_back(c);
                        if ((k == 2 || k == 5) && viewport) viewport->fake.push_back(c);
                    }
                }
                if (description) frame->positioned.push_back(description);
            }
            Comp* expressions = list->FindChild("PC_MENU_ENTRY_EXPRESSIONS");
            Comp* magic = list->FindChild("PC_MENU_ENTRY_MAGIC");
            if (expressions) expressions->preActions.push_back(std::make_pair(235, std::make_pair(expressions, list)));
            if (magic) magic->preActions.push_back(std::make_pair(243, std::make_pair(magic, list)));
            list->initialised = true;
            components.push_back(list);
        }
        static const char* const scrolls[] = { "SCROLL_LIST_UP", "SCROLL_DESCRIPTION_UP", "SCROLL_LIST_DOWN", "SCROLL_DESCRIPTION_DOWN" };
        for (int k = 0; k < 4; ++k) { Comp* s = list->FindChild(scrolls[k]); if (s) s->observed.push_back(25); }
        static const char* const clicks[] = { "PC_BACK_CLICKABLE", "PC_CLOSE_CLICKABLE" };
        for (int k = 0; k < 2; ++k) { Comp* c = list->FindChild(clicks[k]); if (c) { c->managerObserver = true; c->observed.push_back(25); } }
        list->state = 5;
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
int main()
{
    {   // build: frame and viewport present, magic entry missing
        Comp list("PC_SKILLS_LIST"), frame("PC_UI_FRAME"), view("PC_DESCRIPTION_VIEWPORT"), dm("PC_DESCRIPTION_MAGIC"), title("PC_TITLE_SKILLS"),
             ex("PC_MENU_ENTRY_EXPRESSIONS"), up("SCROLL_LIST_UP"), back("PC_BACK_CLICKABLE");
        list.children["PC_UI_FRAME"] = &frame; list.children["PC_DESCRIPTION_VIEWPORT"] = &view; list.children["PC_DESCRIPTION_MAGIC"] = &dm;
        list.children["PC_TITLE_SKILLS"] = &title; list.children["PC_MENU_ENTRY_EXPRESSIONS"] = &ex; list.children["SCROLL_LIST_UP"] = &up;
        list.children["PC_BACK_CLICKABLE"] = &back;
        Comp other("OTHER");
        Inventory inv; inv.menu = 0; inv.built = &list; inv.created = 0; inv.components.push_back(&other);
        inv.OpenPCSkillsMenu();
        check(inv.menu == 1 && inv.created == 1 && inv.components.size() == 2 && list.initialised, "built and kept");
        check(frame.positioned.size() == 3 && frame.positioned[0] == &title && frame.positioned[1] == &dm && frame.positioned[2] == &view, "frame children");
        check(view.fake.size() == 1 && view.fake[0] == &dm, "viewport child");
        check(ex.preActions.size() == 1 && ex.preActions[0].first == 235 && ex.preActions[0].second.second == &list, "expressions action");
        check(up.observed.size() == 1 && back.managerObserver && back.observed.size() == 1 && list.state == 5, "observers and state");
        inv.OpenPCSkillsMenu();   // second open reuses the list
        check(inv.created == 1 && inv.components.size() == 2 && up.observed.size() == 2, "reused");
    }
    {   // build without a frame: nothing positioned, actions still added
        Comp list("PC_SKILLS_LIST"), mg("PC_MENU_ENTRY_MAGIC"), dm("PC_DESCRIPTION_MAGIC");
        list.children["PC_MENU_ENTRY_MAGIC"] = &mg; list.children["PC_DESCRIPTION_MAGIC"] = &dm;
        Inventory inv; inv.menu = 0; inv.built = &list; inv.created = 0;
        inv.OpenPCSkillsMenu();
        check(mg.preActions.size() == 1 && mg.preActions[0].first == 243 && mg.preActions[0].second.first == &mg, "magic action");
        check(dm.positioned.empty() && list.state == 5, "no frame");
    }
    if (fails) return 1;
    std::printf("00587bff_TEST PASS\n");
    return 0;
}