// Behaviour test for 0x0056AD40 CTCBoastUI::Activate: the same control flow over plain models of the quest manager, the UI manager
// and the hero (components are numbered records, the text bank returns fixed labels), checked against outcomes worked out by hand:
// the subtitle / already-active early exits, the "no quest has boasts" exit, and a full menu where one counted quest has no quest
// card (counted by the first loop, skipped by the second), one has no script and one has no boasts.
#include <cstdio>
#include <list>
#include <map>
#include <string>
#include <vector>

struct Boast { long ID; long Cost; long Reward; std::string ScriptName; std::string Text; std::string Title; long GroupIndex; };
struct ActiveQuest { bool hasScript; std::string scriptName; };

enum { ACTION_CLOSE_BOAST_MENU = 0xBBA, ACTION_TAKE_BOAST = 4, ACTION_DELETE_PREVIOUS = 0x26 };
struct Action { int type; std::vector<long> longParams; std::vector<std::string> stringParams; };
struct Component {
    std::string kind;                 // the def name it was created from
    std::string text;                 // menu entry label / description text value
    std::vector<int> children;        // AddChild
    std::vector<int> scrolling;       // AddScrollingChild / AddScrollingChildAndSetOffsets
    std::vector<Action> actions;      // AddAction
    std::vector<Action> backActions;  // AddActionOnBack
    std::vector<int> uiActionParams;  // CMenuEntry::AddUIActionParam
    unsigned long offsets;            // CList::InitialiseOffsets
    bool initialised; int state;
};

struct World {
    bool displayingText, inCutscene;
    bool quickAccessDeactivated, quickAccessUpdated;
    std::list<ActiveQuest> activeQuests;
    std::map<std::string, std::list<Boast> > boastsByScript;
    std::map<std::string, std::string> questCardNameByScript;   // a script with a quest card -> its quest name
    bool guiClosed, guiCleared;
    std::vector<Component> components;
    bool heroPresent, heroBlocking, heroHasFireball, fireballCancelled, boastModeAdded;
    int Create(const std::string& kind, const std::string& text)
    {
        Component c; c.kind = kind; c.text = text; c.offsets = 0; c.initialised = false; c.state = 0;
        components.push_back(c);
        return (int)components.size() - 1;
    }
};

struct BoastUI {
    std::map<int, Boast> BoastGroups;
    bool Active, ActivationRequested;
    std::vector<int> Components;
    std::map<long, std::vector<int> > lastGroupEntries;   // the function's local group map, kept for the test
    void Activate(World& w);
};

static std::string Label(const char* symbol)
{
    std::string s(symbol);
    if (s == "TEXT_GUI_MENU_QUEST") return "Quest";
    if (s == "TEXT_GUI_MENU_BOAST") return "Boast";
    if (s == "TEXT_GUI_MENU_WAGER") return "Wager";
    if (s == "TEXT_GUI_MENU_REWARD") return "Reward";
    if (s == "TEXT_GUI_MENU_GOLD") return "gold";
    return "Take boast";
}
static std::string IntText(long v) { char b[32]; std::sprintf(b, "%ld", v); return b; }

void BoastUI::Activate(World& w)
{
    if (w.displayingText || w.inCutscene)
        return;
    w.quickAccessDeactivated = true; w.quickAccessUpdated = true;          // Deactivate() + GlobalUpdate(true)
    BoastGroups.clear();
    if (Active)
        return;

    unsigned long questsWithBoasts = 0;
    std::list<ActiveQuest>::const_iterator quest = w.activeQuests.begin();
    while (quest != w.activeQuests.end()) {
        if (quest->hasScript) {
            std::list<Boast> boasts = w.boastsByScript[quest->scriptName];
            if (!boasts.empty())
                ++questsWithBoasts;
        }
        ++quest;
    }
    if (questsWithBoasts == 0) {
        ActivationRequested = false;
        return;
    }

    w.guiClosed = true; w.guiCleared = true;
    int mousePointer = w.Create("UI_MOUSE_POINTER", "");
    int boastList = w.Create("PC_BOAST_LIST", "");
    w.components[boastList].offsets = questsWithBoasts;
    for (std::list<ActiveQuest>::const_iterator q = w.activeQuests.begin(); q != w.activeQuests.end(); ++q) {
        if (!q->hasScript)
            continue;
        std::map<std::string, std::string>::const_iterator card = w.questCardNameByScript.find(q->scriptName);
        if (card == w.questCardNameByScript.end())
            continue;
        std::string questName = card->second;
        std::list<Boast> boasts = w.boastsByScript[q->scriptName];
        if (boasts.empty())
            continue;
        int branch = w.Create("PC_MENU_ENTRY_BRANCH_TEMPLATE", questName);
        w.components[boastList].scrolling.push_back(branch);
        Action close; close.type = ACTION_CLOSE_BOAST_MENU;
        w.components[branch].backActions.push_back(close);
        int subList = w.Create("PC_BOAST_SUB_LIST", "");
        w.components[branch].children.push_back(subList);
        w.components[subList].offsets = (unsigned long)boasts.size();
        std::map<long, std::vector<int> > groupEntries;
        std::map<int, int> takeEntries;
        for (std::list<Boast>::const_iterator it = boasts.begin(); it != boasts.end(); ++it) {
            const Boast& boast = *it;
            int entry = w.Create("PC_MENU_ENTRY_BRANCH_TEMPLATE", boast.Title);
            w.components[subList].scrolling.push_back(entry);
            int detailList = w.Create("PC_BOAST_SUB_LIST", "");
            w.components[entry].children.push_back(detailList);
            int backdrop = w.Create("PC_BOAST_BACKDROP", "");
            w.components[entry].children.push_back(backdrop);
            std::string value;
            value = Label("TEXT_GUI_MENU_QUEST") + ": " + questName + "\n";
            value += Label("TEXT_GUI_MENU_BOAST") + ": " + boast.Text + "\n";
            value += Label("TEXT_GUI_MENU_WAGER") + ": " + IntText(boast.Cost) + " " + Label("TEXT_GUI_MENU_GOLD") + ", ";
            value += Label("TEXT_GUI_MENU_REWARD") + ": " + IntText(boast.Reward) + " " + Label("TEXT_GUI_MENU_GOLD") + "\n";
            int descriptionText = w.Create("PC_BOAST_DESCRIPTION_TEXT", value);
            w.components[entry].children.push_back(descriptionText);
            int take = w.Create("PC_MENU_ENTRY_TEMPLATE", Label("TEXT_GUI_MENU_TAKE_BOAST"));
            Action takeAction; takeAction.type = ACTION_TAKE_BOAST;
            takeAction.longParams.push_back(boast.ID);
            takeAction.stringParams.push_back(boast.ScriptName);
            w.components[take].actions.push_back(takeAction);
            if (boasts.size() > 1) {
                Action deleteAction; deleteAction.type = ACTION_DELETE_PREVIOUS;
                w.components[take].actions.push_back(deleteAction);
                w.components[entry].uiActionParams.push_back(entry);
                long group = boast.GroupIndex;
                if (group != 0) {
                    BoastGroups.insert(std::make_pair(entry, boast));
                    takeEntries.insert(std::make_pair(entry, take));
                    groupEntries[group].push_back(entry);
                }
            }
            w.components[detailList].scrolling.push_back(take);
        }
        lastGroupEntries = groupEntries;
    }
    w.components[boastList].initialised = true; w.components[boastList].state = 5;
    w.components[mousePointer].initialised = true;
    Components.push_back(mousePointer);
    Components.push_back(boastList);
    Active = true;
    if (w.heroPresent) {
        if (w.heroBlocking)
            w.heroBlocking = false;                                         // UnsetBlocking
        if (w.heroHasFireball)
            w.fireballCancelled = true;                                     // CancelMove
    }
    w.boastModeAdded = true;                                                // AddMode(PLAYER_MODE_BOAST_UI, false)
}

static int failures = 0;
static void Check(bool ok, const char* what) { if (!ok) { std::printf("FAIL: %s\n", what); ++failures; } }
static void Reset(World& w, BoastUI& ui)
{
    w.displayingText = false; w.inCutscene = false; w.quickAccessDeactivated = false; w.quickAccessUpdated = false;
    w.activeQuests.clear(); w.boastsByScript.clear(); w.questCardNameByScript.clear(); w.guiClosed = false; w.guiCleared = false;
    w.components.clear(); w.heroPresent = true; w.heroBlocking = true; w.heroHasFireball = true; w.fireballCancelled = false;
    w.boastModeAdded = false;
    ui.BoastGroups.clear(); ui.Active = false; ui.ActivationRequested = true; ui.Components.clear(); ui.lastGroupEntries.clear();
}
static Boast MakeBoast(long id, long cost, long reward, const char* script, const char* text, const char* title, long group)
{
    Boast b; b.ID = id; b.Cost = cost; b.Reward = reward; b.ScriptName = script; b.Text = text; b.Title = title; b.GroupIndex = group;
    return b;
}
static ActiveQuest Quest(bool hasScript, const char* name) { ActiveQuest q; q.hasScript = hasScript; q.scriptName = name; return q; }

int main()
{
    World w; BoastUI ui;
    // 1. Subtitles on screen: nothing at all happens (the quick-access menu stays, the old groups stay).
    Reset(w, ui);
    w.displayingText = true;
    ui.BoastGroups[7] = MakeBoast(1, 0, 0, "", "", "", 1);
    ui.Activate(w);
    Check(!w.quickAccessDeactivated && ui.BoastGroups.size() == 1 && !ui.Active, "subtitle exit");
    // 2. Already active: the quick-access menu closes and the groups are cleared, nothing is built.
    Reset(w, ui);
    ui.Active = true;
    ui.BoastGroups[7] = MakeBoast(1, 0, 0, "", "", "", 1);
    ui.Activate(w);
    Check(w.quickAccessDeactivated && w.quickAccessUpdated && ui.BoastGroups.empty() && w.components.empty(), "active exit");
    // 3. No active quest offers boasts: the activation request is dropped, no component is created.
    Reset(w, ui);
    w.activeQuests.push_back(Quest(true, "QuestA"));
    w.questCardNameByScript["QuestA"] = "Quest A";
    ui.Activate(w);
    Check(!ui.ActivationRequested && !ui.Active && w.components.empty() && !w.guiClosed, "no-boasts exit");
    // 4. A full menu.
    Reset(w, ui);
    w.activeQuests.push_back(Quest(true, "QuestA"));    // card + two boasts in group 3 and one ungrouped
    w.activeQuests.push_back(Quest(false, "QuestB"));   // no script
    w.activeQuests.push_back(Quest(true, "QuestC"));    // boasts but no quest card: counted, not shown
    w.activeQuests.push_back(Quest(true, "QuestD"));    // card, no boasts
    w.boastsByScript["QuestA"].push_back(MakeBoast(11, 50, 200, "BoastA1", "No armour", "Naked", 3));
    w.boastsByScript["QuestA"].push_back(MakeBoast(12, 100, 400, "BoastA2", "No weapons", "Unarmed", 3));
    w.boastsByScript["QuestA"].push_back(MakeBoast(13, 10, 30, "BoastA3", "In time", "Quick", 0));
    w.boastsByScript["QuestB"].push_back(MakeBoast(21, 1, 1, "BoastB", "b", "b", 0));
    w.boastsByScript["QuestC"].push_back(MakeBoast(31, 1, 1, "BoastC", "c", "c", 0));
    w.questCardNameByScript["QuestA"] = "Quest A";
    w.questCardNameByScript["QuestD"] = "Quest D";
    ui.Activate(w);
    // components: mouse 0, list 1, branch 2, sub list 3, then 5 per boast (entry, detail list, backdrop, text, take)
    Check(w.guiClosed && w.guiCleared && w.components.size() == 4 + 3 * 5, "component count");
    Check(w.components[1].offsets == 2, "list offsets = quests with boasts (QuestA, QuestC)");
    Check(w.components[1].scrolling.size() == 1 && w.components[1].scrolling[0] == 2 && w.components[2].text == "Quest A", "one branch");
    Check(w.components[2].backActions.size() == 1 && w.components[2].backActions[0].type == ACTION_CLOSE_BOAST_MENU, "close on back");
    Check(w.components[2].children.size() == 1 && w.components[2].children[0] == 3 && w.components[3].offsets == 3, "sub list");
    Check(w.components[3].scrolling.size() == 3 && w.components[3].scrolling[1] == 9, "boast entries");
    const Component& e2 = w.components[9];
    Check(e2.text == "Unarmed" && e2.children.size() == 3 && e2.uiActionParams.size() == 1 && e2.uiActionParams[0] == 9, "entry 2");
    Check(w.components[12].text == "Quest: Quest A\nBoast: No weapons\nWager: 100 gold, Reward: 400 gold\n", "description text");
    const Component& take2 = w.components[13];
    Check(take2.text == "Take boast" && take2.actions.size() == 2 && take2.actions[0].type == ACTION_TAKE_BOAST
          && take2.actions[0].longParams[0] == 12 && take2.actions[0].stringParams[0] == "BoastA2"
          && take2.actions[1].type == ACTION_DELETE_PREVIOUS, "take entry");
    Check(w.components[10].scrolling.size() == 1 && w.components[10].scrolling[0] == 13, "take entry in the detail list");
    Check(ui.BoastGroups.size() == 2 && ui.BoastGroups[4].ID == 11 && ui.BoastGroups[9].ID == 12 && ui.BoastGroups.count(14) == 0,
          "boast groups (grouped boasts only)");
    Check(ui.lastGroupEntries.size() == 1 && ui.lastGroupEntries[3].size() == 2 && ui.lastGroupEntries[3][1] == 9, "group entries");
    Check(ui.Components.size() == 2 && ui.Components[0] == 0 && ui.Components[1] == 1 && w.components[1].state == 5
          && w.components[0].initialised && ui.Active, "pushed components");
    Check(!w.heroBlocking && w.fireballCancelled && w.boastModeAdded, "hero");

    if (failures) { std::printf("0056ad40_TEST FAIL\n"); return 1; }
    std::printf("0056ad40_TEST PASS\n");
    return 0;
}