// Behaviour test for 0x0058F649 CTCInventory::OpenPCInventory(): with a sound manager the groups 0x10 / 0x40 / 0x80 fade to 0 over
// 500 ms; a hero fireball has its move cancelled; the menu state becomes 0, the back buffer is captured and 3D rendering is turned
// off; the mouse pointer and PC_TOP_LEVEL_LIST are created; the skills entry gets the open-skills pre-action (234) when the hero has
// abilities, else it is made unavailable and its arrow (if any) gets a delete-this-instance deletion; the photojournal entry likewise
// with HasScreenshots and action 4000 (its arrow is not null-checked); the UI manager gets the inventory and the hero; both
// components are kept, every component initialised, the list goes to state 5, the base inventory opens, 3D rendering is turned off
// again and a pending shortcut screen is opened and cleared.
#include <cstdio>
#include <string>
#include <vector>
#include <map>

struct Comp {
    std::string name;
    std::map<std::string, Comp*> children;
    std::vector<int> preActions;
    int deletion; int state; int initialised; bool unavailable;
    Comp(const char* n) : name(n), deletion(0), state(0), initialised(0), unavailable(false) {}
    Comp* FindChild(const char* n) { std::map<std::string, Comp*>::iterator it = children.find(n); return it == children.end() ? 0 : it->second; }
};
struct Sound { std::vector<int> groups; std::vector<float> volumes, times; void SetGainFader(int g, float v, float t) { groups.push_back(g); volumes.push_back(v); times.push_back(t); } };
struct Manager { void* inventory; void* player; std::map<std::string, Comp*> make; Comp* CreateComponent(const char* n) { return make[n]; } };
struct Fireball { int cancels; void CancelMove() { ++cancels; } };
struct Hero { Fireball* fireball; bool abilities; };

struct Inventory {
    Sound* sound; Manager* manager; Hero* hero; bool screenshots;
    int menu; int captures; std::vector<bool> render3D; float fade; int opens; int screenToShortcut; std::vector<int> shortcuts;
    Comp* mouse; Comp* top; std::vector<Comp*> components;
    void OpenPCInventory()
    {
        if (sound) { sound->SetGainFader(0x10, 0.0f, 500.0f); sound->SetGainFader(0x40, 0.0f, 500.0f); sound->SetGainFader(0x80, 0.0f, 500.0f); }
        if (hero->fireball) hero->fireball->CancelMove();
        menu = 0; ++captures; render3D.push_back(false); fade = 0.0f;
        mouse = manager->CreateComponent("UI_MOUSE_POINTER");
        top = manager->CreateComponent("PC_TOP_LEVEL_LIST");
        Comp* skillsEntry = top->FindChild("PC_MENU_ENTRY_SKILLS");
        if (skillsEntry) {
            if (!hero->abilities) {
                skillsEntry->unavailable = true;
                Comp* skillsArrow = skillsEntry->FindChild("PC_MENU_ENTRY_TOP_LEVEL_ARROW");
                if (skillsArrow) skillsArrow->deletion = 1;
            } else skillsEntry->preActions.push_back(234);
        }
        Comp* photojournalEntry = top->FindChild("PC_MENU_ENTRY_PHOTOJOURNAL");
        if (photojournalEntry) {
            if (!screenshots) {
                photojournalEntry->unavailable = true;
                photojournalEntry->FindChild("PC_MENU_ENTRY_TOP_LEVEL_ARROW")->deletion = 1;
            } else photojournalEntry->preActions.push_back(4000);
        }
        manager->inventory = this; manager->player = hero;
        components.push_back(mouse); components.push_back(top);
        for (unsigned long i = 0; i < components.size(); ++i) ++components[i]->initialised;
        top->state = 5; ++opens; render3D.push_back(false);
        if (screenToShortcut != -1) { shortcuts.push_back(screenToShortcut); screenToShortcut = -1; }
    }
};
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); ++fails; } }
static void init(Inventory& inv, Sound* s, Manager* m, Hero* h, bool shots, int shortcut)
{
    inv.sound = s; inv.manager = m; inv.hero = h; inv.screenshots = shots; inv.menu = 3; inv.captures = 0; inv.fade = 1.0f;
    inv.opens = 0; inv.screenToShortcut = shortcut; inv.mouse = 0; inv.top = 0;
}
int main()
{
    {   // no abilities, no screenshots, sound and fireball present, pending shortcut 4
        Comp mouse("UI_MOUSE_POINTER"), top("PC_TOP_LEVEL_LIST"), skills("PC_MENU_ENTRY_SKILLS"), photo("PC_MENU_ENTRY_PHOTOJOURNAL"),
             arrow1("PC_MENU_ENTRY_TOP_LEVEL_ARROW"), arrow2("PC_MENU_ENTRY_TOP_LEVEL_ARROW"), old("OLD");
        top.children["PC_MENU_ENTRY_SKILLS"] = &skills; top.children["PC_MENU_ENTRY_PHOTOJOURNAL"] = &photo;
        skills.children["PC_MENU_ENTRY_TOP_LEVEL_ARROW"] = &arrow1; photo.children["PC_MENU_ENTRY_TOP_LEVEL_ARROW"] = &arrow2;
        Sound s; Manager m; m.make["UI_MOUSE_POINTER"] = &mouse; m.make["PC_TOP_LEVEL_LIST"] = &top; m.inventory = 0; m.player = 0;
        Fireball f; f.cancels = 0; Hero h; h.fireball = &f; h.abilities = false;
        Inventory inv; init(inv, &s, &m, &h, false, 4); inv.components.push_back(&old);
        inv.OpenPCInventory();
        check(s.groups.size() == 3 && s.groups[0] == 0x10 && s.groups[1] == 0x40 && s.groups[2] == 0x80 && s.volumes[2] == 0.0f && s.times[1] == 500.0f, "fades");
        check(f.cancels == 1 && inv.menu == 0 && inv.captures == 1 && inv.fade == 0.0f, "fireball / menu / capture");
        check(skills.unavailable && arrow1.deletion == 1 && skills.preActions.empty(), "skills unavailable");
        check(photo.unavailable && arrow2.deletion == 1 && photo.preActions.empty(), "photojournal unavailable");
        check(m.inventory == &inv && m.player == &h, "manager");
        check(inv.components.size() == 3 && old.initialised == 1 && mouse.initialised == 1 && top.initialised == 1 && top.state == 5, "components");
        check(inv.opens == 1 && inv.render3D.size() == 2 && inv.shortcuts.size() == 1 && inv.shortcuts[0] == 4 && inv.screenToShortcut == -1, "shortcut");
    }
    {   // abilities and screenshots, no sound manager, no fireball, no shortcut; skills arrow missing is fine
        Comp mouse("UI_MOUSE_POINTER"), top("PC_TOP_LEVEL_LIST"), skills("PC_MENU_ENTRY_SKILLS"), photo("PC_MENU_ENTRY_PHOTOJOURNAL");
        top.children["PC_MENU_ENTRY_SKILLS"] = &skills; top.children["PC_MENU_ENTRY_PHOTOJOURNAL"] = &photo;
        Manager m; m.make["UI_MOUSE_POINTER"] = &mouse; m.make["PC_TOP_LEVEL_LIST"] = &top;
        Hero h; h.fireball = 0; h.abilities = true;
        Inventory inv; init(inv, 0, &m, &h, true, -1);
        inv.OpenPCInventory();
        check(skills.preActions.size() == 1 && skills.preActions[0] == 234 && !skills.unavailable, "skills action");
        check(photo.preActions.size() == 1 && photo.preActions[0] == 4000 && !photo.unavailable, "photojournal action");
        check(inv.shortcuts.empty() && inv.screenToShortcut == -1 && inv.components.size() == 2, "no shortcut");
    }
    {   // no abilities, skills entry without an arrow; no entries at all for the photojournal
        Comp mouse("UI_MOUSE_POINTER"), top("PC_TOP_LEVEL_LIST"), skills("PC_MENU_ENTRY_SKILLS");
        top.children["PC_MENU_ENTRY_SKILLS"] = &skills;
        Manager m; m.make["UI_MOUSE_POINTER"] = &mouse; m.make["PC_TOP_LEVEL_LIST"] = &top;
        Hero h; h.fireball = 0; h.abilities = false;
        Inventory inv; init(inv, 0, &m, &h, false, -1);
        inv.OpenPCInventory();
        check(skills.unavailable && skills.preActions.empty() && top.state == 5, "no arrow");
    }
    if (fails) return 1;
    std::printf("0058f649_TEST PASS\n");
    return 0;
}