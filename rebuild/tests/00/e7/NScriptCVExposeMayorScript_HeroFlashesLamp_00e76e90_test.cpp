// Behaviour test for 0x00E76E90 NScript::CV_ExposeMayorScript::HeroFlashesLamp(long) (links the REAL row object: the row body with its
// one-level expanded recursion, the real recursive calls back into 0x00E76E90, and the out-of-line CTimer ctor / dtor COMDATs all run as
// compiled). No class models: every external the row names is bound with /alternatename to an extern "C" __fastcall stub
// (thiscall-compatible, unused edx); the virtuals it reaches go through hand-built vtables (Game slots 7, 70, 72, 185; pDSTGame slots
// 87..90 on a SEPARATE object, so the timer calls are proved to use the global). The script object is a raw native-layout block
// (Game +0x40, HeroFlashedLightsCorrectly +0x50, ResetLamp +0x52, MaxFlashDistFromStables +0x9C).
// Reference model: Model() below, written from the script's statements (recursive, as the source); every run is compared event by event
// (terminate checks, frames, lamp queries, timer register / set / get / deregister with the timer index, the point lookup, the distance
// test, the stores, the point's release) plus the final state (flags, every timer deregistered, no live string or thing).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:?pDSTGame@@3PAVCGameScriptInterfaceBase@@A=_g_pDSTGame")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@ThingDtor@8")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@DistUnder@12")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ=@IsTerm@8")
extern "C" void __fastcall CallRow(void* self, void*, long timesLeft);   // the row (thiscall `ret 4`: this in ecx, edx unused)
#pragma comment(linker, "/alternatename:@CallRow@12=?HeroFlashesLamp@CV_ExposeMayorScript@NScript@@QAEXJ@Z")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static Script sTerm, sLit, sTime, sDist;          // past the end: terminating / unlit / timer run out / out of range
static long timeValue, stopValue;                  // what GetTimer returns while running (>0) / once run out (0 or negative)

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 20000 };
static char got[MAXEV + 1], want[MAXEV + 1]; static int ngot, nwant;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static void Want(char c) { if (nwant < MAXEV) want[nwant++] = c; }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0xA0]; double a; } self;                 // CV_ExposeMayorScript (0xA0)
static void* gameVtbl[300]; static void* gameObj[1];
static void* dstVtbl[100]; static void* dstObj[1];
extern "C" void* g_pDSTGame = dstObj;
static unsigned char hero[12];
static void* pointVtbl[1]; static int pointInfo;
static float maxDist = 7.75f;

// strings: one text pointer
static int liveStrings;
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len)
{
    *s = text; ++liveStrings; check(len == -1, "string length", 0); check(strcmp(text, "EM_FlashLightPoint") == 0, "point name", 0);
    Ev('n'); return s;
}
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; *s = 0; Ev('~'); }

// things: {vptr, Data, Info}
static int liveThings; static void* lastPoint;
extern "C" void __fastcall ThingDtor(void** t, void*)
{
    check(t == lastPoint && t[0] == (void*)pointVtbl && t[2] == (void*)&pointInfo, "released thing is the point", 0);
    --liveThings; t[0] = 0; Ev('x');
}
extern "C" bool __fastcall IsTerm(void* s, void*) { check(s == self.b, "terminate this", 0); bool r = Next(sTerm); Ev(r ? 'T' : 't'); return r; }
extern "C" bool __fastcall DistUnder(const void* a, const void* b, float d)
{
    check(a == hero, "distance: hero first", 0); check(b == lastPoint, "distance: the point second", 0); check(d == maxDist, "distance value", 0);
    bool r = Next(sDist); Ev(r ? 'D' : 'd'); return r;
}

// Game
extern "C" void __fastcall G_NewScriptFrame(void* g, void*) { check(g == gameObj, "frame this", 0); Ev('F'); }
extern "C" void* __fastcall G_GetHero(void* g, void*) { check(g == gameObj, "hero this", 0); Ev('h'); return hero; }
extern "C" void* __fastcall G_GetThingWithScriptName(void* g, void*, void** ret, const char* const* name)
{
    check(g == gameObj, "lookup this", 0); check(name && *name && strcmp(*name, "EM_FlashLightPoint") == 0, "lookup name", 0);
    ret[0] = pointVtbl; ret[1] = (void*)0x5150; ret[2] = &pointInfo; lastPoint = ret; ++liveThings; Ev('P'); return ret;
}
extern "C" bool __fastcall G_IsHeroHandLampLit(void* g, void*) { check(g == gameObj, "lamp this", 0); bool r = Next(sLit); Ev(r ? 'L' : 'l'); return r; }

// pDSTGame timers: indices 0..63, logged as letters
static int nextTimer, liveTimers; static bool timerLive[64]; static long timerSet[64];
extern "C" long __fastcall D_RegisterTimer(void* g, void*)
{
    check(g == dstObj, "register this", 0); int i = nextTimer++ & 63; timerLive[i] = true; timerSet[i] = -999; ++liveTimers;
    Ev('g'); Ev((char)('A' + i)); return 100 + i;
}
extern "C" void __fastcall D_DeregisterTimer(void* g, void*, long idx)
{
    check(g == dstObj, "deregister this", 0); int i = (int)idx - 100; check(i >= 0 && i < 64 && timerLive[i], "deregister a live timer", 0);
    if (i >= 0 && i < 64) timerLive[i] = false; --liveTimers; Ev('d'); Ev((char)('A' + i));
}
extern "C" void __fastcall D_SetTimer(void* g, void*, long idx, long n)
{
    check(g == dstObj, "set this", 0); int i = (int)idx - 100; check(i >= 0 && i < 64 && timerLive[i], "set a live timer", 0);
    if (i >= 0 && i < 64) timerSet[i] = n; Ev('s'); Ev((char)('A' + i)); Ev((char)('0' + n));
}
extern "C" long __fastcall D_GetTimer(void* g, void*, long idx)
{
    check(g == dstObj, "get this", 0); int i = (int)idx - 100; check(i >= 0 && i < 64 && timerLive[i] && timerSet[i] == 3, "get a set live timer", 0);
    bool run = Next(sTime); Ev(run ? 'R' : 'r'); Ev((char)('A' + i)); return run ? timeValue : stopValue;
}

// ---- reference model ------------------------------------------------------------------------------------------------------------
static Script mTerm, mLit, mTime, mDist; static int mTimer; static bool mCorrect, mReset;
static bool T() { bool r = Next(mTerm); Want(r ? 'T' : 't'); return r; }
static bool Lit() { bool r = Next(mLit); Want(r ? 'L' : 'l'); return r; }
static void Reg(int i) { Want('g'); Want((char)('A' + i)); }
static void Dereg(int i) { Want('d'); Want((char)('A' + i)); }
static bool Running(int i) { bool r = Next(mTime); Want(r ? 'R' : 'r'); Want((char)('A' + i)); return r; }
static void Model(long timesLeft)
{
    if (timesLeft == 0) {
        if (T()) return;
        Want('n'); Want('P'); Want('~');                    // CScriptThing TargetPosition = Game->GetThingWithScriptName("EM_FlashLightPoint");
        Want('h'); bool d = Next(mDist); Want(d ? 'D' : 'd');
        if (d) { if (!T()) mCorrect = true; }
        else   { if (!T()) mReset = true; }
        Want('x');
        return;
    }
    int timer = mTimer++ & 63; Reg(timer);
    Want('s'); Want((char)('A' + timer)); Want('3');
    if (Lit()) {
        if (T()) { Dereg(timer); return; }
        while (Running(timer)) {
            Want('F'); if (T()) { Dereg(timer); return; }
            if (!Lit()) { if (T()) { Dereg(timer); return; } Model(timesLeft - 1); }
        }
        if (T()) { Dereg(timer); return; }
    } else {
        if (T()) { Dereg(timer); return; }
        while (Running(timer)) {
            Want('F'); if (T()) { Dereg(timer); return; }
            if (Lit()) { if (T()) { Dereg(timer); return; } Model(timesLeft - 1); }
        }
        if (T()) { Dereg(timer); return; }
    }
    mReset = true;
    Dereg(timer);
}

static void Run(const char* term, const char* lit, const char* tim, const char* dist, long times, long tv, bool c0, bool r0, int k)
{
    memset(self.b, 0xEE, sizeof self.b);
    *(void**)(self.b + 0x40) = gameObj; self.b[0x50] = c0; self.b[0x52] = r0; *(float*)(self.b + 0x9C) = maxDist;
    timeValue = tv > 0 ? tv : -tv; stopValue = tv > 0 ? 0 : tv;
    sTerm.s = term; sLit.s = lit; sTime.s = tim; sDist.s = dist; sTerm.pos = sLit.pos = sTime.pos = sDist.pos = 0;
    sTerm.dflt = true; sLit.dflt = false; sTime.dflt = false; sDist.dflt = false;
    mTerm = sTerm; mLit = sLit; mTime = sTime; mDist = sDist;
    ngot = nwant = 0; nextTimer = mTimer = 0; liveTimers = liveStrings = liveThings = 0; lastPoint = 0;
    for (int i = 0; i < 64; ++i) timerLive[i] = false;
    mCorrect = c0; mReset = r0;
    Model(times);
    CallRow(self.b, 0, times);
    got[ngot] = want[nwant] = 0;
    if (ngot != nwant || memcmp(got, want, ngot) != 0) {
        check(false, "event sequence", k);
        if (failures < 6) printf("  got  %.300s\n  want %.300s\n", got, want);
    }
    check(self.b[0x50] == (unsigned char)mCorrect, "HeroFlashedLightsCorrectly", k);
    check(self.b[0x52] == (unsigned char)mReset, "ResetLamp", k);
    check(*(void**)(self.b + 0x40) == gameObj && self.b[0x51] == 0xEE && self.b[0x53] == 0xEE, "no other store", k);
    check(liveTimers == 0 && liveStrings == 0 && liveThings == 0, "everything released", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; dstObj[0] = dstVtbl; pointVtbl[0] = (void*)VirtualTrap;
    for (int i = 0; i < 300; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 100; ++i) dstVtbl[i] = (void*)VirtualTrap;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThingWithScriptName;
    gameVtbl[185] = (void*)G_IsHeroHandLampLit;
    dstVtbl[87] = (void*)D_RegisterTimer; dstVtbl[88] = (void*)D_DeregisterTimer; dstVtbl[89] = (void*)D_SetTimer; dstVtbl[90] = (void*)D_GetTimer;
    int k = 0;
    for (int c = 0; c < 2; ++c) {
        Run("1", "", "", "", 0, 2, c, 0, ++k);                                // zero flashes, terminating
        Run("00", "", "", "1", 0, 2, c, 0, ++k);                              // in range: lights correct
        Run("01", "", "", "1", 0, 2, 0, c, ++k);                              // in range, terminating
        Run("00", "", "", "0", 0, 2, c, 0, ++k);                              // out of range: reset
        Run("01", "", "", "0", 0, 2, c, c, ++k);
        Run("1", "1", "", "", 1, 2, 0, c, ++k);                               // lit, terminating before the wait
        Run("1", "0", "", "", 1, 2, 0, c, ++k);                               // unlit, terminating before the wait
        Run("00", "1", "0", "", 1, 2, 0, c, ++k);                             // lit, timer already out: reset
        Run("00", "0", "0", "", 3, -5, 0, c, ++k);                            // unlit, negative time: reset
        Run("01", "1", "0", "", 1, 2, c, 0, ++k);                             // terminating after the wait
        Run("001", "1", "1", "", 1, 2, 0, 0, ++k);                            // terminating after a frame
        Run("0000", "11", "10", "", 1, 2, 0, 0, ++k);                         // still lit, then timeout
        Run("00000000", "10", "1", "1", 1, 2, 0, 0, ++k);                     // flash: lit -> unlit, recursion to zero, in range
        Run("00000000", "01", "1", "0", 1, 2, 0, 0, ++k);                     // unlit -> lit, recursion to zero, out of range
        Run("00010000", "10", "1", "1", 1, 2, 0, 0, ++k);
        Run("00100000", "01", "1", "1", 1, 2, 0, 0, ++k);
        Run("0000000000000000000000", "10101", "1101", "1", 2, 2, 0, 0, ++k); // depth 2 (into the out-of-line call)
        Run("0000000000000000000000000000000000", "0101010", "11011010", "10", 3, 9, 0, 0, ++k);
        Run("000000000000000000000000000000000000000000", "1010101010", "110110110", "1", 4, 4, 0, 0, ++k);
    }
    unsigned long seed = 59;
    for (int r = 0; r < 20000; ++r) {
        static char ts[200], ls[120], ms[120], ds[8];
        for (int i = 0; i < 199; ++i) { seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 29 == 0) ? '1' : '0'; }
        for (int i = 0; i < 119; ++i) { seed = seed * 1103515245ul + 12345ul; ls[i] = ((seed >> 16) % 2 == 0) ? '1' : '0'; }
        for (int i = 0; i < 119; ++i) { seed = seed * 1103515245ul + 12345ul; ms[i] = ((seed >> 16) % 4 != 0) ? '1' : '0'; }
        for (int i = 0; i < 7; ++i) { seed = seed * 1103515245ul + 12345ul; ds[i] = ((seed >> 16) % 2 == 0) ? '1' : '0'; }
        ts[199] = ls[119] = ms[119] = ds[7] = 0;
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 8);
        long times = v % 6; long tv = 1 + (long)((v >> 3) % 9);
        Run(ts, ls, ms, ds, times, ((v >> 7) & 3) == 0 ? -tv : tv, ((v >> 9) & 1) != 0, ((v >> 10) & 1) != 0, 1000 + r);
    }
    if (failures) { printf("00e76e90_TEST FAIL %d\n", failures); return 1; }
    printf("00e76e90_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}