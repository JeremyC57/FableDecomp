// Behaviour test for 0x00D11930 NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main: a portable model of the marker's
// thread -- the three assassins made unkillable; until the assassin cutscene triggers or they are attacked: when they come under
// attack, script all three, have each say its ATTACKED line, set the hero as their best enemy target and release them; otherwise,
// with the hero within 4 m, no conversation running and the assassins not yet talked to, play the next overheard line (six, in
// order); afterwards make the assassins killable and, if the cutscene triggered, put them in the Twinblade camp faction, play
// CS_BANDITCAMP_ASSATTACK, open gate 3 and remove its guard; then idle.
#include <cstdio>
#include <string>
#include <vector>

struct World {
    int frame, terminateAt; bool triggered, underAttack, talkedTo, gate3Open, guardRemoved;
    int convEndsAt; bool killable[3]; std::string faction[3];
    std::vector<int> heroNear;
    std::vector<std::string> log;
    bool T() const { return frame >= terminateAt; }
    bool at(const std::vector<int>& v) const { return frame < (int)v.size() && v[frame] != 0; }
};

static void AssassinMarkerMain(World& w)
{
    for (int i = 0; i < 3; ++i) w.killable[i] = false;
    bool attacked = false; long line = 0;
    static const char* lines[6] = { "A1_ONE", "A2_ONE", "A1_TWO", "A2_TWO", "A1_THREE", "A1_FOUR" };
    while (!w.triggered && !attacked) {
        ++w.frame; if (w.T()) return;
        if (w.underAttack) {
            w.log.push_back("A1_ATTACKED"); w.log.push_back("A2_ATTACKED"); w.log.push_back("A3_ATTACKED");
            w.log.push_back("TARGET_HERO");
            attacked = true;
        } else if (w.at(w.heroNear) && w.frame >= w.convEndsAt && !w.talkedTo) {
            if (line < 6) w.log.push_back(lines[line]);
            w.convEndsAt = w.frame + 2;
            ++line;
        }
    }
    if (w.T()) return;
    for (int i = 0; i < 3; ++i) w.killable[i] = true;
    if (w.triggered) {
        for (int i = 0; i < 3; ++i) w.faction[i] = "FACTION_TWINBLADE_CAMP_BANDITS";
        w.log.push_back("CS_BANDITCAMP_ASSATTACK");
        w.gate3Open = true; w.guardRemoved = true;
    }
}

int main()
{
    World w = World();
    w.terminateAt = 100; w.heroNear.assign(200, 1);
    w.underAttack = false;
    // overhear three lines, then the attack
    World a = w; a.terminateAt = 7; AssassinMarkerMain(a);
    bool ok = a.log.size() == 3 && a.log[0] == "A1_ONE" && a.log[1] == "A2_ONE" && a.log[2] == "A1_TWO" && !a.killable[0];
    World b = w; b.underAttack = true; AssassinMarkerMain(b);
    ok = ok && b.log.size() == 4 && b.log[0] == "A1_ATTACKED" && b.log[3] == "TARGET_HERO" && b.killable[2] && !b.gate3Open;
    World c = w; c.triggered = true; AssassinMarkerMain(c);
    ok = ok && c.log.size() == 1 && c.log[0] == "CS_BANDITCAMP_ASSATTACK" && c.gate3Open && c.guardRemoved
            && c.faction[1] == "FACTION_TWINBLADE_CAMP_BANDITS" && c.killable[1];
    if (!ok) { std::printf("00d11930_TEST FAIL\n"); return 1; }
    std::printf("00d11930_TEST PASS\n");
    return 0;
}
