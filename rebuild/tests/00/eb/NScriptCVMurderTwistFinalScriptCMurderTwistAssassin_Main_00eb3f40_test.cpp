// Behaviour test for NScript::CV_MurderTwistFinalScript::CMurderTwistAssassin::Main 0x00EB3F40: the same control flow run through
// a small mock of the script interface (frame counter, terminate flag, scripted-entity handle, parent AssassinState).
#include <cstdio>
#include <cstring>

struct Sim {
    int frames;            // NewScriptFrame calls so far
    int terminateAtFrame;  // IsActiveThreadTerminating() is true once frames >= this
    int startFailures;     // StartScriptingEntity fails this many times first
    bool handleValid;
    int state;             // parent AssassinState
    int watchCalls, remindCalls, attackCalls, releaseCalls, startCalls, trustCalls, predicateSet;
    int watchesUntilState3, remindsUntilState2;
    char log[128];
    void Note(char c) { size_t n = std::strlen(log); if (n + 1 < sizeof(log)) { log[n] = c; log[n + 1] = 0; } }
    void NewScriptFrame() { ++frames; }
    bool IsActiveThreadTerminating() const { return frames >= terminateAtFrame; }
    bool StartScriptingEntity(int priority) { ++startCalls; Note('S'); if (priority != 4) Note('!'); if (startFailures > 0) { --startFailures; return false; } handleValid = true; return true; }
    bool DoesAssassinTrustHero() { ++trustCalls; Note('T'); return (frames & 1) != 0; }
    void DoWatchingForTrouble() { ++watchCalls; Note('W'); if (watchCalls == 1 && state == 0) state = 1; if (watchCalls >= watchesUntilState3) state = 3; }
    void DoRemindingHeroToKillSheriff() { ++remindCalls; Note('R'); if (remindCalls >= remindsUntilState2) state = 2; }
    void AttackHero() { ++attackCalls; Note('A'); }
    void Release() { ++releaseCalls; Note('L'); handleValid = false; }
};

static void Main(Sim& s)
{
    s.predicateSet++;
    s.NewScriptFrame();
    if (s.IsActiveThreadTerminating()) return;
    if (s.handleValid) s.Release();
    while (!s.StartScriptingEntity(4)) {
        s.NewScriptFrame();
        if (s.IsActiveThreadTerminating()) return;
    }
    if (s.IsActiveThreadTerminating()) return;
    bool trust = s.DoesAssassinTrustHero();
    while (s.state == 0 || s.state == 1) {
        s.NewScriptFrame();
        if (s.IsActiveThreadTerminating()) return;
        if (trust) {
            if (s.IsActiveThreadTerminating()) return;
            s.DoWatchingForTrouble();
        }
        else {
            if (s.IsActiveThreadTerminating()) return;
            s.DoWatchingForTrouble();
        }
    }
    if (s.IsActiveThreadTerminating()) return;
    while (s.state == 3) {
        s.NewScriptFrame();
        if (s.IsActiveThreadTerminating()) return;
        s.DoRemindingHeroToKillSheriff();
    }
    if (s.IsActiveThreadTerminating()) return;
    if (s.state == 2) {
        if (s.IsActiveThreadTerminating()) return;
        s.AttackHero();
    }
    if (s.IsActiveThreadTerminating()) return;
    for (;;) {
        s.NewScriptFrame();
        if (s.IsActiveThreadTerminating()) return;
    }
}

static Sim Make(int terminateAtFrame, int startFailures, bool handleValid, int state, int w3, int r2)
{
    Sim s;
    std::memset(&s, 0, sizeof(s));
    s.terminateAtFrame = terminateAtFrame; s.startFailures = startFailures; s.handleValid = handleValid; s.state = state;
    s.watchesUntilState3 = w3; s.remindsUntilState2 = r2;
    return s;
}

int main()
{
    bool ok = true;
    // 1. terminating on the first frame: nothing but the predicate and one frame
    { Sim s = Make(1, 0, true, 0, 3, 2); Main(s);
      ok = ok && s.predicateSet == 1 && s.frames == 1 && std::strcmp(s.log, "") == 0 && s.handleValid; }
    // 2. full run: stale handle released, two failed starts, three watches (state 0 -> 1 -> 3), two reminders (-> 2), attack, idle to frame 40
    { Sim s = Make(40, 2, true, 0, 3, 2); Main(s);
      ok = ok && std::strcmp(s.log, "LSSSTWWWRRA") == 0 && s.frames == 40 && s.trustCalls == 1 && s.state == 2 && s.handleValid; }
    // 3. state already 2: no watching, no reminding, straight to the attack
    { Sim s = Make(10, 0, false, 2, 3, 2); Main(s);
      ok = ok && std::strcmp(s.log, "STA") == 0 && s.frames == 10 && s.releaseCalls == 0; }
    // 4. terminating inside the watch loop: the frame that terminates does not watch, and nothing later runs
    { Sim s = Make(4, 0, false, 0, 99, 2); Main(s);
      ok = ok && std::strcmp(s.log, "STWW") == 0 && s.frames == 4 && s.attackCalls == 0 && s.remindCalls == 0; }
    // 5. terminating while the entity cannot be scripted: the trust question is never asked
    { Sim s = Make(3, 99, false, 0, 3, 2); Main(s);
      ok = ok && std::strcmp(s.log, "SS") == 0 && s.trustCalls == 0 && s.frames == 3; }
    // 6. state 3 from the start: reminders only, then (state 2) the attack
    { Sim s = Make(20, 0, false, 3, 3, 4); Main(s);
      ok = ok && std::strcmp(s.log, "STRRRRA") == 0 && s.frames == 20; }
    // 7. a state outside 0..3 goes straight to the idle loop
    { Sim s = Make(6, 0, false, 7, 3, 2); Main(s);
      ok = ok && std::strcmp(s.log, "ST") == 0 && s.frames == 6; }
    if (!ok) { std::printf("00eb3f40_TEST FAIL\n"); return 1; }
    std::printf("00eb3f40_TEST PASS\n");
    return 0;
}