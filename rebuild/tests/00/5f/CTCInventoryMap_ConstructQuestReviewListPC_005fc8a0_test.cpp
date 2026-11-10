// Test for 0x005FC8A0 CTCInventoryMap::ConstructQuestReviewListPC: self-contained behaviour model -- the parent's PC_UI_FRAME_QUESTS
// frame is looked up first; the active quest cards are walked in place (no copy) and their things collected; with none, nothing is built and 0 is
// returned; otherwise a PC_SUB_LIST is created, the PC_TITLE_QUEST_CURRENT title becomes its non-scrolling child and a position child
// of the frame, and the quest inventory's PC builder gets (container, things, list, 1, parent); the list is returned.
#include <cstdio>
#include <cstring>
#include <vector>
#include <list>

struct Component { const char* name; std::vector<Component*> fixed; std::vector<Component*> positioned; };
static std::vector<Component*> g_created;
static int g_finds;
static Component g_frame = { "PC_UI_FRAME_QUESTS" };
static Component* FindChild(Component*, const char* name)
{
    ++g_finds;
    return std::strcmp(name, "PC_UI_FRAME_QUESTS") == 0 ? &g_frame : 0;
}
static Component* CreateComponent(const char* name)
{
    Component* c = new Component;
    c->name = name;
    g_created.push_back(c);
    return c;
}
struct QuestCall { int calls; Component* container; std::vector<int> things; Component* list; int type; Component* parent; };
static QuestCall g_call;
static void ConstructQuestListPC(Component* container, const std::vector<int>& things, Component* list, int type, Component* parent)
{
    ++g_call.calls; g_call.container = container; g_call.things = things; g_call.list = list; g_call.type = type; g_call.parent = parent;
}
static Component* ConstructQuestReviewListPC(const std::list<int>& active, Component* container, Component* parent)
{
    Component* frame = FindChild(parent, "PC_UI_FRAME_QUESTS");
    const std::list<int>& cards = active;
    std::vector<int> things;
    for (std::list<int>::const_iterator it = cards.begin(); it != cards.end(); ++it)
        things.push_back(*it);
    if (cards.size() <= 0)
        return 0;
    Component* list = CreateComponent("PC_SUB_LIST");
    Component* title = CreateComponent("PC_TITLE_QUEST_CURRENT");
    list->fixed.push_back(title);
    frame->positioned.push_back(title);
    ConstructQuestListPC(container, things, list, 1, parent);
    return list;
}

static int fail(int n) { std::printf("005fc8a0_TEST FAIL %d\n", n); return 1; }

int main()
{
    Component container = { "container" };
    Component parent = { "parent" };
    std::list<int> none;
    if (ConstructQuestReviewListPC(none, &container, &parent) != 0) return fail(1);
    if (g_finds != 1 || !g_created.empty() || g_call.calls != 0) return fail(2);      // the frame lookup happens even then
    std::list<int> done;
    done.push_back(4); done.push_back(11);
    Component* list = ConstructQuestReviewListPC(done, &container, &parent);
    if (!list || g_created.size() != 2 || list != g_created[0]) return fail(3);
    if (std::strcmp(list->name, "PC_SUB_LIST") != 0 || list->fixed.size() != 1 || list->fixed[0] != g_created[1]) return fail(4);
    if (g_frame.positioned.size() != 1 || g_frame.positioned[0] != g_created[1]) return fail(5);
    if (g_call.calls != 1 || g_call.container != &container || g_call.list != list || g_call.type != 1 || g_call.parent != &parent)
        return fail(6);
    if (g_call.things.size() != 2 || g_call.things[0] != 4 || g_call.things[1] != 11) return fail(7);
    if (std::strcmp(g_created[1]->name, "PC_TITLE_QUEST_CURRENT") != 0 || done.size() != 2) return fail(8);   // the manager's list is only read
    for (unsigned int i = 0; i < g_created.size(); ++i) delete g_created[i];
    std::printf("005fc8a0_TEST PASS\n");
    return 0;
}