// Behaviour test for 0x00E5E350 NScript::CV_BodyGuardScript::HenchmanCommentary (links the REAL row object, including the out-of-line
// ~CTimer COMDAT the row calls at its idle-comment exit). Fixtures: the script object (Game +0x40, MasterScriptData +0x44,
// HeroWieldingWeapon +0x4D, HeroStoodStillCounter +0x50, CommentaryTimer +0x5C, HenchmanList begin/end +0x64/+0x68, 12-byte elements);
// a fake Game (slot 7 NewScriptFrame, 70 GetHero, 195 IsPlayerWieldingWeapon), the hero (slot 6 GetPos) and pDSTGame (timer slots 87..90:
// RegisterTimer / DeregisterTimer / SetTimer / GetTimer). IsActiveThreadTerminating, MakeHenchmanPerformIdleComment and
// CCounter::operator=(float) are recording __fastcall stubs bound by /alternatename. Each run checks the exact event sequence.
// HeroLastPosition is uninitialised in the row, as in retail: every run is primed by a first call (from the same frame, with no call in
// between) that leaves a known position in that stack slot.
#include <stdio.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@8=?HenchmanCommentary@CV_BodyGuardScript@NScript@@QAEXXZ")
#pragma comment(linker, "/alternatename:?DELTA_NOTIONAL_ZERO@@3MB=_g_delta")
#pragma comment(linker, "/alternatename:?pDSTGame@@3PAVCGameScriptInterfaceBase@@A=_g_pdst")
#pragma comment(linker, "/alternatename:?MakeHenchmanPerformIdleComment@CV_BodyGuardScript@NScript@@QAE_NXZ=@StubMake@8")
#pragma comment(linker, "/alternatename:??4CCounter@@QAEAAV0@M@Z=@StubCounterSet@12")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ=@StubTerm@8")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct V3 { float x, y, z; };
struct Obj { void** vt; };
struct Master { long a, b, commentaryState, override_; };
struct Script { void* vt; unsigned char p04[0x40 - 4]; Obj* game; Master* master; unsigned char p48[0x4d - 0x48]; bool wielding;
                unsigned char p4e[2]; float counter; unsigned char p54[0x5c - 0x54]; long commentaryTimer; unsigned char p60[4];
                char* listBegin; char* listEnd; char* listCap; unsigned char p70[0x94 - 0x70]; };
typedef char OffCheck[(size_t)&((Script*)0)->listBegin == 0x64 ? 1 : -1];
typedef char OffCheck2[(size_t)&((Script*)0)->counter == 0x50 ? 1 : -1];
struct State {                                     // everything the stubs read or write; copied by plain struct assignment
    Script s; Master m; long timers[16]; V3 pos[8]; char log[512];
    int tcalls, tTrueAt, frames, emptyFrames, posReads; long nextTimer; bool make, wield;
};

extern "C" const float g_delta; extern "C" Obj* g_pdst;
const float g_delta = 1e-4f;
static Obj g_dst, g_game, g_hero; Obj* g_pdst = &g_dst;
static void* g_dstVt[100]; static void* g_gameVt[200]; static void* g_heroVt[8];
static char g_list[3 * 12];
static State g_cur, g_next, g_prime;

static void Log(const char* fmt, long v) { char b[64]; sprintf(b, fmt, v); strcat(g_cur.log, b); }
extern "C" bool __fastcall StubTerm(Script* self, void*) { CHECK(self == &g_cur.s); ++g_cur.tcalls; Log("T%d ", g_cur.tcalls); return g_cur.tcalls == g_cur.tTrueAt; }
extern "C" bool __fastcall StubMake(Script* self, void*) { CHECK(self == &g_cur.s); Log("Make ", 0); return g_cur.make; }
extern "C" void* __fastcall StubCounterSet(float* self, void*, float v) { CHECK(self == &g_cur.s.counter && v == 0.0f); Log("Still0 ", 0); *self = -1.0f; return self; }
extern "C" long __fastcall Register(Obj* self, void*) { CHECK(self == &g_dst); long i = g_cur.nextTimer++; Log("Reg%d ", i); return i; }
extern "C" void __fastcall Deregister(Obj* self, void*, long i) { CHECK(self == &g_dst); Log("Dereg%d ", i); }
extern "C" void __fastcall SetTimer(Obj* self, void*, long i, long t) { CHECK(self == &g_dst); Log("Set%d", i); Log("=%d ", t); g_cur.timers[i] = t; }
extern "C" long __fastcall GetTimer(Obj* self, void*, long i) { CHECK(self == &g_dst); return g_cur.timers[i]; }
extern "C" void __fastcall NewFrame(Obj* self, void*) { CHECK(self == &g_game); ++g_cur.frames; Log("F ", 0); if (g_cur.frames >= g_cur.emptyFrames) g_cur.s.listEnd = g_cur.s.listBegin + 12; }
extern "C" Obj* __fastcall GetHero(Obj* self, void*) { CHECK(self == &g_game); return &g_hero; }
extern "C" bool __fastcall Wielding(const Obj* self, void*) { CHECK(self == &g_game); Log("Wield ", 0); return g_cur.wield; }
extern "C" const V3* __fastcall GetPos(const Obj* self, void*) { CHECK(self == &g_hero); return &g_cur.pos[g_cur.posReads++ & 7]; }
extern "C" void __fastcall CallRow(Script* self, void*);

static void Base(State& st, int tTrueAt, int emptyFrames)
{
    memset(&st, 0, sizeof st);
    st.tTrueAt = tTrueAt; st.emptyFrames = emptyFrames; st.nextTimer = 1;
    st.s.game = &g_game; st.s.master = &g_cur.m; st.s.commentaryTimer = 9; st.s.counter = 3.0f;
    st.s.listBegin = g_list; st.s.listEnd = emptyFrames > 0 ? g_list : g_list + 12; st.s.listCap = g_list + 36;
}
// Prime: one pass that only stores `last` as HeroLastPosition (a move from whatever is there is allowed: T3 returns false), then stops.
static void Prime(V3 last)
{
    Base(g_prime, 5, 0); g_prime.timers[1] = 7; g_prime.timers[2] = 7;
    for (int i = 0; i < 8; ++i) g_prime.pos[i] = last;
}
static void __declspec(noinline) RunPrimed()
{
    g_cur = g_prime; CallRow(&g_cur.s, 0);         // the slot now holds the primed position
    g_cur = g_next; CallRow(&g_cur.s, 0);          // no call between the two row calls at this depth
}
static void Expect(const char* want) { if (strcmp(g_cur.log, want) != 0) { g_fail = 1; printf("run %d\n  got  %s\n  want %s\n", g_run, g_cur.log, want); } }
static V3 P(float x, float y, float z) { V3 v; v.x = x; v.y = y; v.z = z; return v; }

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    g_dstVt[87] = (void*)Register; g_dstVt[88] = (void*)Deregister; g_dstVt[89] = (void*)SetTimer; g_dstVt[90] = (void*)GetTimer; g_dst.vt = g_dstVt;
    g_gameVt[7] = (void*)NewFrame; g_gameVt[70] = (void*)GetHero; g_gameVt[195] = (void*)Wielding; g_game.vt = g_gameVt;
    g_heroVt[6] = (void*)GetPos; g_hero.vt = g_heroVt;
    // 1: terminating at once: the two timers (Idle first) are registered and deregistered (Check first)
    g_run = 1; Prime(P(0, 0, 0)); Base(g_next, 1, 0); RunPrimed(); Expect("Reg1 Reg2 T1 Dereg2 Dereg1 ");
    // 2: no henchman for two frames, terminating while waiting
    g_run = 2; Prime(P(0, 0, 0)); Base(g_next, 3, 3); RunPrimed(); Expect("Reg1 Reg2 T1 F T2 F T3 Dereg2 Dereg1 ");
    // 3: one full pass: commentary state cleared (its timer is 0), the expensive check re-armed for 1 and the weapon flag read, the
    //    hero moved (stood-still counter reset), the idle timer expired and a comment made -> idle re-armed for 5; then a pass with
    //    nothing due (the timers read 1 / 5, the hero stands still) and the loop-end terminate check
    g_run = 3; Prime(P(0, 0, 0)); Base(g_next, 11, 0); g_next.m.commentaryState = 4; g_next.make = true; g_next.wield = true;
    for (int i = 0; i < 8; ++i) g_next.pos[i] = P(2, 0, 0);
    RunPrimed();
    Expect("Reg1 Reg2 T1 T2 T3 T4 Set2=1 Wield T5 Still0 T6 Make T7 Set1=5 F T8 T9 F T10 T11 Dereg2 Dereg1 ");
    CHECK(g_cur.m.commentaryState == 0 && g_cur.s.wielding == true && g_cur.s.counter == -1.0f);
    // 4: state set but the commentary timer still running: nothing cleared; a move under DELTA_NOTIONAL_ZERO is no move
    g_run = 4; Prime(P(1, 1, 1)); Base(g_next, 4, 0); g_next.m.commentaryState = 4; g_next.timers[9] = 3; g_next.timers[1] = 7; g_next.timers[2] = 7;
    for (int i = 0; i < 8; ++i) g_next.pos[i] = P(1.00005f, 0.99995f, 1);
    RunPrimed(); Expect("Reg1 Reg2 T1 T2 F T3 T4 Dereg2 Dereg1 ");
    CHECK(g_cur.m.commentaryState == 4 && g_cur.s.counter == 3.0f);
    // 5: a move of 0.5 on one axis alone counts (x, y, z in turn)
    for (int axis = 0; axis < 3; ++axis) {
        g_run = 50 + axis; Prime(P(1, 1, 1)); Base(g_next, 4, 0); g_next.timers[1] = 7; g_next.timers[2] = 7;
        for (int i = 0; i < 8; ++i) g_next.pos[i] = P(axis == 0 ? 1.5f : 1, axis == 1 ? 1.5f : 1, axis == 2 ? 1.5f : 1);
        RunPrimed(); Expect("Reg1 Reg2 T1 T2 T3 Still0 F T4 Dereg2 Dereg1 ");
    }
    // 6: the idle comment made and the thread terminating right after: the out-of-line ~CTimer exit (Check first, then Idle)
    g_run = 6; Prime(P(0, 0, 0)); Base(g_next, 4, 0); g_next.timers[2] = 7; g_next.make = true; RunPrimed();
    Expect("Reg1 Reg2 T1 T2 T3 Make T4 Dereg2 Dereg1 ");
    // 7: no comment made: idle timer not re-armed
    g_run = 7; Prime(P(0, 0, 0)); Base(g_next, 5, 0); g_next.timers[2] = 7; RunPrimed();
    Expect("Reg1 Reg2 T1 T2 T3 Make F T4 T5 Dereg2 Dereg1 ");
    // 8: terminating at each check of a full pass exits there (checks 2..7), always deregistering both timers last
    for (int k = 2; k <= 7; ++k) {
        g_run = 80 + k; Prime(P(0, 0, 0)); Base(g_next, k, 0); g_next.m.commentaryState = 1; g_next.make = true;
        for (int i = 0; i < 8; ++i) g_next.pos[i] = P(0, 1, 0);
        RunPrimed();
        const char* tail = strstr(g_cur.log, "Dereg2 Dereg1 ");
        CHECK(g_cur.tcalls == k && tail != 0 && strlen(tail) == 14);
    }
    if (!g_fail) printf("00e5e350_TEST PASS\n");
    return g_fail;
}