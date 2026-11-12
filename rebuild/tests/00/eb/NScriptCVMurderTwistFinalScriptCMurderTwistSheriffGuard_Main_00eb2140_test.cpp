// Behaviour test for CMurderTwistSheriffGuard::Main 0x00EB2140: model of the state machine (which handler runs per frame, the
// Guards / seh_Guards registration at slot ID, termination), driven by a scripted parent state.
#include <cstdio>
#include <vector>
#include <string>
struct Parent { int SheriffState, AssassinState; std::vector<int> Guards, seh_Guards; };
struct Guard {
    Parent* ParentClass; unsigned long ID; int Me, Handle; int frames, termAt, startFailures; bool valid; int released;
    std::string log; std::string brain;
    bool Term() { return frames >= termAt; }
    void Frame() { ++frames; }
    bool Start() { if (startFailures > 0) { --startFailures; return false; } Handle = 77; return true; }
    void Step(const char* c, int s, int a) { log += c; ParentClass->SheriffState = s; ParentClass->AssassinState = a; }
    int seq; const int (*script)[2]; const char* (*names);
    void Handler(const char* n) { const int* p = script[seq++]; Step(n, p[0], p[1]); }
    void Main() {
        Frame(); if (Term()) return;
        brain = "BRAIN_MELEE_GUARD";
        if (valid) ++released;
        while (!Start()) { Frame(); if (Term()) return; }
        if (Term()) return;
        ParentClass->Guards[ID] = Me; ParentClass->seh_Guards[ID] = Handle;
        while (ParentClass->SheriffState == 0) { Frame(); if (Term()) return; Handler("A"); }
        if (Term()) return;
        while (ParentClass->AssassinState == 0 && ParentClass->SheriffState != 2) { Frame(); if (Term()) return; Handler("R"); }
        if (Term()) return;
        while (ParentClass->AssassinState != 4 && ParentClass->SheriffState != 2) { Frame(); if (Term()) return; Handler("G"); }
        if (Term()) return;
        while (ParentClass->AssassinState == 4 && ParentClass->SheriffState == 1) { Frame(); if (Term()) return; Handler("C"); }
        if (Term()) return;
        while (ParentClass->SheriffState == 3) { Frame(); if (Term()) return; Handler("T"); }
        if (Term()) return;
        if (ParentClass->SheriffState == 2) { if (Term()) return; log += "X"; }
        for (;;) { Frame(); if (Term()) return; }
    }
};
int main()
{
    static const int s1[][2] = { {0,0}, {1,0}, {1,1}, {1,4}, {3,4}, {0,4} };
    Parent p; p.SheriffState = 0; p.AssassinState = 0; p.Guards.assign(3, 0); p.seh_Guards.assign(3, 0);
    Guard g; g.ParentClass = &p; g.ID = 2; g.Me = 5; g.Handle = 0; g.frames = 0; g.termAt = 30; g.startFailures = 2;
    g.valid = true; g.released = 0; g.seq = 0; g.script = s1;
    g.Main();
    bool ok = g.log == "AARGCT" && p.Guards[2] == 5 && p.seh_Guards[2] == 77 && p.Guards[0] == 0 && g.released == 1
           && g.brain == "BRAIN_MELEE_GUARD" && g.frames == 30;
    // sheriff turns on the hero: attack, then idle
    static const int s2[][2] = { {2,0} };
    Parent q; q.SheriffState = 0; q.AssassinState = 0; q.Guards.assign(1, 0); q.seh_Guards.assign(1, 0);
    Guard h; h.ParentClass = &q; h.ID = 0; h.Me = 9; h.Handle = 0; h.frames = 0; h.termAt = 10; h.startFailures = 0;
    h.valid = false; h.released = 0; h.seq = 0; h.script = s2;
    h.Main();
    ok = ok && h.log == "AX" && q.Guards[0] == 9 && h.released == 0;
    // terminate during the start-scripting retry loop: no registration
    Parent r; r.SheriffState = 0; r.AssassinState = 0; r.Guards.assign(1, 0); r.seh_Guards.assign(1, 0);
    Guard k; k.ParentClass = &r; k.ID = 0; k.Me = 3; k.Handle = 0; k.frames = 0; k.termAt = 3; k.startFailures = 10;
    k.valid = false; k.released = 0; k.seq = 0; k.script = s2;
    k.Main();
    ok = ok && r.Guards[0] == 0 && k.log.empty();
    if (!ok) { std::printf("00eb2140_TEST FAIL\n"); return 1; }
    std::printf("00eb2140_TEST PASS\n");
    return 0;
}