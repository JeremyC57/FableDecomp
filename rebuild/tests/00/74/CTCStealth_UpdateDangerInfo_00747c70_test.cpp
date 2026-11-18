// Behaviour test for CTCStealth::UpdateDangerInfo (0x00747C70): dead danger things are removed (order kept); with danger things the
// attackers over the maximum (sorted, furthest first) are told not to attack and the cool-off restarts (frames from the def, 1 without
// a def); without danger things the cool-off counts down and clears the danger status when it goes negative; the running-away timer
// counts down while >= 0.
#include <cstdio>
#include <vector>
#include <algorithm>

struct Thing { int id; bool dead; bool targeting; float distFromInit; std::vector<int> notAttack; };
struct Danger { Thing* thing; int status; };
static long g_fps = 30;
struct Def { float DangerFeedbackHangtimeSeconds; };
struct Stealth {
    Thing self; Def* PDef; long RunningAwayFromCombatTimer, InDangerCoolOffTimer, MaxNumberOfDangerThings; int cleared;
    std::vector<Danger> DangerThings;
    Stealth() : PDef(0), RunningAwayFromCombatTimer(-1), InDangerCoolOffTimer(-1), MaxNumberOfDangerThings(-1), cleared(0) { self.id = 99; }
    static bool Dead(const Danger& d) { return d.thing == 0 || d.thing->dead || d.status == 0; }
    long GetNumberOfAttackersOverMaximum(std::vector<Thing*>* list) const
    {
        for (size_t i = 0; i < DangerThings.size(); ++i)
            if (DangerThings[i].status == 3 || DangerThings[i].status == 2) list->push_back(DangerThings[i].thing);
        long n = (long)list->size();
        if (MaxNumberOfDangerThings != -1 && n > MaxNumberOfDangerThings) return n - MaxNumberOfDangerThings;
        return -1;
    }
    float GetDangerFeedbackHangtimeFrames() const { return PDef ? g_fps * PDef->DangerFeedbackHangtimeSeconds : 1.0f; }
    static bool Further(const Thing* a, const Thing* b) { return a->distFromInit > b->distFromInit; }
    void ClearDangerStatus() { ++cleared; InDangerCoolOffTimer = -1; }
    void UpdateDangerInfo()
    {
        if (!DangerThings.empty()) {
            DangerThings.erase(std::remove_if(DangerThings.begin(), DangerThings.end(), Dead), DangerThings.end());
            std::vector<Thing*> real_threat_list;
            long num_to_send_home = GetNumberOfAttackersOverMaximum(&real_threat_list);
            if (num_to_send_home > 0 && !real_threat_list.empty()) {
                std::sort(real_threat_list.begin(), real_threat_list.end(), Further);
                std::vector<Thing*>::iterator it = real_threat_list.begin();
                do {
                    Thing* pthing = *it++;
                    if (pthing->targeting) pthing->notAttack.push_back(self.id);
                } while (--num_to_send_home > 0);
            }
            InDangerCoolOffTimer = (long)GetDangerFeedbackHangtimeFrames();
        } else if (InDangerCoolOffTimer >= 0) {
            if (--InDangerCoolOffTimer < 0) ClearDangerStatus();
        }
        if (RunningAwayFromCombatTimer >= 0) --RunningAwayFromCombatTimer;
    }
};
static int fail(int n) { std::printf("00747c70_TEST FAIL %d\n", n); return 1; }
int main()
{
    Thing a = { 1, false, true, 5.0f }, b = { 2, false, true, 9.0f }, c = { 3, true, true, 7.0f }, d = { 4, false, false, 8.0f }, e = { 5, false, true, 1.0f };
    Stealth s; Def def = { 2.0f }; s.PDef = &def; s.MaxNumberOfDangerThings = 1; s.RunningAwayFromCombatTimer = 1;
    Danger list[] = { { &a, 3 }, { &c, 3 }, { &b, 2 }, { &d, 3 }, { &e, 1 } };
    for (int i = 0; i < 5; ++i) s.DangerThings.push_back(list[i]);
    s.UpdateDangerInfo();
    if (s.DangerThings.size() != 4 || s.DangerThings[1].thing != &b) return fail(1);           // c (dead) removed, order kept
    // attackers a(5), b(9), d(8): 3 over max 1 -> 2 sent home: furthest first b, d (d has no targeting)
    if (b.notAttack.size() != 1 || b.notAttack[0] != 99 || !a.notAttack.empty() || !e.notAttack.empty() || !d.notAttack.empty()) return fail(2);   // d: no targeting
    if (s.InDangerCoolOffTimer != 60 || s.RunningAwayFromCombatTimer != 0) return fail(3);
    s.PDef = 0; s.UpdateDangerInfo();
    if (s.InDangerCoolOffTimer != 1 || s.RunningAwayFromCombatTimer != -1) return fail(4);
    s.DangerThings.clear(); s.UpdateDangerInfo();
    if (s.InDangerCoolOffTimer != 0 || s.cleared || s.RunningAwayFromCombatTimer != -1) return fail(5);   // timer stays at -1
    s.UpdateDangerInfo();
    if (s.cleared != 1 || s.InDangerCoolOffTimer != -1) return fail(6);
    s.UpdateDangerInfo();
    if (s.cleared != 1) return fail(7);
    Thing f = { 6, false, true, 3.0f }; Stealth s2; s2.MaxNumberOfDangerThings = 5;     // attackers within the maximum (coordinator, audit e01)
    Danger one = { &f, 3 }; s2.DangerThings.push_back(one); s2.UpdateDangerInfo();
    if (!f.notAttack.empty() || s2.InDangerCoolOffTimer != 1) return fail(8);
    std::printf("00747c70_TEST PASS\n");
    return 0;
}