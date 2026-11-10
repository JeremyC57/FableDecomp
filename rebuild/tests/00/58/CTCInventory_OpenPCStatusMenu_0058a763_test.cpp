// Behaviour test for 0x0058A763 CTCInventory::OpenPCStatusMenu(): menu state 7; an open PC_STATUS_LIST is reused, else one is built:
// the fading frame (when present) positions the 13 named children that exist and the viewport, the three descriptions also going
// into the viewport; quests / logbook entries get pre-actions 257 / 256; experience / personality get 282 / 289 when the hero has
// inventory stats, else they are made unavailable and their right selection arrow (when present) gets a delete-this-instance
// deletion; the list is initialised and kept. Then scroll buttons observe event 25, back / close become manager observers, state 5.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name;
    std::map<std::string, Comp*> children;
    std::vector<Comp*> positioned, fake;
    std::vector<int> observed, preActions;
    int state, deletion; bool initialised, unavailable, managerObserver;
    Comp(const char* n) : name(n), state(0), deletion(0), initialised(false), unavailable(false), managerObserver(false) {}
    Comp* FindChild(const char* n) { std::map<std::string, Comp*>::iterator it = children.find(n); return it == children.end() ? 0 : it->second; }
};
struct Inventory {
    int menu; bool hasStats; std::vector<Comp*> components; Comp* built; int created;
    void Entry(Comp* e, int type, bool needsStats)
    {
        if (!e) return;
        if (!needsStats || hasStats) e->preActions.push_back(type);
        else {
            e->unavailable = true;
            Comp* arrow = e->FindChild("UI_SELECTION_ARROW_RIGHT");
            if (arrow) arrow->deletion = 1;
        }
    }
    void OpenPCStatusMenu()
    {
        menu = 7;
        Comp* list = 0;
        for (unsigned long i = 0; i < components.size(); ++i) {
            if (list) break;
            if (components[i]->name == "PC_STATUS_LIST") list = components[i];
        }
        if (!list) {
            list = built; ++created;
            Comp* frame = list->FindChild("PC_UI_FADING_FRAME");
            Comp* viewport = list->FindChild("PC_DESCRIPTION_VIEWPORT");
            if (frame) {
                static const char* const names[] = { "PC_TITLE_STATUS", "PC_RING_PIC_QUESTS", "PC_DESCRIPTION_QUESTS", "PC_SUBTITLE_QUESTS",
                    "PC_RING_PIC_LOGBOOK", "PC_DESCRIPTION_LOGBOOK", "PC_SUBTITLE_LOGBOOK", "PC_RING_PIC_EXPERIENCE", "PC_DESCRIPTION_EXPERIENCE",
                    "PC_SUBTITLE_EXPERIENCE", "PC_RING_PIC_PERSONALITY", "PC_DESCRIPTION_PERSONALITY", "PC_SUBTITLE_PERSONALITY",
                    "PC_DESCRIPTION_VIEWPORT" };
                for (int k = 0; k < 14; ++k) {
                    Comp* c = list->FindChild(names[k]);
                    if (c) {
                        frame->positioned.push_back(c);
                        if ((k == 2 || k == 5 || k == 8 || k == 11) && viewport) viewport->fake.push_back(c);
                    }
                }
            }
            Comp* quests = list->FindChild("PC_MENU_ENTRY_QUESTS");
            Comp* logBook = list->FindChild("PC_MENU_ENTRY_LOG_BOOK");
            Comp* experience = list->FindChild("PC_MENU_ENTRY_EXPERIENCE");
            Comp* personality = list->FindChild("PC_MENU_ENTRY_PERSONALITY");
            Entry(quests, 257, false); Entry(logBook, 256, false); Entry(experience, 282, true); Entry(personality, 289, true);
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
    {   Comp list("PC_STATUS_LIST"), frame("F"), view("V"), dq("DQ"), q("Q"), ex("EX"), pe("PE"), arrow("A"), close("C");
        list.children["PC_UI_FADING_FRAME"] = &frame; list.children["PC_DESCRIPTION_VIEWPORT"] = &view; list.children["PC_DESCRIPTION_QUESTS"] = &dq;
        list.children["PC_MENU_ENTRY_QUESTS"] = &q; list.children["PC_MENU_ENTRY_EXPERIENCE"] = &ex; list.children["PC_MENU_ENTRY_PERSONALITY"] = &pe;
        list.children["PC_CLOSE_CLICKABLE"] = &close; ex.children["UI_SELECTION_ARROW_RIGHT"] = &arrow;
        Inventory inv; inv.menu = 0; inv.hasStats = false; inv.built = &list; inv.created = 0;
        inv.OpenPCStatusMenu();
        check(inv.menu == 7 && inv.created == 1 && list.initialised && inv.components.size() == 1, "built");
        check(frame.positioned.size() == 2 && frame.positioned[0] == &dq && frame.positioned[1] == &view && view.fake.size() == 1, "frame");
        check(q.preActions.size() == 1 && q.preActions[0] == 257, "quests action");
        check(ex.unavailable && arrow.deletion == 1 && ex.preActions.empty() && pe.unavailable, "no stats");
        check(close.managerObserver && close.observed.size() == 1 && list.state == 5, "tail");
        inv.OpenPCStatusMenu();
        check(inv.created == 1 && close.observed.size() == 2, "reused");
    }
    {   Comp list("PC_STATUS_LIST"), ex("EX"), pe("PE"), lb("LB");
        list.children["PC_MENU_ENTRY_EXPERIENCE"] = &ex; list.children["PC_MENU_ENTRY_PERSONALITY"] = &pe; list.children["PC_MENU_ENTRY_LOG_BOOK"] = &lb;
        Inventory inv; inv.menu = 0; inv.hasStats = true; inv.built = &list; inv.created = 0;
        inv.OpenPCStatusMenu();
        check(ex.preActions.size() == 1 && ex.preActions[0] == 282 && pe.preActions[0] == 289 && lb.preActions[0] == 256 && !ex.unavailable, "with stats");
    }
    if (fails) return 1;
    std::printf("0058a763_TEST PASS\n");
    return 0;
}