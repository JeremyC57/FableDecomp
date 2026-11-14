// Behaviour test for NScript::CV_AmbushScamScript::SpringAmbush (0x00E2F170): per frame, stop on thread termination; a dead trickster
// (not alive or health <= 0.0001) ends the wait (TricksterKilledBeforeAmbush unless terminating); the hero within the trigger
// proximity of the RUN-AWAY POINT springs the ambush (DoAmbush, HitCount = 0, six creatures of the morality's set, a conversation
// opened by the first two, the all-dead watcher), then waits for all ambushers dead or the trickster's death
// (TricksterKilledAfterAmbush). A state model of the generated script.
#include <cstdio>
#include <cstring>

struct Script {
    // world model
    int Frame, TerminateAt, TricksterDiesAt, HeroNearRunAwayAt, HeroNearTricksterAt, AmbushersDeadAt;
    bool MoralitySwitch;
    // script state
    bool DoAmbush, TricksterKilledBeforeAmbush, TricksterKilledAfterAmbush, AllAmbushersDead;
    long HitCount; int Created, Bandits, Guards, Conversations, Watchers;
    bool T() const { return Frame >= TerminateAt; }
    bool TricksterAlive() const { return Frame < TricksterDiesAt; }
    float TricksterHealth() const { return TricksterAlive() ? 50.0f : 0.0f; }
    bool HeroNearRunAway() const { return Frame >= HeroNearRunAwayAt; }
    void NewScriptFrame() { ++Frame; if (Watchers && Frame >= AmbushersDeadAt) AllAmbushersDead = true; }
    void SpringAmbush()
    {
        for (;; NewScriptFrame()) {
            if (T()) return;
            if (!TricksterAlive() || TricksterHealth() <= 0.0001f) break;
            if (HeroNearRunAway()) {
                if (T()) return;
                if (T()) return;
                DoAmbush = true; HitCount = 0;
                if (MoralitySwitch) { if (T()) return; Created += 6; Bandits += 6; }
                else { if (T()) return; Created += 6; Guards += 6; }
                ++Conversations; ++Watchers;
                for (;; NewScriptFrame()) {
                    if (T()) return;
                    if (AllAmbushersDead) { if (T()) return; break; }
                    if (!TricksterAlive() || TricksterHealth() <= 0.0001f) { if (T()) return; TricksterKilledAfterAmbush = true; break; }
                }
                return;
            }
        }
        if (T()) return;
        TricksterKilledBeforeAmbush = true;
    }
};

static Script Make(int terminateAt, int diesAt, int nearRunAway, int nearTrickster, int deadAt, bool morality)
{
    Script s; std::memset(&s, 0, sizeof s);
    s.TerminateAt = terminateAt; s.TricksterDiesAt = diesAt; s.HeroNearRunAwayAt = nearRunAway; s.HeroNearTricksterAt = nearTrickster;
    s.AmbushersDeadAt = deadAt; s.MoralitySwitch = morality; s.HitCount = 7;
    return s;
}

int main()
{
    bool ok = true;
    // good variant: hero reaches the run-away point at frame 3 (near the trickster from frame 0 is irrelevant), ambushers die at 6
    Script a = Make(100, 100, 3, 0, 6, true); a.SpringAmbush();
    ok = ok && a.DoAmbush && a.HitCount == 0 && a.Bandits == 6 && a.Guards == 0 && a.Conversations == 1 && a.Watchers == 1
            && a.AllAmbushersDead && !a.TricksterKilledAfterAmbush && !a.TricksterKilledBeforeAmbush && a.Frame == 6;
    // evil variant: the trickster dies during the fight
    Script b = Make(100, 5, 2, 0, 50, false); b.SpringAmbush();
    ok = ok && b.DoAmbush && b.Guards == 6 && b.TricksterKilledAfterAmbush && !b.TricksterKilledBeforeAmbush;
    // trickster dead before the hero arrives
    Script c = Make(100, 2, 10, 0, 50, true); c.SpringAmbush();
    ok = ok && !c.DoAmbush && c.TricksterKilledBeforeAmbush && c.Created == 0 && c.HitCount == 7;
    // terminated while waiting: nothing recorded
    Script d = Make(4, 100, 10, 0, 50, true); d.SpringAmbush();
    ok = ok && !d.DoAmbush && !d.TricksterKilledBeforeAmbush && d.Frame == 4;
    std::printf(ok ? "00e2f170_TEST PASS\n" : "00e2f170_TEST FAIL\n");
    return ok ? 0 : 1;
}
