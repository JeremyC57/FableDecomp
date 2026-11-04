// Behaviour test for 0x00d37330 CFH_BriarRose::Main
#include <cstdio>
class CScriptThing { public: int id; };
struct CHandle { bool scripting; int releases; bool IsScripting() { return scripting; } void Release() { scripting = false; ++releases; } };
class CGame { public:
    int frames, removes, attempts, succeedOn;
    void NewScriptFrame() { ++frames; }
    bool StartScriptingEntity(const CScriptThing&, CHandle& h, long priority) { ++attempts; if (attempts >= succeedOn && priority == 4) { h.scripting = true; return true; } return false; }
    void RemoveThing(const CScriptThing&, bool a, bool b) { if (!a && b) ++removes; }
};
struct CQ_FireHeartScript { CHandle seh_BriarRose; bool HasScytheCutscenePlayed; };
struct CFH_BriarRose {
    CGame* Game; CScriptThing Me; CQ_FireHeartScript* ParentClass; int doneAfter, doneChecks;
    bool IsDone() { return ++doneChecks > doneAfter; }
    void Main()
    {
        if (ParentClass->HasScytheCutscenePlayed) { if (IsDone()) return; Game->RemoveThing(Me, false, true); return; }
        CHandle* handle = &ParentClass->seh_BriarRose;
        if (handle->IsScripting()) handle->Release();
        while (!Game->StartScriptingEntity(Me, ParentClass->seh_BriarRose, 4)) { Game->NewScriptFrame(); if (IsDone()) return; }
        IsDone();
    }
};
int main()
{
    CGame g = { 0, 0, 0, 3 }; CQ_FireHeartScript q; q.seh_BriarRose.scripting = true; q.seh_BriarRose.releases = 0; q.HasScytheCutscenePlayed = false;
    CFH_BriarRose b; b.Game = &g; b.Me.id = 1; b.ParentClass = &q; b.doneAfter = 100; b.doneChecks = 0;
    b.Main();
    bool ok = q.seh_BriarRose.releases == 1 && g.attempts == 3 && g.frames == 2 && q.seh_BriarRose.scripting && b.doneChecks == 3;
    q.HasScytheCutscenePlayed = true; b.Main(); ok = ok && g.removes == 1;
    b.doneAfter = 0; b.doneChecks = 0; b.Main(); ok = ok && g.removes == 1;   // script ending: nothing removed
    if (!ok) { std::printf("00d37330_TEST FAIL\n"); return 1; }
    std::printf("00d37330_TEST PASS\n");
    return 0;
}