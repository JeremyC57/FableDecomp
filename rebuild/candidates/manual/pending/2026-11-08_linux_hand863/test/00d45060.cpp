// Behaviour test for 0x00D45060 NScript::CQ_GuildTrainingScript::CheckFriendlyAttacks: a portable model of the Guild guard thread --
// every creature but the sparrows, Whisper (both ages) and the maze rival made friends with everything and unkillable, re-applied
// whenever the hero comes back into HeroGuildComplex; a friendly hit by the hero (bare hands / melee / ranged) inside the complex
// that is not a (non-heal) hit on the maze rival gives the FIRST / SECOND / THIRD warning, and the fourth plays CS_GUILD_BADHERO with
// the FOURTH warning (or waits out a running skill / will test) and resets the count, GuildWarningOccuring set around it.
#include <cstdio>
#include <string>
#include <vector>

struct Creature { std::string def; bool friendly, killable; };
struct World {
    int frame, terminateAt; long warnings; bool warningOccuring, skillTest;
    std::vector<Creature> creatures;
    std::vector<int> inComplex, friendlyHit, mazeHit, healHit;
    std::vector<std::string> log;
    bool T() const { return frame >= terminateAt; }
    bool at(const std::vector<int>& v) const { return frame < (int)v.size() && v[frame] != 0; }
};

static void Befriend(World& w)
{
    for (size_t i = 0; i < w.creatures.size(); ++i) {
        const std::string& d = w.creatures[i].def;
        if (d != "CREATURE_BIRD_GUILD_SPARROW" && d != "CREATURE_RIVAL_HERO_WHISPER_APPRENTICE"
            && d != "CREATURE_RIVAL_HERO_WHISPER_TEEN_APPRENTICE" && d != "CREATURE_RIVAL_HERO_MAZE") {
            w.creatures[i].friendly = true; w.creatures[i].killable = false;
        }
    }
}

static void CheckFriendlyAttacks(World& w)
{
    Befriend(w);
    for (; !w.T(); ++w.frame) {
        if (!w.at(w.inComplex)) {
            while (!w.at(w.inComplex)) { ++w.frame; if (w.T()) return; }
            Befriend(w); w.log.push_back("REAPPLY");
        }
        if (w.at(w.friendlyHit) && w.at(w.inComplex) && !w.at(w.mazeHit) && !w.at(w.healHit)) {
            if (w.warnings < 3) {
                static const char* text[3] = { "FIRST", "SECOND", "THIRD" };
                w.log.push_back(text[w.warnings]);
                ++w.warnings;
            } else {
                w.warningOccuring = true;
                if (!w.skillTest) { w.log.push_back("FOURTH"); w.log.push_back("CS_GUILD_BADHERO"); }
                else { w.log.push_back("WAIT_TEST"); w.skillTest = false; }
                w.warnings = 0;
                w.warningOccuring = false;
            }
        }
    }
}

int main()
{
    World w = World();
    w.terminateAt = 20;
    Creature c1 = { "CREATURE_GUILD_GUARD", false, true }, c2 = { "CREATURE_BIRD_GUILD_SPARROW", false, true },
             c3 = { "CREATURE_RIVAL_HERO_MAZE", false, true };
    w.creatures.push_back(c1); w.creatures.push_back(c2); w.creatures.push_back(c3);
    w.inComplex.assign(30, 1); w.friendlyHit.assign(30, 0); w.mazeHit.assign(30, 0); w.healHit.assign(30, 0);
    w.friendlyHit[1] = w.friendlyHit[2] = w.friendlyHit[3] = w.friendlyHit[5] = 1;
    w.friendlyHit[4] = 1; w.mazeHit[4] = 1;          // hitting the maze rival does not count
    w.inComplex[7] = w.inComplex[8] = 0;             // leaves and comes back: re-applied

    CheckFriendlyAttacks(w);
    bool ok = w.creatures[0].friendly && !w.creatures[0].killable && !w.creatures[1].friendly && !w.creatures[2].friendly
              && w.log.size() == 6 && w.log[0] == "FIRST" && w.log[1] == "SECOND" && w.log[2] == "THIRD" && w.log[3] == "FOURTH"
              && w.log[4] == "CS_GUILD_BADHERO" && w.log[5] == "REAPPLY" && w.warnings == 0 && !w.warningOccuring;
    if (!ok) { std::printf("00d45060_TEST FAIL\n"); return 1; }
    std::printf("00d45060_TEST PASS\n");
    return 0;
}
