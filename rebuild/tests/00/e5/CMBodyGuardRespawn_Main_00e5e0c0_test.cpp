// Behaviour test for CM_BodyGuardRespawn::Main @ 0x00E5E0C0 (copy of the model with a stub world).
#include <cstdio>
#include <cstring>
class C3DVector { public: float x, y, z; };
static char g_def[40], g_tag[16];
static int g_created;
static bool g_done;
static long g_timer[4];
class CScriptThing { public:
    const char* data; bool real; C3DVector pos;
    const char* GetDataString() const { return data; }
    void SetDataString(const char* t) { if (real) std::strcpy(g_tag, t); }
    const C3DVector& GetPos() const { return pos; }
};
class CGameScriptInterface { public:
    long GetTimer(long index) const { return g_timer[index]; }
    CScriptThing CreateCreature(const char* def, const C3DVector&, const char* name, bool f) const
    { CScriptThing t; t.data = ""; t.real = !std::strcmp(name, "BodyGuard") && !f; std::strcpy(g_def, def); ++g_created; return t; }
};
static CGameScriptInterface g_gsi;
class CTimer { public: long TimerIndex; long GetTime() const { return g_gsi.GetTimer(TimerIndex); } };
struct CV_BodyGuardScript {
    CTimer HenchmanDeadRespawnTimer[4]; long HenchmanGlobalStatus[4];
    long GetHenchmanIndex(const char* code) { return code[0] - '0'; }
};
struct CM_BodyGuardRespawn {
    CGameScriptInterface* Game; CScriptThing Me; CV_BodyGuardScript* ParentClass;
    bool IsDone() { return g_done; }
    void Main()
    {
        long index = ParentClass->GetHenchmanIndex(Me.GetDataString());
        if (ParentClass->HenchmanGlobalStatus[index] != 1) return;
        if (ParentClass->HenchmanDeadRespawnTimer[index].GetTime() != 0) return;
        if (IsDone()) return;
        const char* def = ""; const char* suffix = "";
        switch (index) {
        case 0: def = "CREATURE_BODY_GUARD_WEAK"; suffix = "_EVIL1"; break;
        case 1: def = "CREATURE_BODY_GUARD"; suffix = "_EVIL2"; break;
        case 2: def = "CREATURE_BODY_GUARD_GOOD_WEAK"; suffix = "_GOOD1"; break;
        case 3: def = "CREATURE_BODY_GUARD_GOOD"; suffix = "_GOOD2"; break;
        default: return;
        }
        CScriptThing bodyguard = Game->CreateCreature(def, Me.GetPos(), "BodyGuard", false);
        bodyguard.SetDataString(suffix);
        ParentClass->HenchmanGlobalStatus[index] = 0;
    }
};
int main()
{
    int bad = 0;
    CV_BodyGuardScript q; CM_BodyGuardRespawn s; s.Game = &g_gsi; s.ParentClass = &q; s.Me.real = false;
    const char* defs[4] = { "CREATURE_BODY_GUARD_WEAK", "CREATURE_BODY_GUARD", "CREATURE_BODY_GUARD_GOOD_WEAK", "CREATURE_BODY_GUARD_GOOD" };
    const char* tags[4] = { "_EVIL1", "_EVIL2", "_GOOD1", "_GOOD2" };
    const char* codes[4] = { "0", "1", "2", "3" };
    for (int i = 0; i < 4; ++i) { q.HenchmanDeadRespawnTimer[i].TimerIndex = i; q.HenchmanGlobalStatus[i] = 1; g_timer[i] = 0; }
    g_done = false;
    for (int i = 0; i < 4; ++i) {
        s.Me.data = codes[i]; g_created = 0; s.Main();
        if (g_created != 1 || std::strcmp(g_def, defs[i]) || std::strcmp(g_tag, tags[i]) || q.HenchmanGlobalStatus[i] != 0) bad++;
    }
    s.Me.data = "1"; g_created = 0; s.Main(); if (g_created) bad++;                          // already alive
    q.HenchmanGlobalStatus[1] = 1; g_timer[1] = 5; s.Main(); if (g_created) bad++;           // timer running
    g_timer[1] = 0; g_done = true; s.Main(); if (g_created || q.HenchmanGlobalStatus[1] != 1) bad++;
    if (bad) { std::printf("00e5e0c0_TEST FAIL\n"); return 1; }
    std::printf("00e5e0c0_TEST PASS\n");
    return 0;
}