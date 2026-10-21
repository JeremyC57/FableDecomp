#include <cstdio>
// Behaviour copy of CAIStateGroup_GoToBed::IsUnreportedCrimeKnown (0x0092D060).
struct Thing { bool dead; };
struct Opinion {
    bool crimeToRun; int ongoingType; Thing* ongoingTarget; int queries;
    bool FindCrime() { ++queries; return crimeToRun; }
    bool FindOngoing(int type, Thing* target) { ++queries; return type == ongoingType && target == ongoingTarget; }
};
struct Creature { bool isVillageMember; bool inVillage; bool hasOpinion; Opinion* opinion; Thing* home; };
static bool IsUnreportedCrimeKnown(Creature* c) {
    if (c->isVillageMember && c->inVillage && c->hasOpinion) {
        Opinion* opinion = c->opinion;
        if (opinion->FindCrime()) return true;
        Thing* home = c->home;
        if (home && !home->dead) {
            if (opinion->FindOngoing(10, c->home) || opinion->FindOngoing(11, c->home) || opinion->FindOngoing(2, c->home)) return true;
        }
    }
    return false;
}
int main() {
    Thing home = { false };
    Opinion o = { false, 2, &home, 0 };
    Creature c = { true, true, true, &o, &home };
    if (!IsUnreportedCrimeKnown(&c) || o.queries != 4) { std::printf("0092d060_TEST FAIL\n"); return 1; }
    o.ongoingType = 5; o.queries = 0;
    if (IsUnreportedCrimeKnown(&c) || o.queries != 4) { std::printf("0092d060_TEST FAIL\n"); return 1; }
    o.crimeToRun = true; o.queries = 0;
    if (!IsUnreportedCrimeKnown(&c) || o.queries != 1) { std::printf("0092d060_TEST FAIL\n"); return 1; }
    c.inVillage = false; o.queries = 0;
    if (IsUnreportedCrimeKnown(&c) || o.queries != 0) { std::printf("0092d060_TEST FAIL\n"); return 1; }
    c.inVillage = true; o.crimeToRun = false; o.ongoingType = 2; home.dead = true; o.queries = 0;
    if (IsUnreportedCrimeKnown(&c) || o.queries != 1) { std::printf("0092d060_TEST FAIL\n"); return 1; }
    std::printf("0092d060_TEST PASS\n"); return 0;
}