// Behaviour test for EnsureUndoAbility (0x00570009): with a living hero the parked ability purchase is undone
// (confirmed) and the mouse pointer is released, re-added and set to observe event 0x19 after the dialog is released;
// the dialog's yes/no buttons (when found) are always released and the close event (2) is always sent -- through a
// copy of the body over a recording stand-in manager.
#include <cstdio>
#include <string>
#include <vector>

struct Component {
    std::string name;
    std::vector<Component*> children;
    std::vector<int> observed;
    Component(const std::string& n) : name(n) { }
    Component* FindChild(const std::string& n)
    {
        for (unsigned int i = 0; i < children.size(); ++i)
            if (children[i]->name == n) return children[i];
        return 0;
    }
};

struct Manager {
    std::vector<std::string> log;
    void ReleaseObserver(Component* c) { log.push_back("release " + (c ? c->name : std::string("null"))); }
    void AddObserver(Component* c) { log.push_back("add " + c->name); }
    void SendEvent(int e) { log.push_back(e == 2 ? "event 2" : "event ?"); }
};

struct Hero { bool dead; int undone; bool confirmed; };

static Manager g_manager;
static int g_undoAbility = 0;
static Component* g_dialog = 0;
static Component* g_pointer = 0;

static void EnsureUndoAbility(Hero* hero)
{
    if (hero != 0 && !hero->dead) {
        hero->undone = g_undoAbility;
        hero->confirmed = true;
        g_manager.ReleaseObserver(g_dialog);
        g_manager.ReleaseObserver(g_pointer);
        g_manager.AddObserver(g_pointer);
        g_pointer->observed.push_back(0x19);
    }
    Component* yes = g_dialog->FindChild("PC_YESNO_GAME_YESBUTTON");
    if (yes) g_manager.ReleaseObserver(yes);
    Component* no = g_dialog->FindChild("PC_YESNO_GAME_NOBUTTON");
    if (no) g_manager.ReleaseObserver(no);
    g_manager.SendEvent(2);
}

int main()
{
    Component dialog("dialog"), pointer("pointer"), yes("PC_YESNO_GAME_YESBUTTON");
    dialog.children.push_back(&yes);
    g_dialog = &dialog;
    g_pointer = &pointer;
    g_undoAbility = 12;
    Hero hero = { false, 0, false };
    EnsureUndoAbility(&hero);
    const char* expect[] = { "release dialog", "release pointer", "add pointer", "release PC_YESNO_GAME_YESBUTTON", "event 2" };
    if (hero.undone != 12 || !hero.confirmed || pointer.observed.size() != 1 || pointer.observed[0] != 0x19 ||
        g_manager.log.size() != 5) {
        std::printf("00570009_TEST FAIL\n");
        return 1;
    }
    for (int i = 0; i < 5; ++i)
        if (g_manager.log[i] != expect[i]) { std::printf("00570009_TEST FAIL\n"); return 1; }
    g_manager.log.clear();
    Hero dead = { true, 0, false };
    EnsureUndoAbility(&dead);
    if (dead.undone != 0 || g_manager.log.size() != 2 || g_manager.log[1] != "event 2") {
        std::printf("00570009_TEST FAIL\n");
        return 1;
    }
    std::printf("00570009_TEST PASS\n");
    return 0;
}