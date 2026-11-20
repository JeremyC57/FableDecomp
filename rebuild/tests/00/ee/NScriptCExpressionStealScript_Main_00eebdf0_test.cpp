// Behaviour test for 0x00EEBDF0 NScript::CExpression_StealScript::Main (links the REAL row object: the row body, its expanded
// CIsThingConscious ctor / dtor, the expanded movie ctor and both CScriptThing::IsAlive forms -- expanded in the loop, the row TU's
// out-of-line COMDAT after it -- run as compiled). No class models: every external the row names is bound with /alternatename to an
// extern "C" __fastcall stub (thiscall-compatible, unused edx); every virtual goes through a hand-built vtable (Game slots 7, 70, 71,
// 159, 168, 169, 228, 281, 290, 342, 343, 346, 349, 370, 395, 629, 630, 631, 654, 655, 740; the hero's slot 21 MsgIsHitBy; the
// target implementation's slot 75 IsAlive). The script object is raw storage with Game at +0x40.
// Reference model: Model() below, written from the script's statements (float steps in double with the row's float stores); every
// run is compared event by event plus the bar values, the stolen / morality calls and the release of every string, thing and movie.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingConscious@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingConscious@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingConscious@NScript@@UBE_NXZ=@FilterCallOp@8")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@ThingDtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@BaseObjectCtor@8")
#pragma comment(linker, "/alternatename:??1CScriptGameResourceObjectMovieBase@@UAE@XZ=@MovieDtor@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@SetPredicate@12")
extern "C" void __fastcall CallMain(void* self);   // the row (virtual, thiscall: this in ecx)
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CExpression_StealScript@NScript@@UAEXXZ")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
void __cdecl operator delete(void*) { VirtualTrap(); }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static Script sTerm, sHeld, sHit, sSeen, sAlive;   // defaults past the end: terminating / held / not hit / not seen / alive
static bool stealable, inMovie, nullTarget;
static long duration, statLevel, statMax, fps, deedHandle; static const int* randVals; static int randPos;

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 8000 };
static char got[MAXEV + 1]; static int ngot;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static int curCase;
static void Bad(const char* what) { check(false, what, curCase); }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x60]; double a; } self;                // CExpression_StealScript (0x44)
static void* gameVtbl[800]; static void* gameObj[1];
static void* heroVtbl[80]; static void* hero[3];                       // CScriptThing {vptr, Data, Info}
static void* implVtbl[80]; static void* implObj[1]; static void* targetVtbl[1]; static int targetInfo, heroInfo;
static float bar[MAXEV]; static int nbar;

static int liveStrings;
static const char* questText = "Q_EXPRESSION_STEAL_INSTANCE";
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; if (len != -1) Bad("string length"); return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; *s = 0; }

static int liveThings;
extern "C" void* __fastcall ThingCopy(void** t, void*, void* const* o) { t[0] = o[0]; t[1] = o[1]; t[2] = o[2]; ++liveThings; Ev('C'); return t; }
extern "C" void __fastcall ThingDtor(void** t, void*) { --liveThings; Ev(t[2] == &targetInfo ? 'd' : 'D'); t[0] = t[1] = t[2] = 0; }
extern "C" bool __fastcall FilterCallOp(void*, void*) { VirtualTrap(); return false; }
static void* filterAddr;
extern "C" void __fastcall SetPredicate(void* s, void*, void** filter)
{
    if (s != self.b) Bad("SetPredicate receiver");
    void** vt = (void**)filter[0];
    if (!vt || vt[0] != (void*)FilterCallOp) Bad("filter vtable slot 0 = CIsThingConscious::operator()");
    if (filter[1] != heroVtbl || filter[2] != implObj || filter[3] != &heroInfo) Bad("filter Thing is a copy of the hero");
    filterAddr = filter; Ev('S');
}
extern "C" bool __fastcall IsTerm(void* s, void*) { if (s != self.b) Bad("terminate receiver"); bool t = Next(sTerm); Ev(t ? 'T' : 't'); return t; }
static void* movieAddr; static int liveMovies;
extern "C" void* __fastcall BaseObjectCtor(void* m, void*) { movieAddr = m; ++liveMovies; Ev('B'); return m; }
extern "C" void __fastcall MovieDtor(void** m, void*)
{
    if (m != movieAddr) Bad("movie dtor receiver");
    if (m[2] != 0 || m[3] != 0) Bad("movie PImp zeroed by the ctor");
    --liveMovies; Ev('b');
}
extern "C" int __cdecl rand(void) { Ev('?'); return randVals[randPos++]; }

// hero slot 21, target implementation slot 75
static bool __fastcall H_MsgIsHitBy(void* t, void*, const char* const* n) { if (t != hero) Bad("MsgIsHitBy on the hero"); if (strcmp(*n, "")) Bad("MsgIsHitBy name"); bool h = Next(sHit); Ev(h ? 'H' : 'h'); return h; }
static bool __fastcall I_IsAlive(void* t, void*) { if (t != implObj) Bad("IsAlive receiver"); bool a = Next(sAlive); Ev(a ? 'A' : 'a'); return a; }

// game interface
static void CheckGame(void* t) { if (t != gameObj) Bad("Game receiver"); }
static void** targetAddr;
static void __fastcall G_NewScriptFrame(void* t, void*) { CheckGame(t); Ev('N'); }
static void* __fastcall G_GetHero(void* t, void*) { CheckGame(t); return hero; }
static void* __fastcall G_GetHeroTargetedThing(void* t, void*, void** ret)
{ CheckGame(t); ret[0] = targetVtbl; ret[1] = nullTarget ? 0 : (void*)implObj; ret[2] = &targetInfo; ++liveThings; targetAddr = ret; Ev('G'); return ret; }
static void CheckTarget(const void* p) { if (p != targetAddr) Bad("the targeted thing"); }
static void __fastcall G_ChangeMorality(void* t, void*) { CheckGame(t); Ev('M'); }
static long __fastcall G_GetHeroStatLevel(void* t, void*, int s) { CheckGame(t); if (s != 5) Bad("stat = HERO_STAT_SKILL_STEALTH"); Ev('l'); return statLevel; }
static long __fastcall G_GetHeroStatMax(void* t, void*, int s) { CheckGame(t); if (s != 5) Bad("stat max = HERO_STAT_SKILL_STEALTH"); Ev('x'); return statMax; }
static bool __fastcall G_IsDPadHeld(void* t, void*, const char* const* n) { CheckGame(t); if (strcmp(*n, "EXPRESSION_STEAL")) Bad("expression name"); bool h = Next(sHeld); Ev(h ? 'E' : 'e'); return h; }
static void __fastcall G_DeactivateQuestLater(void* t, void*, const char* const* n, unsigned long d) { CheckGame(t); if (*n != questText || d != 0) Bad("DeactivateQuestLater(active quest, 0)"); Ev('Q'); }
static void __fastcall G_SetQuestAsPersistent(void* t, void*, const char* const* n, bool p) { CheckGame(t); if (*n != questText || p) Bad("SetQuestAsPersistent(active quest, false)"); Ev('P'); }
static void __fastcall G_DisplayMiniGameInfo(void* t, void*, bool on, int type) { CheckGame(t); if (type != 5) Bad("MINIGAME_STEAL"); Ev(on ? 'I' : 'i'); }
static void __fastcall G_UpdateBar(void* t, void*, float v) { CheckGame(t); if (nbar < MAXEV) bar[nbar++] = v; Ev('U'); }
static bool __fastcall G_IsEntityStealable(void* t, void*, const void* th) { CheckGame(t); CheckTarget(th); Ev('s'); return stealable; }
static void __fastcall G_EntitySetAsStolen(void* t, void*, const void* th) { CheckGame(t); CheckTarget(th); Ev('X'); }
static void __fastcall G_StartMovieSequence(void* t, void*, const char* const* n, void* m) { CheckGame(t); if (strcmp(*n, "") || m != movieAddr) Bad("StartMovieSequence(\"\", the movie)"); Ev('V'); }
static bool __fastcall G_IsInMovieSequence(void* t, void*) { CheckGame(t); Ev('m'); return inMovie; }
static long __fastcall G_PostDeed(void* t, void*, const void* who, int type, const void* th)
{ CheckGame(t); if (who != hero || type != 6) Bad("deed: hero, OPINION_DEED_TYPE_CRIME_THEFT"); CheckTarget(th); Ev('O'); return deedHandle; }
static void __fastcall G_RemoveDeed(void* t, void*, const void* who, long h) { CheckGame(t); if (who != hero || h != deedHandle) Bad("RemoveOpinionDeed(hero, handle)"); Ev('R'); }
static bool __fastcall G_IsDeedWitnessed(void* t, void*, long h) { CheckGame(t); if (h != deedHandle) Bad("IsDeedWitnessed(handle)"); bool w = Next(sSeen); Ev(w ? 'W' : 'w'); return w; }
static long __fastcall G_GetConstantFPS(void* t, void*) { CheckGame(t); Ev('F'); return fps; }
static void* __fastcall G_GetActiveQuestName(void* t, void*, const char** ret) { CheckGame(t); *ret = questText; ++liveStrings; return ret; }
static long __fastcall G_GetStealDuration(void* t, void*, const void* th) { CheckGame(t); CheckTarget(th); Ev('L'); return duration; }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static char want[MAXEV + 1]; static int nwant; static float wbar[MAXEV]; static int nwbar;
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static Script mTerm, mHeld, mHit, mSeen, mAlive; static int mRand;
static bool MT() { bool t = Next(mTerm); W(t ? 'T' : 't'); return t; }
static bool MAlive() { if (nullTarget) return false; bool a = Next(mAlive); W(a ? 'A' : 'a'); return a; }
static void Model()
{
    W('C'); W('S'); W('D'); W('N');
    if (MT()) return;
    W('P'); W('G'); W('s');
    if (!stealable) { if (MT()) { W('d'); return; } W('Q'); W('d'); return; }
    W('m');
    if (inMovie) { if (MT()) { W('d'); return; } W('Q'); W('d'); return; }
    W('B'); W('V'); W('I');
    bool finished = false, terminated = false;
    W('L');
    float td = (float)duration;
    W('l'); float lv1 = (float)((double)statLevel + 1.0);
    W('x'); double ratio = (double)lv1 / ((double)statMax + 1.0);
    td = (float)((double)td / ratio);
    W('F'); double prod = (double)fps * (double)td; td = (float)prod; long timer = (long)prod;
    W('O');
    float pct = 0.0f;
    while (!finished) {
        W('N'); if (MT()) { W('b'); W('d'); return; }
        bool held = Next(mHeld); W(held ? 'E' : 'e');
        if (!held) { if (MT()) { W('b'); W('d'); return; } finished = terminated = true; }
        bool hit = Next(mHit); W(hit ? 'H' : 'h');
        if (hit) { if (MT()) { W('b'); W('d'); return; } finished = terminated = true; }
        bool seen = Next(mSeen); W(seen ? 'W' : 'w');
        if (seen) { if (MT()) { W('b'); W('d'); return; } finished = terminated = true; }
        double p = 1.0 - (double)timer / (double)td; pct = (float)p;
        if (p > 1.0) { if (MT()) { W('b'); W('d'); return; } pct = 1.0f; }
        W('U'); if (nwbar < MAXEV) wbar[nwbar++] = pct;
        if (MAlive()) {
            if (MT()) { W('b'); W('d'); return; }
            if (pct >= 1.0f) { if (MT()) { W('b'); W('d'); return; } finished = true; }
        } else {
            if (MT()) { W('b'); W('d'); return; }
            terminated = finished = true;
        }
        if (timer > 0) { if (MT()) { W('b'); W('d'); return; } --timer; }
    }
    if (MT()) { W('b'); W('d'); return; }
    W('R'); W('i');
    if (!terminated) {
        if (MT()) { W('b'); W('d'); return; }
        if (MAlive()) {
            if (MT()) { W('b'); W('d'); return; }
            W('?'); long r = randVals[mRand++] % 100;
            if ((double)r < (double)pct * 100.0) {
                if (MT()) { W('b'); W('d'); return; }
                W('X'); W('M');
            }
        }
    }
    W('b'); W('Q'); W('d');
}

static void Run(const char* term, const char* held, const char* hit, const char* seen, const char* alive, bool st, bool mv, bool nt,
                long dur, long lvl, long mx, long f, const int* rv, int k)
{
    curCase = k;
    sTerm.s = mTerm.s = term; sHeld.s = mHeld.s = held; sHit.s = mHit.s = hit; sSeen.s = mSeen.s = seen; sAlive.s = mAlive.s = alive;
    sTerm.pos = sHeld.pos = sHit.pos = sSeen.pos = sAlive.pos = mTerm.pos = mHeld.pos = mHit.pos = mSeen.pos = mAlive.pos = 0;
    sTerm.dflt = mTerm.dflt = true; sHeld.dflt = mHeld.dflt = true; sHit.dflt = mHit.dflt = false; sSeen.dflt = mSeen.dflt = false;
    sAlive.dflt = mAlive.dflt = true;
    stealable = st; inMovie = mv; nullTarget = nt; duration = dur; statLevel = lvl; statMax = mx; fps = f; deedHandle = 0x4D00 + k;
    randVals = rv; randPos = mRand = 0;
    ngot = nwant = nbar = nwbar = 0; Model();
    liveStrings = liveThings = liveMovies = 0; movieAddr = 0; targetAddr = 0; filterAddr = 0;
    memset(self.b, 0x5A, sizeof self.b); void* g = gameObj; memcpy(self.b + 0x40, &g, 4);
    CallMain(self.b);
    got[ngot] = 0; want[nwant] = 0;
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); return; }
    check(nbar == nwbar, "bar update count", k);
    for (int i = 0; i < nbar && i < nwbar; ++i) if (memcmp(&bar[i], &wbar[i], 4)) { check(false, "bar value", k); printf("   bar %d: %.9g want %.9g\n", i, bar[i], wbar[i]); break; }
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(liveThings == 0, "every CScriptThing destroyed", k);
    check(liveMovies == 0, "movie destroyed", k);
    for (int i = 0; i < 0x60; ++i) if (i < 0x40 || i >= 0x44) check(self.b[i] == 0x5A, "script object untouched", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; implObj[0] = implVtbl;
    for (int i = 0; i < 800; ++i) gameVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) heroVtbl[i] = implVtbl[i] = (void*)VirtualTrap;
    targetVtbl[0] = (void*)VirtualTrap;
    hero[0] = heroVtbl; hero[1] = implObj; hero[2] = &heroInfo;
    heroVtbl[21] = (void*)H_MsgIsHitBy; implVtbl[75] = (void*)I_IsAlive;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[70] = (void*)G_GetHero; gameVtbl[71] = (void*)G_GetHeroTargetedThing;
    gameVtbl[159] = (void*)G_ChangeMorality; gameVtbl[168] = (void*)G_GetHeroStatLevel; gameVtbl[169] = (void*)G_GetHeroStatMax;
    gameVtbl[228] = (void*)G_IsDPadHeld; gameVtbl[281] = (void*)G_DeactivateQuestLater; gameVtbl[290] = (void*)G_SetQuestAsPersistent;
    gameVtbl[342] = (void*)G_DisplayMiniGameInfo; gameVtbl[343] = (void*)G_UpdateBar; gameVtbl[346] = (void*)G_IsEntityStealable;
    gameVtbl[349] = (void*)G_EntitySetAsStolen; gameVtbl[370] = (void*)G_StartMovieSequence; gameVtbl[395] = (void*)G_IsInMovieSequence;
    gameVtbl[629] = (void*)G_PostDeed; gameVtbl[630] = (void*)G_RemoveDeed; gameVtbl[631] = (void*)G_IsDeedWitnessed;
    gameVtbl[654] = (void*)G_GetConstantFPS; gameVtbl[655] = (void*)G_GetActiveQuestName; gameVtbl[740] = (void*)G_GetStealDuration;
    static const int r0[] = { 0, 0 }, r50[] = { 1250, 0 }, r99[] = { 99, 0 }, rneg[] = { -37, 0 };
    int k = 0;
    const char* Z = "00000000000000000000000000000000000000000000000000000000000000000000000000000000";
    for (int nt = 0; nt < 2; ++nt) {
        Run("1", "", "", "", "", true, false, nt, 3, 2, 10, 30, r0, ++k);                        // terminating at the first frame
        Run("0", "", "", "", "", false, false, nt, 3, 2, 10, 30, r0, ++k);                       // not stealable, terminating
        Run("00", "", "", "", "", false, false, nt, 3, 2, 10, 30, r0, ++k);                      // not stealable: deactivate
        Run("00", "", "", "", "", true, true, nt, 3, 2, 10, 30, r0, ++k);                        // in a movie: deactivate
        Run("0", "", "", "", "", true, true, nt, 3, 2, 10, 30, r0, ++k);
        Run(Z, "0", "", "", "1", true, false, nt, 3, 2, 10, 30, r0, ++k);                       // button released at once
        Run(Z, "1", "1", "", "1", true, false, nt, 3, 2, 10, 30, r0, ++k);                      // hit
        Run(Z, "11", "00", "01", "11", true, false, nt, 3, 2, 10, 30, r0, ++k);                 // witnessed on frame 2
        Run(Z, "", "", "", "0", true, false, nt, 3, 2, 10, 30, r0, ++k);                        // the thing dies
        Run(Z, "", "", "", "", true, false, nt, 1, 9, 9, 4, r0, ++k);                           // completes, roll 0 succeeds
        Run(Z, "", "", "", "", true, false, nt, 1, 9, 9, 4, r99, ++k);                          // roll 99 fails at 100%? (99 < 100)
        Run(Z, "", "", "", "", true, false, nt, 0, 0, 0, 30, r50, ++k);                         // zero duration
        Run(Z, "", "", "", "1110", true, false, nt, 2, 0, 3, 10, r0, ++k);                      // dies after three frames
        Run(Z, "", "", "", "", true, false, nt, -2, 1, 1, 10, rneg, ++k);                       // negative duration: percentage > 1
        Run(Z, "", "", "", "", true, false, nt, 5, 4, 10, 3, r50, ++k);
        Run(Z, "", "", "", "", true, false, nt, 5, 4, 10, 3, rneg, ++k);
    }
    for (int cut = 2; cut < 60; ++cut) {                                                         // a terminate at every check
        static char ts[64]; for (int i = 0; i < 63; ++i) ts[i] = (i == cut) ? '1' : '0'; ts[63] = 0;
        Run(ts, "", "", "", "", true, false, false, 2, 3, 5, 6, r0, ++k);
        Run(ts, "11110", "000001", "", "", true, false, false, 4, 1, 5, 6, r50, ++k);
        Run(ts, "", "", "", "11011", true, false, false, 4, 1, 5, 6, r50, ++k);
    }
    unsigned long seed = 31;
    static int rv[64];
    for (int r = 0; r < 20000; ++r) {
        static char ts[200], hs[200], is[200], ws[200], as[200];
        for (int i = 0; i < 199; ++i) {
            seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 61 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; hs[i] = ((seed >> 16) % 29 == 0) ? '0' : '1';
            seed = seed * 1103515245ul + 12345ul; is[i] = ((seed >> 16) % 41 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; ws[i] = ((seed >> 16) % 37 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; as[i] = ((seed >> 16) % 43 == 0) ? '0' : '1';
        }
        ts[199] = hs[199] = is[199] = ws[199] = as[199] = 0;
        for (int i = 0; i < 64; ++i) { seed = seed * 1103515245ul + 12345ul; rv[i] = (int)((seed >> 8) % 1000) - 200; }
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 4);
        long dur = (long)(v % 9) - 1, mx = (long)((v >> 4) % 11), lvl = (long)((v >> 8) % (mx + 1)), f = 1 + (long)((v >> 12) % 12);
        bool st = ((v >> 16) % 9) != 0, mv = ((v >> 19) % 9) == 0, nt = ((v >> 22) % 13) == 0;
        Run(ts, hs, is, ws, as, st, mv, nt, dur, lvl, mx, f, rv, 1000 + r);
    }
    if (failures) { printf("00eebdf0_TEST FAIL %d\n", failures); return 1; }
    printf("00eebdf0_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}