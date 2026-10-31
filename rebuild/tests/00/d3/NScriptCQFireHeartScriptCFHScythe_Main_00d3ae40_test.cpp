// Behaviour test for CQ_FireHeartScript::CFH_Scythe::Main (0x00D3AE40): a copy of the control flow over a recording fake
// game interface -- stale handle released, StartScriptingEntity retried a frame at a time, fade only when the hero is far,
// the cutscene / flag / boasting / persistence / removal tail, and every IsActiveThreadTerminating early exit.
#include <cstdio>
#include <cstring>

struct Log { char text[512]; void add(const char* s) { std::strcat(text, s); std::strcat(text, ","); } };

struct Handle { bool valid; Log* log; bool IsValid() const { return valid; } void Release() { valid = false; log->add("release"); } };

struct Game {
    Log* log; int failStarts; bool heroNear;
    bool StartScriptingEntity(Handle& h) { log->add("start"); if (failStarts > 0) { --failStarts; return false; } h.valid = true; return true; }
    void NewScriptFrame() { log->add("frame"); }
    bool IsDistanceUnder(float d) { return heroNear && d == 50.0f; }
    void FadeScreenIn() { log->add("fade"); }
    void DeactivateBoastUi() { log->add("noboastui"); }
    void SetBoastingEnabled(bool) { log->add("boast"); }
    void SetThingPersistent(bool) { log->add("persist"); }
    void RemoveThing() { log->add("remove"); }
};

struct Parent { Handle seh_Scythe; bool HasScytheCutscenePlayed; Log* log; void PlayCutscene(const char* n) { log->add(n); } };

struct Scythe {
    Game* game; Parent* parent; int terminateAfter; int checks;
    bool IsActiveThreadTerminating() { return ++checks > terminateAfter; }
    void Main()
    {
        Parent* p = parent;
        if (p->seh_Scythe.IsValid())
            p->seh_Scythe.Release();
        while (!game->StartScriptingEntity(parent->seh_Scythe)) {
            game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!game->IsDistanceUnder(50.0f)) {
            if (IsActiveThreadTerminating())
                return;
            game->FadeScreenIn();
        }
        game->DeactivateBoastUi();
        parent->PlayCutscene("CS_FIREHEART_SCYTHE");
        parent->HasScytheCutscenePlayed = true;
        game->SetBoastingEnabled(true);
        game->SetThingPersistent(true);
        game->RemoveThing();
    }
};

static bool run(bool valid, int failStarts, bool near, int terminateAfter, const char* expect, bool played)
{
    Log log; log.text[0] = 0;
    Game g; g.log = &log; g.failStarts = failStarts; g.heroNear = near;
    Parent p; p.seh_Scythe.valid = valid; p.seh_Scythe.log = &log; p.HasScytheCutscenePlayed = false; p.log = &log;
    Scythe s; s.game = &g; s.parent = &p; s.terminateAfter = terminateAfter; s.checks = 0;
    s.Main();
    return std::strcmp(log.text, expect) == 0 && p.HasScytheCutscenePlayed == played;
}

int main()
{
    if (!run(true, 1, false, 10, "release,start,frame,start,fade,noboastui,CS_FIREHEART_SCYTHE,boast,persist,remove,", true) ||
        !run(false, 0, true, 10, "start,noboastui,CS_FIREHEART_SCYTHE,boast,persist,remove,", true) ||
        !run(false, 3, true, 1, "start,frame,start,frame,", false) ||
        !run(false, 0, false, 1, "start,", false)) {
        std::printf("00d3ae40_TEST FAIL\n");
        return 1;
    }
    std::printf("00d3ae40_TEST PASS\n");
    return 0;
}