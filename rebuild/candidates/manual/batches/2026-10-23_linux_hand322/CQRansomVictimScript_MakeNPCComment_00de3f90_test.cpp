#include <cstdio>
#include <string>
// behaviour test: CQ_RansomVictimScript::MakeNPCComment -- alive + timer gate, pre/post-rescue line, guard-comment
// response by the other NPC (victim when the guard speaks), timer restart, result
struct Thing { std::string name; bool alive; };
static Thing g_guard = { "VictimsFatherGuard", true }, g_victim = { "Victim", true };
static std::string g_lines[8];
static int g_nlines = 0, g_people = 0;
struct Script {
    bool RescuedVictim; long timer; long interval;
    bool Term() { return false; }
    bool TextExists(const std::string& t) { return t.find("GREET") != std::string::npos; }
    bool MakeNPCComment(const std::string& name, const Thing& npc, int comment)
    {
        Thing speaker = npc;
        if (!speaker.alive) { if (Term()) return false; return false; }
        if (timer > 0) { if (Term()) return false; return false; }
        ++g_people;
        if (RescuedVictim) g_lines[g_nlines++] = "TEXT_QST_B06_POST_RESC_" + speaker.name + "_" + name;
        else g_lines[g_nlines++] = "TEXT_QST_B06_PRE_RESC_" + speaker.name + "_" + name;
        if (comment == 1) {
            Thing guard = g_guard;
            Thing other = (guard.alive && speaker.name == guard.name) ? g_victim : g_guard;
            std::string resp = "TEXT_QST_B06_PRE_RESC_" + other.name + "_" + name + "_RESPONSE";
            if (other.alive && other.name != speaker.name && TextExists(resp)) {
                ++g_people;
                g_lines[g_nlines++] = resp;
            }
        }
        timer = interval;
        return true;
    }
};
int main()
{
    Script s; s.RescuedVictim = false; s.timer = 0; s.interval = 30;
    bool a = s.MakeNPCComment("GREET", g_guard, 1);
    bool b = s.MakeNPCComment("GREET", g_victim, 0);          // timer still running
    s.timer = 0; s.RescuedVictim = true;
    bool c = s.MakeNPCComment("BYE", g_victim, 1);            // victim speaks: guard answers? "BYE" has no response text
    Thing dead = { "Dead", false };
    long t = s.timer; s.timer = 0;
    bool d = s.MakeNPCComment("GREET", dead, 1);
    if (!a || b || !c || d || g_nlines != 3 || g_lines[0] != "TEXT_QST_B06_PRE_RESC_VictimsFatherGuard_GREET"
        || g_lines[1] != "TEXT_QST_B06_PRE_RESC_Victim_GREET_RESPONSE" || g_lines[2] != "TEXT_QST_B06_POST_RESC_Victim_BYE" || t != 30 || s.timer != 0) {
        std::printf("00de3f90_TEST FAIL %d %d %d %d %d\n", a, b, c, d, g_nlines); return 1;
    }
    std::printf("00de3f90_TEST PASS\n");
    return 0;
}
