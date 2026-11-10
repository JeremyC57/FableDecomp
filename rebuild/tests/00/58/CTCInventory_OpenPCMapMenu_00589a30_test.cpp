// Behaviour test for 0x00589A30 CTCInventory::OpenPCMapMenu(): the old menu components are dropped and the menu state becomes 6;
// an open PC_MAP_LIST component is reused (the search stops at the first match), else the hero's inventory map builds one
// (ConstructPCMapList(false, 0)) -- a null list returns at once -- which is initialised and appended to the menu components; then
// the four scroll buttons observe event 25, the back / close clickables become manager observers and observe event 25, and the
// list goes to state 5.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name;
    std::map<std::string, Comp*> children;
    std::vector<int> observed;
    int state; bool initialised; bool managerObserver;
    Comp(const char* n) : name(n), state(0), initialised(false), managerObserver(false) {}
    Comp* FindChild(const char* n) { std::map<std::string, Comp*>::iterator it = children.find(n); return it == children.end() ? 0 : it->second; }
};
struct MapTC {
    Comp* built; int calls; bool lastSkip; bool* lastOut;
    Comp* ConstructPCMapList(bool skip, bool* out) { ++calls; lastSkip = skip; lastOut = out; return built; }
};
struct Inventory {
    int menu; int deletes; std::vector<Comp*> components; MapTC* map;
    void DeleteMenuComponents() { ++deletes; }
    void OpenPCMapMenu()
    {
        unsigned long i;
        DeleteMenuComponents();
        menu = 6;
        Comp* list = 0;
        for (i = 0; i < components.size(); ++i) {
            if (list) break;
            if (components[i]->name == "PC_MAP_LIST") list = components[i];
        }
        if (!list) {
            list = map->ConstructPCMapList(false, 0);
            if (!list) return;
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
    {   // build: the map TC makes the list
        Comp list("PC_MAP_LIST"), up("SCROLL_LIST_UP"), dd("SCROLL_DESCRIPTION_DOWN"), back("PC_BACK_CLICKABLE"), close("PC_CLOSE_CLICKABLE");
        list.children["SCROLL_LIST_UP"] = &up; list.children["SCROLL_DESCRIPTION_DOWN"] = &dd;
        list.children["PC_BACK_CLICKABLE"] = &back; list.children["PC_CLOSE_CLICKABLE"] = &close;
        Comp other("OTHER");
        MapTC m; m.built = &list; m.calls = 0; m.lastSkip = true; m.lastOut = (bool*)1;
        Inventory inv; inv.menu = 0; inv.deletes = 0; inv.map = &m; inv.components.push_back(&other);
        inv.OpenPCMapMenu();
        check(inv.deletes == 1 && inv.menu == 6, "deleted and state");
        check(m.calls == 1 && !m.lastSkip && m.lastOut == 0, "built through the map TC");
        check(inv.components.size() == 2 && inv.components[1] == &list && list.initialised, "kept");
        check(up.observed.size() == 1 && up.observed[0] == 25 && dd.observed.size() == 1, "scroll observers");
        check(back.managerObserver && close.managerObserver && back.observed.size() == 1 && close.observed.size() == 1, "clickables");
        check(list.state == 5, "list state");
        inv.OpenPCMapMenu();   // second open reuses the list
        check(m.calls == 1 && inv.components.size() == 2 && up.observed.size() == 2 && inv.deletes == 2, "reused");
    }
    {   // the map TC fails: nothing kept, no state change on any list
        MapTC m; m.built = 0; m.calls = 0;
        Inventory inv; inv.menu = 0; inv.deletes = 0; inv.map = &m;
        inv.OpenPCMapMenu();
        check(m.calls == 1 && inv.components.empty() && inv.menu == 6, "null list returns");
    }
    {   // two lists named PC_MAP_LIST: the first one is used
        Comp a("PC_MAP_LIST"), b("PC_MAP_LIST");
        MapTC m; m.built = 0; m.calls = 0;
        Inventory inv; inv.menu = 0; inv.deletes = 0; inv.map = &m; inv.components.push_back(&a); inv.components.push_back(&b);
        inv.OpenPCMapMenu();
        check(m.calls == 0 && a.state == 5 && b.state == 0, "first match");
    }
    if (fails) return 1;
    std::printf("00589a30_TEST PASS\n");
    return 0;
}