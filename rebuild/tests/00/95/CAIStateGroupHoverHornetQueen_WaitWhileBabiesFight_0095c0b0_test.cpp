// Behaviour test for CAIStateGroup_HoverHornetQueen::WaitWhileBabiesFight @ 0x0095C0B0: loop exit conditions.
#include <cstdio>
class CThing { public: bool dead; };
class CTCBoss { public: int helpers; int countCalls; bool ready;
    long CountLivingHelpers() { ++countCalls; return helpers; }
    bool IsReadyForPhaseChange() const { return ready; } };
struct Q {
    CThing* target; CThing* markers[2]; int markerCount; bool stopping; CTCBoss* boss; int keepCalls; int babiesDieAfter;
    bool IsStillRunnable() {
        if (!(target && !target->dead)) return false;
        if (markerCount > 0) { CThing* m = markers[0]; if (m == 0) return false; if (m->dead) return false; }
        if (stopping) return false;
        return true;
    }
    int KeepOutOfWay() { ++keepCalls; if (keepCalls >= babiesDieAfter) boss->helpers = 0; return 0; }
    int WaitWhileBabiesFight() {
        CTCBoss* b = boss; CTCBoss* current = b;
        while (IsStillRunnable()) {
            current = boss;
            if (current->CountLivingHelpers() <= 0 || b->IsReadyForPhaseChange()) break;
            KeepOutOfWay();
        }
        return 2;
    }
};
int main()
{
    CThing t = { false }, m = { false };
    CTCBoss boss = { 3, 0, false };
    Q q = { &t, { &m, 0 }, 1, false, &boss, 0, 4 };
    bool ok = q.WaitWhileBabiesFight() == 2 && q.keepCalls == 4 && boss.countCalls == 5;
    boss.helpers = 3; boss.countCalls = 0; q.keepCalls = 0; m.dead = true;
    ok = ok && q.WaitWhileBabiesFight() == 2 && q.keepCalls == 0 && boss.countCalls == 0;
    m.dead = false; boss.ready = true;
    ok = ok && q.WaitWhileBabiesFight() == 2 && q.keepCalls == 0 && boss.countCalls == 1;
    boss.ready = false; q.stopping = true; boss.countCalls = 0;
    ok = ok && q.WaitWhileBabiesFight() == 2 && boss.countCalls == 0;
    q.stopping = false; q.markerCount = 0; t.dead = true;
    ok = ok && q.WaitWhileBabiesFight() == 2 && boss.countCalls == 0;
    if (!ok) { std::printf("0095c0b0_TEST FAIL\n"); return 1; }
    std::printf("0095c0b0_TEST PASS\n");
    return 0;
}