// Behaviour test for 0x00EFA4F0 NScript::Cbandit_level_cutsceneScript::Main (links the REAL row object, with its expanded and out-of-line
// movie-dtor paths). Fixtures: the script object (CScriptBase 0x40, Game +0x40, MasterScriptData +0x44) and a fake Game (slots 7
// NewScriptFrame, 12 IsRegionLoaded, 64 PostAddScriptedEntities, 370 StartMovieSequence, 376 Pause, 379 PauseAllNonScriptedEntities,
// 403 CameraSetCameraPreloadFlag, 412 CameraUseCameraPoint, 416 CameraDefault, 417 CameraResetToViewBehindHero, 418
// IsCameraInScriptedMode). IsActiveThreadTerminating / AddEntityScriptBinding / PostAddScriptedEntities, CCharString and CBaseObject are
// recording __fastcall stubs bound by /alternatename (CCharString keeps the literal pointer). One full pass is checked token by token;
// then the thread terminates at each of the 12 checks in turn: inside the movie scope (checks 4..9: the SPLINE_04 / 03 / 01 wait
// returns and post-wait checks, three expanded dtor sites and three COMDAT ones) the log must end with the pause and movie destruction.
#include <stdio.h>
#include <string.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }

#pragma comment(linker, "/alternatename:@CallRow@8=?Main@Cbandit_level_cutsceneScript@NScript@@UAEXXZ")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ=@StubTerm@8")
#pragma comment(linker, "/alternatename:?AddEntityScriptBinding@CScriptBase@@QAEXPAVCEntityScriptBindingBase@@@Z=@StubAdd@12")
#pragma comment(linker, "/alternatename:?PostAddScriptedEntities@CScriptBase@@QAEXXZ=@StubPost@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StubStr@16")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@ABV0@@Z=@StubStrCopy@12")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StubStrDtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@StubBOCtor@8")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBODtor@8")
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_StubNever")
#pragma comment(linker, "/alternatename:??1CScriptGameResourceObjectBase@@UAE@XZ=_StubNever")
#pragma comment(linker, "/alternatename:?Alloc@Cwelcome_guy@Cbandit_level_cutsceneScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=_StubAlloc")
#pragma comment(linker, "/alternatename:?CreateActiveEntityScript@?$CEntityScriptBinding@VCbandit_level_cutsceneScript@NScript@@@@MAE?AV?$CCountedPointer@VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z=_StubNever")

static int g_fail = 0, g_run = 0;
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d (run %d)\n", line, g_run); } }
#define CHECK(c) Check((c) ? true : false, __LINE__)

struct Str { const char* p; };
struct Obj { void** vt; };
struct Script { void* vt; unsigned char p04[0x40 - 4]; Obj* game; void* master; };
typedef char OffCheck[(size_t)&((Script*)0)->game == 0x40 ? 1 : -1];
struct Binding { void* vt; Str name; Script* script; long p0c; void* alloc; unsigned char b14; unsigned char p15[3]; long p18; };

static Obj g_game; static void* g_gameVt[420];
static Script g_s;
static char g_log[2048];
static int g_frames, g_tcalls, g_tTrueAt, g_scripted, g_preload, g_bindings;
static void* g_movie;
static void L(const char* t) { strcat(g_log, t); strcat(g_log, " "); }
static bool Loaded(int frame) { return frame >= 1 && frame <= 4; }

extern "C" void StubNever() { printf("unexpected call\n"); g_fail = 1; }
extern "C" void StubAlloc() { }
extern "C" bool __fastcall StubTerm(Script* self, void*) { CHECK(self == &g_s); L("T"); return ++g_tcalls == g_tTrueAt; }
extern "C" void __fastcall StubAdd(Script* self, void*, Binding* b)
{
    CHECK(self == &g_s); CHECK(b != 0 && strcmp(b->name.p, "welcome_guy") == 0 && b->script == &g_s && b->alloc == (void*)StubAlloc);
    CHECK(b->b14 == 1 && b->p18 == 0); ++g_bindings; L("B");
}
extern "C" void __fastcall StubPost(Script* self, void*) { CHECK(self == &g_s); L("PA"); }
extern "C" Str* __fastcall StubStr(Str* self, void*, const char* s, long n) { CHECK(n == -1); self->p = s; return self; }
extern "C" Str* __fastcall StubStrCopy(Str* self, void*, const Str* o) { self->p = o->p; return self; }
extern "C" void __fastcall StubStrDtor(Str* self, void*) { self->p = 0; }
extern "C" void* __fastcall StubBOCtor(void* self, void*) { g_movie = self; L("MC"); return self; }
extern "C" void __fastcall StubBODtor(void* self, void*) { CHECK(self == g_movie); L("MD"); }

extern "C" void __fastcall NewFrame(Obj* self, void*) { CHECK(self == &g_game); ++g_frames; if (g_scripted > 0) --g_scripted; L("F"); }
extern "C" bool __fastcall IsRegionLoaded(Obj* self, void*, const Str* n) { CHECK(self == &g_game && strcmp(n->p, "bandits") == 0); bool r = Loaded(g_frames); L(r ? "R1" : "R0"); return r; }
extern "C" void __fastcall GamePost(Obj* self, void*) { CHECK(self == &g_game); L("PG"); }
extern "C" void __fastcall StartMovie(Obj* self, void*, const Str* n, void* movie) { CHECK(self == &g_game && strcmp(n->p, "") == 0 && movie == g_movie); L("SM"); }
extern "C" void __fastcall Pause(Obj* self, void*, float t) { CHECK(self == &g_game && t == 3.0f); L("PZ"); }
extern "C" void __fastcall PauseAll(Obj* self, void*, bool on) { CHECK(self == &g_game); L(on ? "P1" : "P0"); }
extern "C" void __fastcall Preload(Obj* self, void*, bool on) { CHECK(self == &g_game); g_preload = on; L(on ? "PL1" : "PL0"); }
extern "C" void __fastcall CamPoint(Obj* self, void*, const Str* n, long a, long b, float c, long d, long e)
{
    CHECK(self == &g_game && a == 0 && b == 0 && c == -1.0f && d == 0 && e == -1); L(n->p); if (!g_preload) g_scripted = 1;
}
extern "C" void __fastcall CamDefault(Obj* self, void*) { CHECK(self == &g_game); L("CD"); }
extern "C" void __fastcall CamBehind(Obj* self, void*, float t) { CHECK(self == &g_game && t == 0.0f); L("RH"); }
extern "C" bool __fastcall CamScripted(Obj* self, void*) { CHECK(self == &g_game); L(g_scripted > 0 ? "W1" : "W0"); return g_scripted > 0; }
extern "C" void __fastcall CallRow(Script* self, void*);

static void Run(int tTrueAt)
{
    memset(&g_s, 0, sizeof g_s); g_s.game = &g_game; g_log[0] = 0;
    g_frames = 0; g_tcalls = 0; g_tTrueAt = tTrueAt; g_scripted = 0; g_preload = 0; g_bindings = 0; g_movie = 0;
    CallRow(&g_s, 0);
    CHECK(g_tcalls == tTrueAt);
}

static const char* kFull =
    "T R0 F T R1 T B PA PG MC SM P1 PL1 SPLINE_01 SPLINE_02 SPLINE_03 SPLINE_04 SPLINE_05 PL0 SPLINE_04 "
    "W1 F T W0 T SPLINE_03 W1 F T W0 T SPLINE_01 W1 F T W0 T RH CD PZ P0 MD "
    "R1 F T R0 T F T ";

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    g_gameVt[7] = (void*)NewFrame; g_gameVt[12] = (void*)IsRegionLoaded; g_gameVt[64] = (void*)GamePost; g_gameVt[370] = (void*)StartMovie;
    g_gameVt[376] = (void*)Pause; g_gameVt[379] = (void*)PauseAll; g_gameVt[403] = (void*)Preload; g_gameVt[412] = (void*)CamPoint;
    g_gameVt[416] = (void*)CamDefault; g_gameVt[417] = (void*)CamBehind; g_gameVt[418] = (void*)CamScripted; g_game.vt = g_gameVt;
    // 1: one whole pass (region loaded on frames 1..4: one wait frame before, one in the unload wait), terminating at the next loop top
    g_run = 1; Run(12);
    if (strcmp(g_log, kFull) != 0) { g_fail = 1; printf("run 1\n  got  %s\n  want %s\n", g_log, kFull); }
    CHECK(g_bindings == 1);
    // 2: terminating at check k: the log is the full log cut after its k-th "T ", plus "P0 MD " when k lies in the movie scope (4..9)
    for (int k = 1; k < 12; ++k) {
        g_run = 100 + k; Run(k);
        char want[2048]; const char* p = kFull; int seen = 0;
        while (seen < k) { p = strstr(p, "T ") ; p += 2; ++seen; }
        size_t n = (size_t)(p - kFull); memcpy(want, kFull, n); want[n] = 0;
        if (k >= 4 && k <= 9) strcat(want, "P0 MD ");
        if (strcmp(g_log, want) != 0) { g_fail = 1; printf("run %d\n  got  %s\n  want %s\n", g_run, g_log, want); }
    }
    if (!g_fail) printf("00efa4f0_TEST PASS\n");
    return g_fail;
}