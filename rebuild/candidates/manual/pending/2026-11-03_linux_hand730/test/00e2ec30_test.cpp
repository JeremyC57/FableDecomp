// Behaviour test for NScript::CV_AmbushScamScript::GetTrickster (0x00E2EC30): the first living "AS_Trickster" whose data
// string matches the morality (Good when MoralitySwitch is set, else Evil); a null thing when none, the null thing when the
// thread is terminating.
#include <cstdio>
#include <cstring>
#include <vector>
struct Thing { int id; bool alive; const char* data; };
static int terminate_after = 1000;
static bool IsActiveThreadTerminating() { return --terminate_after < 0; }
static int GetTrickster(const std::vector<Thing>& things, bool morality_switch)
{
    for (long i = 0; i < (long)things.size(); ++i) {
        if (IsActiveThreadTerminating()) return 0;
        const Thing& thing = things[i];
        if (thing.alive) {
            if (IsActiveThreadTerminating()) return 0;
            if (morality_switch && std::strcmp(thing.data, "Good") == 0) { if (IsActiveThreadTerminating()) return 0; return thing.id; }
            if (!morality_switch && std::strcmp(thing.data, "Evil") == 0) { if (IsActiveThreadTerminating()) return 0; return thing.id; }
        }
    }
    if (IsActiveThreadTerminating()) return 0;
    return 0;
}
int main()
{
    std::vector<Thing> t;
    Thing a = { 1, false, "Good" }, b = { 2, true, "Evil" }, c = { 3, true, "Good" };
    t.push_back(a); t.push_back(b); t.push_back(c);
    if (GetTrickster(t, true) != 3 || GetTrickster(t, false) != 2) { std::printf("00e2ec30_TEST FAIL\n"); return 1; }
    std::vector<Thing> none; none.push_back(a);
    if (GetTrickster(none, true) != 0) { std::printf("00e2ec30_TEST FAIL\n"); return 1; }
    terminate_after = 0;
    if (GetTrickster(t, true) != 0) { std::printf("00e2ec30_TEST FAIL\n"); return 1; }
    std::printf("00e2ec30_TEST PASS\n");
    return 0;
}
