// Behaviour test for 0x007B57C0 CTCCarriedActionUseActivateQuest::OnActionUse(CThing&): the base use always runs and the item always
// returns to the inventory (when it has the inventory-item TC); nothing else happens without an activate-quest sub-def or with an empty
// script name. The "Global_TeleportToHeroGuild" item does nothing while the user is in creature mode 34 or dead; otherwise it marks the
// map as opened via the guild seal (not an inventory screen) and, unless subtitles are showing, a region load is running, the map list
// cannot be built, the creature is not alive or its health is not above DELTA_NOTIONAL_ZERO, posts game event 26 (SLONG 8, the main
// player's number) and message 0x3D (ConstantFPS, origin); on any of those failures the map flags go back (inventory screen, not via
// seal). Any other script makes a live user activate the quest (no object, script name, load flag).
#include <cstdio>
#include <string>
#include <vector>
struct Def { std::string script; bool load; };
struct Map { bool inventoryScreen, viaSeal; bool listOk; int builds; };
struct World { bool subtitles, regionLoad; long fps; int mainPlayer; };
struct User { bool null, dead, hasModes; int mode; bool alive; float health; };
struct Log { int baseUses, returns; std::vector<long> events; std::vector<long> eventData; std::vector<int> eventPlayers;
             std::vector<long> messages; std::vector<long> messageFps; std::vector<std::string> quests; std::vector<bool> loads;
             Log() : baseUses(0), returns(0) {} };
static const float DELTA_NOTIONAL_ZERO = 1e-4f;
static void OnActionUse(User& user, const Def* def, bool inventoryItem, Map& map, const World& w, Log& log)
{
    ++log.baseUses;
    if (def && def->script != "") {
        if (def->script == "Global_TeleportToHeroGuild") {
            if (!(user.hasModes && user.mode == 34)) {
                if (!user.dead) {
                    map.inventoryScreen = false; map.viaSeal = true;
                    bool done = false;
                    if (!w.subtitles && !w.regionLoad) {
                        bool opened = false; ++map.builds; opened = map.listOk;
                        if (opened && user.alive && user.health > DELTA_NOTIONAL_ZERO) {
                            log.events.push_back(26); log.eventData.push_back(8); log.eventPlayers.push_back(w.mainPlayer);
                            log.messages.push_back(0x3d); log.messageFps.push_back(w.fps);
                            done = true;
                        }
                    }
                    if (!done) { map.inventoryScreen = true; map.viaSeal = false; }
                }
            }
        } else if (!user.null && !user.dead) {
            log.quests.push_back(def->script); log.loads.push_back(def->load);
        }
    }
    if (inventoryItem) ++log.returns;
}
static int fails = 0;
static void check(bool c, const char* w) { if (!c) { ++fails; std::printf("FAIL %s\n", w); } }
int main() {
    Def tele = { "Global_TeleportToHeroGuild", false }, quest = { "Q_Guild", true }, empty = { "", true };
    World w = { false, false, 30, 2 };
    User u = { false, false, true, 0, true, 50.0f };
    Map m = { true, false, true, 0 };
    Log l;
    OnActionUse(u, &tele, true, m, w, l);
    check(l.baseUses == 1 && l.returns == 1 && l.events.size() == 1 && l.events[0] == 26 && l.eventData[0] == 8 && l.eventPlayers[0] == 2 &&
          l.messages.size() == 1 && l.messages[0] == 0x3d && l.messageFps[0] == 30 && !m.inventoryScreen && m.viaSeal && m.builds == 1, "teleport ok");
    u.mode = 34; m = Map(); m.listOk = true; m.inventoryScreen = true; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.builds == 0 && m.inventoryScreen && l.returns == 1, "mode 34 blocks");
    u.hasModes = false; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.size() == 1, "no mode manager: allowed");
    u.hasModes = true; u.mode = 0; u.dead = true; m.inventoryScreen = true; m.viaSeal = false; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.inventoryScreen && !m.viaSeal, "dead user: nothing");
    u.dead = false; w.subtitles = true; m.builds = 0; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.builds == 0 && m.inventoryScreen && !m.viaSeal, "subtitles: reset flags");
    w.subtitles = false; w.regionLoad = true; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.builds == 0 && m.inventoryScreen, "region load: reset");
    w.regionLoad = false; m.listOk = false; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.builds == 1 && m.inventoryScreen && !m.viaSeal, "map list failed");
    m.listOk = true; u.alive = false; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.inventoryScreen, "not alive");
    u.alive = true; u.health = 0.0f; l = Log(); OnActionUse(u, &tele, true, m, w, l);
    check(l.events.empty() && m.inventoryScreen, "no health");
    u.health = 1.0f; l = Log(); OnActionUse(u, &quest, false, m, w, l);
    check(l.quests.size() == 1 && l.quests[0] == "Q_Guild" && l.loads[0] && l.returns == 0 && l.events.empty(), "quest activation");
    u.dead = true; l = Log(); OnActionUse(u, &quest, true, m, w, l);
    check(l.quests.empty() && l.returns == 1, "dead user: no quest");
    u.dead = false; u.null = true; l = Log(); OnActionUse(u, &quest, true, m, w, l);
    check(l.quests.empty(), "null user: no quest");
    u.null = false; l = Log(); OnActionUse(u, &empty, true, m, w, l);
    check(l.quests.empty() && l.events.empty() && l.baseUses == 1 && l.returns == 1, "empty script");
    l = Log(); OnActionUse(u, 0, true, m, w, l);
    check(l.quests.empty() && l.baseUses == 1 && l.returns == 1, "no sub-def");
    if (fails == 0) std::printf("007b57c0_TEST PASS\n");
    return fails;
}
