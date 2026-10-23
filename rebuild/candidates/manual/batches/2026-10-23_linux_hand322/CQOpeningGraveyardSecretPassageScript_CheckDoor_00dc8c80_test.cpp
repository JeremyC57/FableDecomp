#include <cstdio>
#include <cstring>
// behaviour test: CQ_OpeningGraveyardSecretPassageScript::CheckDoor -- lock the gate, re-trigger the generator each time
// the hero returns to the graveyard path until the lieutenant appears and dies, then the path-door cut-scene and door swap
static char g_log[2048];
static void L(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
static int g_frames = 0, g_termAt = 1000;
struct Game {
    int lieutenantAt, lieutenantDeadAt;
    void NewScriptFrame() { ++g_frames; }
    bool PathLoaded() const { return !(g_frames >= 3 && g_frames < 6) && !(g_frames >= 12 && g_frames < 14); }
    bool LieutenantAlive() const { return g_frames < lieutenantDeadAt; }
    void Trigger() { L("trigger"); }
    void Persistent(const char* n) { L(n); }
    void Usable(bool u) { L(u ? "usable" : "locked"); }
};
struct Script {
    Game* G; bool DoorClosed; bool LieutenantAround;
    bool IsActiveThreadTerminating() { if (g_frames >= G->lieutenantAt) LieutenantAround = true; return g_frames >= g_termAt; }
    void CheckDoor()
    {
        G->Trigger(); G->Persistent("GraveyardPathGenerator");
        DoorClosed = true; G->Usable(false);
        while (!LieutenantAround) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (!G->PathLoaded()) {
                if (IsActiveThreadTerminating()) return;
                while (!G->PathLoaded()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                G->Trigger();
            }
        }
        if (IsActiveThreadTerminating()) return;
        while (G->LieutenantAlive()) {
            G->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
            if (!G->PathLoaded()) {
                if (IsActiveThreadTerminating()) return;
                while (!G->PathLoaded()) { G->NewScriptFrame(); if (IsActiveThreadTerminating()) return; }
                if (IsActiveThreadTerminating()) return;
                G->Trigger();
            }
        }
        if (IsActiveThreadTerminating()) return;
        L("CS_OPENGRAVE_PATHDOOR"); L("objective"); L("fade"); L("remove");
        G->Usable(true); G->Persistent("GraveyardPathNewDoor");
        DoorClosed = false;
    }
};
int main()
{
    Game g; g.lieutenantAt = 8; g.lieutenantDeadAt = 16;
    Script s; s.G = &g; s.DoorClosed = false; s.LieutenantAround = false;
    s.CheckDoor();
    const char* want = "trigger;GraveyardPathGenerator;locked;trigger;trigger;CS_OPENGRAVE_PATHDOOR;objective;fade;remove;usable;GraveyardPathNewDoor;";
    bool ok = std::strcmp(g_log, want) == 0 && !s.DoorClosed && g_frames == 16;
    g_log[0] = 0; g_frames = 0; g_termAt = 4; s.LieutenantAround = false; s.CheckDoor();
    ok = ok && std::strcmp(g_log, "trigger;GraveyardPathGenerator;locked;") == 0 && s.DoorClosed;
    if (!ok) { std::printf("00dc8c80_TEST FAIL %s %d\n", g_log, g_frames); return 1; }
    std::printf("00dc8c80_TEST PASS\n");
    return 0;
}
