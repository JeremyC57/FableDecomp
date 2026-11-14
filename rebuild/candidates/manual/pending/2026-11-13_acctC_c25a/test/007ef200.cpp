// Behaviour test for 0x007EF200 CTCCarriedActionUseExpression::OnActionUse(CThing&): the base use always runs and the item always
// returns to the inventory (when it has the inventory-item TC); nothing else happens for a null / dead / berserk user or an expression
// TC without a def. A game-event def posts one CGameEvent (type, player number) only when the item's player is a character. A script
// def activates its quest (name, game_script false, load flag) and performs the expression only when PerformAfterScript is set; a def
// with neither performs the expression. The expression action targets the targeted thing when it is of type 2 and in range 2, else of
// type 1 and in range 1, else nothing; it carries the expression def and the item's def index with -1, -1, -1.
#include <cstdio>
#include <string>
#include <vector>
struct Targeting { int type; bool range[3]; void* targeted;
    bool IsOfType(int t) const { return type == t; } bool InRange(int t) const { return range[t]; } };
struct Def { long eventType; std::string script; bool load, performAfter; };
struct Event { long type; int player; };
struct Action { void* target; const Def* def; long index, a, b, c; };
struct Log { int baseUses, returns; std::vector<Event> events; std::vector<std::string> quests; std::vector<bool> loads;
             std::vector<bool> gameScripts; std::vector<Action> actions;
             Log() : baseUses(0), returns(0) {} };
struct User { bool null, dead, berserk; Targeting* targeting; };
struct Item { const Def* def; bool playerIsCharacter; int playerNumber; long defIndex; bool inventoryItem; };
static void OnActionUse(User& user, Item& item, Log& log)
{
    ++log.baseUses;
    if (!user.null && !user.dead && !user.berserk) {
        void* target = 0;
        if (user.targeting) {
            const Targeting* t = user.targeting;
            if (t->IsOfType(2) && t->InRange(2)) target = t->targeted;
            else if (t->IsOfType(1) && t->InRange(1)) target = t->targeted;
        }
        if (item.def) {
            const Def& def = *item.def;
            Action action = { target, item.def, item.defIndex, -1, -1, -1 };
            if (def.eventType != 0) {
                if (item.playerIsCharacter) { Event e = { def.eventType, item.playerNumber }; log.events.push_back(e); }
            } else if (def.script != "") {
                log.quests.push_back(def.script); log.gameScripts.push_back(false); log.loads.push_back(def.load);
                if (def.performAfter) log.actions.push_back(action);
            } else
                log.actions.push_back(action);
        }
    }
    if (item.inventoryItem) ++log.returns;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    int thing2 = 2;
    Targeting tg = { 2, { false, true, true }, &thing2 };
    Def plain = { 0, "", false, false }, evdef = { 77, "", false, false }, quest = { 0, "Q_X", true, false }, questp = { 0, "Q_Y", false, true };
    User u = { false, false, false, &tg };
    Item it = { &plain, true, 3, 41, true };
    Log l;
    OnActionUse(u, it, l);
    check(l.baseUses == 1 && l.returns == 1 && l.actions.size() == 1 && l.actions[0].target == &thing2 && l.actions[0].def == &plain &&
          l.actions[0].index == 41 && l.actions[0].a == -1 && l.actions[0].b == -1 && l.actions[0].c == -1, "plain expression at type-2 target");
    tg.range[2] = false; l = Log(); OnActionUse(u, it, l);
    check(l.actions.size() == 1 && l.actions[0].target == 0, "type 2 out of range: no target");
    tg.type = 1; tg.range[1] = true; l = Log(); OnActionUse(u, it, l);
    check(l.actions[0].target == &thing2, "type 1 in range");
    tg.range[1] = false; l = Log(); OnActionUse(u, it, l);
    check(l.actions[0].target == 0, "type 1 out of range");
    it.def = &evdef; l = Log(); OnActionUse(u, it, l);
    check(l.events.size() == 1 && l.events[0].type == 77 && l.events[0].player == 3 && l.actions.empty(), "game event");
    it.playerIsCharacter = false; l = Log(); OnActionUse(u, it, l);
    check(l.events.empty() && l.actions.empty() && l.returns == 1, "game event needs a character player");
    it.def = &quest; l = Log(); OnActionUse(u, it, l);
    check(l.quests.size() == 1 && l.quests[0] == "Q_X" && l.loads[0] && !l.gameScripts[0] && l.actions.empty(), "quest without perform");
    it.def = &questp; l = Log(); OnActionUse(u, it, l);
    check(l.quests.size() == 1 && l.quests[0] == "Q_Y" && !l.loads[0] && l.actions.size() == 1, "quest then perform");
    it.def = 0; l = Log(); OnActionUse(u, it, l);
    check(l.quests.empty() && l.actions.empty() && l.baseUses == 1 && l.returns == 1, "no def");
    it.def = &plain; u.berserk = true; l = Log(); OnActionUse(u, it, l);
    check(l.actions.empty() && l.returns == 1, "berserk user");
    u.berserk = false; u.dead = true; l = Log(); OnActionUse(u, it, l);
    check(l.actions.empty(), "dead user");
    u.dead = false; u.null = true; it.inventoryItem = false; l = Log(); OnActionUse(u, it, l);
    check(l.actions.empty() && l.returns == 0 && l.baseUses == 1, "null user, not an inventory item");
    if (fails == 0) std::printf("007ef200_TEST PASS\n");
    return fails;
}
