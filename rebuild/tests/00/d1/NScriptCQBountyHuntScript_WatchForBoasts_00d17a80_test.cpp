// Behaviour test for 0x00D17A80 NScript::CQ_BountyHuntScript::WatchForBoasts: the same logic through a small copy of the model
// (script, game interface, timer, boast list); checks the two boast arms, the spawn, the timer-expiry flag and the terminate exits.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <utility>

struct Game {
    std::vector<std::vector<std::pair<long, std::string> > > frames;   // boasts reported per frame
    unsigned frame;
    long timer;
    int newFrames;
    Game() : frame(0), timer(100), newFrames(0) {}
    void MsgOnBoastsMade(std::vector<std::pair<long, std::string> >& out) {
        if (frame < frames.size()) out = frames[frame];
    }
    void NewScriptFrame() { ++frame; ++newFrames; }
    long GetTimer() const { return timer; }
};
struct MasterData { bool BountyHuntTimeLimitExceeded; MasterData() : BountyHuntTimeLimitExceeded(false) {} };
struct Script {
    Game* game; MasterData* master;
    bool BoastActiveTimeLimit, BoastActiveDecapitation;
    int spawned; std::string spawnedName;
    int terminateAfterChecks;      // IsActiveThreadTerminating turns true after this many calls (-1: never by count)
    unsigned stopAtFrame;          // ... or once this frame is reached
    int checks;
    std::string lastBoastName;
    Script(Game* g, MasterData* m) : game(g), master(m), BoastActiveTimeLimit(false), BoastActiveDecapitation(false), spawned(0),
        terminateAfterChecks(-1), stopAtFrame(0), checks(0) {}
    bool IsActiveThreadTerminating() {
        ++checks;
        if (terminateAfterChecks >= 0 && checks > terminateAfterChecks) return true;
        return game->frame >= stopAtFrame;
    }
    void AddSpawnedFunction(const char* name) { ++spawned; spawnedName = name; }
    void WatchForBoasts() {
        std::string boastName;
        while (!IsActiveThreadTerminating()) {
            {
                std::vector<std::pair<long, std::string> > boasts;
                game->MsgOnBoastsMade(boasts);
                for (unsigned long i = 0; i != boasts.size(); ++i) {
                    if (IsActiveThreadTerminating())
                        return;
                    long boast = boasts[i].first;
                    boastName = boasts[i].second;
                    lastBoastName = boastName;
                    if (boast == 15) {
                        if (!BoastActiveTimeLimit) {
                            if (IsActiveThreadTerminating())
                                return;
                            BoastActiveTimeLimit = true;
                            AddSpawnedFunction("WatchForQuestInfoDisplayConditions");
                        }
                    } else if (boast == 16) {
                        if (!BoastActiveDecapitation) {
                            if (IsActiveThreadTerminating())
                                return;
                            BoastActiveDecapitation = true;
                        }
                    }
                    if (BoastActiveTimeLimit && game->GetTimer() <= 0 && !master->BountyHuntTimeLimitExceeded) {
                        if (IsActiveThreadTerminating())
                            return;
                        master->BountyHuntTimeLimitExceeded = true;
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            game->NewScriptFrame();
        }
    }
};
static std::vector<std::pair<long, std::string> > one(long id, const char* name) {
    std::vector<std::pair<long, std::string> > v; v.push_back(std::make_pair(id, std::string(name))); return v;
}
int main()
{
    bool ok = true;
    {   // both boasts over three frames, timer still running: flags armed, one spawn, no expiry
        Game g; MasterData m; Script s(&g, &m);
        g.frames.push_back(one(16, "decap")); g.frames.push_back(one(15, "time")); g.frames.push_back(one(15, "time"));
        s.stopAtFrame = 3; s.WatchForBoasts();
        ok = ok && s.BoastActiveDecapitation && s.BoastActiveTimeLimit && s.spawned == 1 && s.spawnedName == "WatchForQuestInfoDisplayConditions";
        ok = ok && !m.BountyHuntTimeLimitExceeded && g.newFrames == 3 && s.lastBoastName == "time";
    }
    {   // timer already run out when the time-limit boast arrives: expiry flag set in the same pass
        Game g; MasterData m; Script s(&g, &m);
        g.timer = 0; g.frames.push_back(one(15, "time"));
        s.stopAtFrame = 1; s.WatchForBoasts();
        ok = ok && s.BoastActiveTimeLimit && m.BountyHuntTimeLimitExceeded && s.spawned == 1 && !s.BoastActiveDecapitation;
    }
    {   // an unrelated boast (7) changes nothing; expiry needs the time-limit boast
        Game g; MasterData m; Script s(&g, &m);
        g.timer = -5; g.frames.push_back(one(7, "other"));
        s.stopAtFrame = 1; s.WatchForBoasts();
        ok = ok && !s.BoastActiveTimeLimit && !s.BoastActiveDecapitation && !m.BountyHuntTimeLimitExceeded && s.spawned == 0;
    }
    {   // terminate at the for-top check (2nd call): nothing is read or armed, no new frame
        Game g; MasterData m; Script s(&g, &m);
        g.frames.push_back(one(15, "time")); s.stopAtFrame = 9; s.terminateAfterChecks = 1; s.WatchForBoasts();
        ok = ok && !s.BoastActiveTimeLimit && s.spawned == 0 && g.newFrames == 0 && s.lastBoastName.empty();
    }
    {   // terminate at the time-limit arm check (3rd call): name copied, flag NOT armed, no spawn
        Game g; MasterData m; Script s(&g, &m);
        g.frames.push_back(one(15, "time")); s.stopAtFrame = 9; s.terminateAfterChecks = 2; s.WatchForBoasts();
        ok = ok && !s.BoastActiveTimeLimit && s.spawned == 0 && s.lastBoastName == "time" && g.newFrames == 0;
    }
    {   // terminate at the expiry check: time-limit armed and spawned, expiry flag NOT set
        Game g; MasterData m; Script s(&g, &m);
        g.timer = 0; g.frames.push_back(one(15, "time")); s.stopAtFrame = 9; s.terminateAfterChecks = 3; s.WatchForBoasts();
        ok = ok && s.BoastActiveTimeLimit && s.spawned == 1 && !m.BountyHuntTimeLimitExceeded && g.newFrames == 0;
    }
    {   // terminate at the post-loop check: no NewScriptFrame
        Game g; MasterData m; Script s(&g, &m);
        g.frames.push_back(one(16, "decap")); s.stopAtFrame = 9; s.terminateAfterChecks = 3; s.WatchForBoasts();
        ok = ok && s.BoastActiveDecapitation && g.newFrames == 0;
    }
    if (!ok) { std::printf("00d17a80_TEST FAIL\n"); return 1; }
    std::printf("00d17a80_TEST PASS\n");
    return 0;
}