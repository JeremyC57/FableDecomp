// Behaviour test for 0x00D6D670 NScript::CQ_HangingTreeEvilScript::Conclusion(): the wait loop and the execution / escape
// decision, through a copy of the row's control flow over a scripted fake game (timer, executioner and prisoner states).
#include <cstdio>
#include <cstring>
struct FakeThing { bool alive; bool hitBy; bool killedBy; float distToHero; bool MsgIsHitBy() const { return hitBy; }
                   bool MsgIsKilledBy() const { return killedBy; } bool IsAlive() const { return alive; } bool IsDead() const { return !alive; } };
struct Model {
    bool ExecutionerInterrupted, AudienceLeggit, MissionFailed, MissionSucceeded;
    const char* ReasonForFailing; const char* cutscene;
    FakeThing Executioner, Prisoner;
    long timer; int frames; int hitOnFrame; int prisonerKilledOnFrame;
    void NewScriptFrame() { ++frames; --timer; Executioner.hitBy = (frames == hitOnFrame);
                            Prisoner.killedBy = (frames == prisonerKilledOnFrame); if (Prisoner.killedBy) Prisoner.alive = false; }
    void Conclusion()
    {
        bool timeUp = false;
        do {
            if (ExecutionerInterrupted)
                break;
            NewScriptFrame();
            if (timer <= 0) {
                NewScriptFrame();
                if (Executioner.IsAlive())
                    timeUp = true;
            }
            if (Executioner.MsgIsHitBy() || Executioner.MsgIsKilledBy() || Executioner.distToHero < 3.0f) {
                ExecutionerInterrupted = true;
                AudienceLeggit = true;
            }
            if (Prisoner.MsgIsKilledBy()) {
                ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED";
                MissionFailed = true;
            }
        } while (!timeUp);
        if (!ExecutionerInterrupted && Executioner.IsAlive() && Prisoner.IsAlive()) {
            cutscene = "CS_HANGINGTREE_EVIL_EXECUTION";
            ReasonForFailing = "TEXT_QST_029_FAILURE_PRISONER_KILLED";
            MissionFailed = true;
        } else {
            Executioner.alive = false;   // GiveThingBestEnemyTarget(Executioner, hero): wait until IsDead
            while (!Executioner.IsDead()) NewScriptFrame();
            cutscene = "CS_HANGINGTREE_EVIL_ESCAPE";
            MissionSucceeded = true;
        }
    }
};
static Model make(long timer, int hit, int pk)
{
    Model m; std::memset(&m, 0, sizeof m);
    m.ReasonForFailing = ""; m.cutscene = "";
    m.Executioner.alive = true; m.Executioner.distToHero = 100.0f; m.Prisoner.alive = true;
    m.timer = timer; m.hitOnFrame = hit; m.prisonerKilledOnFrame = pk;
    return m;
}
int main()
{
    Model a = make(5, -1, -1); a.Conclusion();     // timer runs out: execution, quest failed
    Model b = make(50, 3, -1); b.Conclusion();     // hero hits the executioner: escape, quest succeeded
    Model c = make(50, -1, 2); c.Executioner.distToHero = 100.0f; c.hitOnFrame = 4; c.Conclusion();   // prisoner killed, then hit
    // Pins the loop-head `if (ExecutionerInterrupted) break;`: after the hit on frame 3 no further frame runs (without the break the
    // loop would wait for the 50-frame timer).
    Model d = make(50, 3, -1); d.Conclusion();
    // Pins the `Prisoner.IsAlive()` term of the execution test: the prisoner is killed while the timer runs out and the executioner
    // stays alive and uninterrupted, so the execution cut-scene is skipped (escape path) and the failure from the kill stands.
    Model e = make(5, -1, 2); e.Conclusion();
    if (std::strcmp(a.cutscene, "CS_HANGINGTREE_EVIL_EXECUTION") != 0 || !a.MissionFailed || a.MissionSucceeded || a.AudienceLeggit
        || std::strcmp(b.cutscene, "CS_HANGINGTREE_EVIL_ESCAPE") != 0 || !b.MissionSucceeded || b.MissionFailed || !b.AudienceLeggit
        || b.frames < 3 || !c.MissionFailed || !c.ExecutionerInterrupted
        || std::strcmp(c.ReasonForFailing, "TEXT_QST_029_FAILURE_PRISONER_KILLED") != 0
        || d.frames != 3 || !d.ExecutionerInterrupted || !d.MissionSucceeded
        || e.ExecutionerInterrupted || !e.Executioner.IsDead() || std::strcmp(e.cutscene, "CS_HANGINGTREE_EVIL_ESCAPE") != 0
        || !e.MissionSucceeded || !e.MissionFailed || std::strcmp(e.ReasonForFailing, "TEXT_QST_029_FAILURE_PRISONER_KILLED") != 0) {
        std::printf("00d6d670_TEST FAIL\n");
        return 1;
    }
    std::printf("00d6d670_TEST PASS\n");
    return 0;
}