// Test for 0x005FCBE0 CTCInventoryMap::ConstructPastQuestsList: self-contained behaviour model -- the completed quest cards are
// copied, their things collected in order; with none, no component is built and 0 is returned; otherwise a sub list is created,
// the past-quests title is added as its non-scrolling child, and the quest inventory fills it with the things, screen type 2 and
// the parent; the list is returned.
#include <cstdio>
#include <vector>
#include <list>

struct Component { const char* name; std::vector<Component*> fixed; };
static std::vector<Component*> g_created;
static Component* CreateComponent(const char* name)
{
    Component* c = new Component;
    c->name = name;
    g_created.push_back(c);
    return c;
}
struct QuestCall { int calls; std::vector<int> things; Component* list; int type; Component* parent; };
static QuestCall g_call;
static void ConstructQuestList(const std::vector<int>& things, Component* list, int type, Component* parent)
{
    ++g_call.calls; g_call.things = things; g_call.list = list; g_call.type = type; g_call.parent = parent;
}
static Component* ConstructPastQuestsList(const std::list<int>& completed, Component* parent)
{
    std::list<int> cards = completed;
    std::vector<int> things;
    for (std::list<int>::iterator it = cards.begin(); it != cards.end(); ++it)
        things.push_back(*it);
    if (cards.size() <= 0)
        return 0;
    Component* list = CreateComponent("UI_SUB_LIST");
    Component* title = CreateComponent("UI_TITLE_QUEST_PAST");
    list->fixed.push_back(title);
    ConstructQuestList(things, list, 2, parent);
    return list;
}

static int fail(int n) { std::printf("005fcbe0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Component parent = { "parent" };
    std::list<int> none;
    g_call.calls = 0;
    if (ConstructPastQuestsList(none, &parent) != 0) return fail(1);
    if (!g_created.empty() || g_call.calls != 0) return fail(2);
    std::list<int> done;
    done.push_back(7); done.push_back(3); done.push_back(9);
    Component* list = ConstructPastQuestsList(done, &parent);
    if (!list || g_created.size() != 2) return fail(3);
    if (list != g_created[0] || list->fixed.size() != 1 || list->fixed[0] != g_created[1]) return fail(4);
    if (g_call.calls != 1 || g_call.list != list || g_call.type != 2 || g_call.parent != &parent) return fail(5);
    if (g_call.things.size() != 3 || g_call.things[0] != 7 || g_call.things[1] != 3 || g_call.things[2] != 9) return fail(6);
    if (done.size() != 3) return fail(7);                                       // the manager's list is only copied
    for (unsigned int i = 0; i < g_created.size(); ++i) delete g_created[i];
    std::printf("005fcbe0_TEST PASS\n");
    return 0;
}