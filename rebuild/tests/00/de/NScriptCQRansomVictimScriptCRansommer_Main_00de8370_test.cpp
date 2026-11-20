// Behaviour test for 0x00DE8370 NScript::CQ_RansomVictimScript::CRansommer::Main (links the REAL row object, built with its row flags
// /d2inlS13 /d2inlT29: the row body, the TU's CIsThingAlive ctor / dtor and vtable, the implicit movie special members (the ctor COMDAT
// it calls, the dtor COMDAT and its counted-pointer release), the expanded CWideScreenMagicPauseEntities, and the virtual calls through
// the parent's seh_Ransommer handle and Me all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the entity (0x1c bytes: Game +4, Me +8, ParentClass +0x14,
// PMasterData +0x18); the parent (RescuedVictim +0x4b, RansomHelpersAttacked +0x50, seh_Ransommer +0xac: a handle whose vtable has
// GetScriptThing 12, Speak 13, IsPerformingScriptTask 26); Me with a counted pointer-info record and a fake vtable (MsgIsHitBy 21,
// MsgIsTalkedToBy 27, MsgIsHitBySpecialAbilityFrom 41, MsgIsHitByAnySpecialAbilityFrom 42); Game's vtable slots 7, 8, 39, 70, 114,
// 126, 127, 264, 350, 352, 360, 361, 364, 365, 366, 370, 379, 520, 601. A scripted world gives the talk / hit / ability / heal messages
// and the speech task per frame; termination, StartScriptingEntity, the question answers (with unanswered frames), the hero's gold,
// health and the parent's flags are scripted.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the parent's
// RescuedVictim, and the balance of strings, things, CBaseObject parts, movies and reference counts.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); fflush(stdout); exit(3); }
#pragma comment(linker, "/alternatename:?Validate@CBaseObject@@UBEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CBaseObject@@MAE@XZ=@StubBaseDtor@8")
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:__purecall=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@StubThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@StubThingDtor@8")
#pragma comment(linker, "/alternatename:??0CBaseObject@@IAE@XZ=@StubBaseCtor@8")
#pragma comment(linker, "/alternatename:??_GCScriptGameResourceObjectMovieBase@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?PlayCutscene@CQ_RansomVictimScript@NScript@@QAEXVCCharString@@0@Z=@StubCutscene@16")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?IsValid@CScriptGameResourceObjectScriptedThingBase@@QBE_NXZ=@StubIsValid@8")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CRansommer@CQ_RansomVictimScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

// ---- strings (one pointer, like retail's CCharString) ---------------------------------------------------------------------------
static int liveStrings;
class CCharString { public: CCharString(const char* text, long length); ~CCharString(); char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
static void Bad(const char* what) { ++failures; if (failures < 40) printf("FAIL %s\n", what); }

// ---- event logs ------------------------------------------------------------------------------------------------------------------
enum { MAXLOG = 60000 };
static char got[MAXLOG + 64]; static int ngot;
static char want[MAXLOG + 64]; static int nwant;
static void Put(char* buf, int& n, const char* s) { size_t m = strlen(s); if (n + (int)m + 1 < MAXLOG) { memcpy(buf + n, s, m); n += (int)m; buf[n++] = ' '; buf[n] = 0; } }
static void Ev(const char* s) { Put(got, ngot, s); }
static void Wv(const char* s) { Put(want, nwant, s); }
static const char *termScript, *startScript;      // '1' = true; past the end: true
static const char* answerScript;                  // per MsgIsQuestionAnsweredYesOrNo call: '-' unanswered (-1), '1' yes, '0' no; past the end: '0'
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }
static long NextAnswer(const char* s, int& pos) { char c = s[pos]; if (!c) return 0; ++pos; return c == '-' ? -1 : c == '1' ? 1 : 0; }

// ---- world -----------------------------------------------------------------------------------------------------------------------
enum { FRAMES = 64 };
struct World { unsigned char talk[FRAMES], hit[FRAMES], ability[FRAMES], heal[FRAMES]; int speakFrames; long gold; bool rescued0, attacked; };
static World world;
struct State { int frame, tpos, spos, apos, busyUntil; bool valid; long gold; };
static State gs, ms;
static int F(int f) { return f < FRAMES ? f : FRAMES - 1; }

static union { unsigned char b[0x1c]; double align; } ent;
static union { unsigned char b[0x100]; double align; } parent;

struct Info { unsigned long RefCount; void* DeleteFunc; void* Data; };
struct Impl { const char* name; };
struct Thing { void** vtbl; Impl* data; Info* info; };
static void* thingVtbl[80];
static Impl meImpl, heroImpl;
static Info meInfo;
static Thing heroThing;
static Thing* Me() { return (Thing*)(ent.b + 8); }
static int liveThings, baseLive, movieLive;
static const char* Who(const void* t)
{ const Thing* th = (const Thing*)t; return t == Me() ? "Me" : t == &heroThing ? "Hero" : (th->data == &meImpl ? "MeCopy" : th->data ? th->data->name : "null"); }

// the parent's handle: { vptr, IntelligentPointer, PImp.Data, PImp.Info }
struct Handle { void* vtbl; void* ip; void* data; Info* info; };
static Info hInfo; static int stbImpl; static void* sehVtbl[40];
static Handle* Seh() { return (Handle*)(parent.b + 0xac); }
enum { REF0 = 1000 };

extern "C" {
void __fastcall StubBaseCtor(void* self, void*) { ((void**)self)[1] = 0; ++baseLive; }
void __fastcall StubBaseDtor(void*, void*) { --baseLive; }
void* __fastcall StubThingCopy(void* self, void*, const void* o)
{ Thing* t = (Thing*)self; const Thing* s = (const Thing*)o; t->vtbl = s->vtbl; t->data = s->data; t->info = s->info; if (t->info) ++t->info->RefCount; ++liveThings; return self; }
void __fastcall StubThingDtor(void* self, void*) { Thing* t = (Thing*)self; if (t->info) --t->info->RefCount; --liveThings; memset(self, 0xDD, 12); }
void __fastcall StubCutscene(void* self, void*, CCharString a, CCharString b)
{   // by-value strings: the callee destroys them (the destructors run at this function's end)
    if (self != parent.b) Bad("PlayCutscene receiver"); char s[160]; sprintf(s, "CUT:%s/%s", a.Text, b.Text); Ev(s);
}
bool __fastcall StubIsValid(const void* self, void*) { if (self != Seh()) Bad("IsValid receiver"); return ((const Handle*)self)->data != 0; }
void __fastcall StubRelease(void* self, void*)
{ Handle* h = (Handle*)self; if (self != Seh()) Bad("Release receiver"); if (h->info) --h->info->RefCount; h->data = 0; h->info = 0; Ev("R"); }
void __fastcall StubSetPredicate(void* self, void*, const void* pred)
{
    if (self != ent.b) Bad("SetPredicate receiver");
    const Thing* t = (const Thing*)((const char*)pred + 4);
    if (!*(void* const*)pred || t->data != &meImpl || t->info != &meInfo || meInfo.RefCount != REF0 + 1) Bad("predicate: a counted copy of Me");
    Ev("P");
}
bool __fastcall StubTerminating(void* self, void*)
{ if (self != ent.b) Bad("terminate receiver"); bool t = NextBit(termScript, gs.tpos, true); Ev(t ? "T" : "t"); return t; }
}

// the handle's virtuals
static void CheckSeh(const void* self) { if (self != Seh() || Seh()->data != &stbImpl) Bad("seh_Ransommer receiver"); }
static void __fastcall S_Speak(void* self, void*, const void* to, const char* text, int sel, bool a, bool b, bool c)
{ CheckSeh(self); char s[96]; sprintf(s, "SPEAK:%s/%s/%d/%d%d%d", Who(to), text, sel, a, b, c); Ev(s); gs.busyUntil = gs.frame + world.speakFrames; }
static bool __fastcall S_Busy(const void* self, void*) { CheckSeh(self); bool r = gs.frame < gs.busyUntil; Ev(r ? "BUSY1" : "BUSY0"); return r; }
static void* __fastcall S_GetScriptThing(const void* self, void*, Thing* ret)
{ CheckSeh(self); ret->vtbl = thingVtbl; ret->data = &meImpl; ret->info = 0; ++liveThings; Ev("SCRIPTTHING"); return ret; }

// Me's messages
static void CheckMeThing(const void* self) { if (self != Me()) Bad("message receiver"); }
static bool __fastcall T_HitBy(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.hit[F(gs.frame)] != 0; char s[64]; sprintf(s, "HIT:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall T_TalkedTo(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.talk[F(gs.frame)] != 0; char s[64]; sprintf(s, "TALK:%s=%d", w->Text, r); Ev(s); return r; }
static bool __fastcall T_AbilityFrom(const void* self, void*, int ab, const CCharString* w)
{ CheckMeThing(self); bool r = world.heal[F(gs.frame)] != 0; char s[64]; sprintf(s, "ABIL%d:%s=%d", ab, w->Text, r); Ev(s); return r; }
static bool __fastcall T_AnyAbilityFrom(const void* self, void*, const CCharString* w)
{ CheckMeThing(self); bool r = world.ability[F(gs.frame)] != 0; char s[64]; sprintf(s, "ANYABIL:%s=%d", w->Text, r); Ev(s); return r; }

// ---- game interface ----------------------------------------------------------------------------------------------------------------
static void* gameVtbl[900]; static void* gameObj[1]; static bool badGame;
static void CheckGame(const void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev("N"); ++gs.frame; }
static bool __fastcall G_StartScripting(void* self, void*, const void* who, void* handle, int prio)
{
    CheckGame(self); char s[32];
    if (who != Me() || handle != Seh()) Bad("StartScriptingEntity arguments");
    bool ok = NextBit(startScript, gs.spos, true);
    Handle* h = (Handle*)handle;
    if (ok) { ++hInfo.RefCount; if (h->info) --h->info->RefCount; h->data = &stbImpl; h->info = &hInfo; }
    sprintf(s, "%c%d", ok ? 'S' : 's', prio); Ev(s); return ok;
}
static long __fastcall G_Answer(const void* self, void*) { CheckGame(self); long a = NextAnswer(answerScript, gs.apos); char s[16]; sprintf(s, "ANS=%ld", a); Ev(s); return a; }
static Thing* __fastcall G_GetHero(void* self, void*) { CheckGame(self); return &heroThing; }
static void __fastcall G_Question(const void* self, void*, const CCharString* q, const CCharString* y, const CCharString* n, const CCharString* x, bool b)
{ CheckGame(self); char s[200]; sprintf(s, "QUESTION:%s/%s/%s/[%s]/%d", q->Text, y->Text, n->Text, x->Text, b); Ev(s); }
static void __fastcall G_GiveGold(const void* self, void*, long g) { CheckGame(self); gs.gold += g; char s[32]; sprintf(s, "GIVEGOLD:%ld", g); Ev(s); }
static long __fastcall G_GetGold(const void* self, void*) { CheckGame(self); char s[32]; sprintf(s, "GOLD=%ld", gs.gold); Ev(s); return gs.gold; }
static float healthNext; static const float* healthScript; static int healthPos, mHealthPos;
static float __fastcall G_GetHealth(void* self, void*, const void* thing)
{ CheckGame(self); healthNext = healthScript[healthPos++]; char s[48]; sprintf(s, "HEALTH:%s=%g", Who(thing), healthNext); Ev(s); return healthNext; }
static void __fastcall G_MiniMap(void* self, void*, const void* t, const CCharString* m) { CheckGame(self); char s[64]; sprintf(s, "MINIMAP:%s/%s", Who(t), m->Text); Ev(s); }
static void __fastcall G_MiniMapRemove(void* self, void*, const void* t) { CheckGame(self); char s[48]; sprintf(s, "MMREMOVE:%s", Who(t)); Ev(s); }
static void __fastcall G_HasInfo(void* self, void*, const void* t, bool a, bool b, bool c) { CheckGame(self); char s[48]; sprintf(s, "INFO:%s/%d%d%d", Who(t), a, b, c); Ev(s); }
static void __fastcall G_ClearInfo(void* self, void*, const void* t) { CheckGame(self); char s[48]; sprintf(s, "CLEARINFO:%s", Who(t)); Ev(s); }
static long convIndex;
static long __fastcall G_NewConv(void* self, void*, const void* t, bool a, bool b) { CheckGame(self); char s[48]; sprintf(s, "CONV:%s/%d%d", Who(t), a, b); Ev(s); return ++convIndex; }
static void __fastcall G_AddPerson(void* self, void*, long i, const void* t) { CheckGame(self); char s[48]; sprintf(s, "PERSON:%ld/%s", i, Who(t)); Ev(s); }
static void __fastcall G_AddLine(void* self, void*, long i, const CCharString* l, bool b, const void* a, const void* c)
{ CheckGame(self); char s[96]; sprintf(s, "LINE:%ld/%s/%d/%s/%s", i, l->Text, b, Who(a), Who(c)); Ev(s); }
static int movieImpl;
static void __fastcall MovieDelete(void* data) { if (data != &movieImpl) Bad("movie delete data"); --movieLive; Ev("~MOVIE"); }
static void __fastcall G_StartMovie(void* self, void*, const CCharString* name, void* movie)
{
    CheckGame(self); char s[48]; sprintf(s, "MOVIE:[%s]", name->Text); if (((void**)movie)[2] || ((void**)movie)[3]) Bad("movie handle not empty"); Ev(s);
    Info* info = new Info; info->RefCount = 1; info->DeleteFunc = (void*)MovieDelete; info->Data = &movieImpl;
    ((void**)movie)[2] = &movieImpl; ((void**)movie)[3] = info; ++movieLive;
}
static void __fastcall G_Pause(void* self, void*, bool b) { CheckGame(self); Ev(b ? "PAUSE1" : "PAUSE0"); }
static void __fastcall G_EntityGold(void* self, void*, const void* t, long g) { CheckGame(self); char s[48]; sprintf(s, "ENTITYGOLD:%s/%ld", Who(t), g); Ev(s); }
static void __fastcall G_Faction(void* self, void*, const void* t, const CCharString* f) { CheckGame(self); char s[64]; sprintf(s, "FACTION:%s/%s", Who(t), f->Text); Ev(s); }

// ---- reference model -----------------------------------------------------------------------------------------------------------
static bool mInMovie;
static bool MT() { bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); if (t && mInMovie) { Wv("PAUSE0"); Wv("~MOVIE"); } return t; }
static void MF() { Wv("N"); ++ms.frame; }
static bool MStart(int prio) { bool ok = NextBit(startScript, ms.spos, true); if (ok) ms.valid = true; char s[8]; sprintf(s, "%c%d", ok ? 'S' : 's', prio); Wv(s); return ok; }
static void MRelease() { if (ms.valid) { Wv("R"); ms.valid = false; } }
static bool mRescued;
// a health-gated line and its wait: false = the thread ended
static bool MSpeak(const char* text, int sel)
{
    char s[96]; Wv("SCRIPTTHING");
    float h = healthScript[mHealthPos++]; sprintf(s, "HEALTH:MeCopy=%g", h); Wv(s);
    if (h > 0.0f) {
        sprintf(s, "SPEAK:Hero/%s/%d/010", text, sel); Wv(s); int busyUntil = ms.frame + world.speakFrames;
        for (;;) { bool b = ms.frame < busyUntil; Wv(b ? "BUSY1" : "BUSY0"); if (!b) break; MF(); if (MT()) return false; }
        if (MT()) return false;
    }
    return true;
}
// the question and its wait: -2 = the thread ended, else the answer
static long MAsk(const char* q)
{
    char s[200]; sprintf(s, "QUESTION:%s/TEXT_OBJECT_HERO_ANSWER_YES/TEXT_OBJECT_HERO_ANSWER_NO/[]/1", q); Wv(s);
    long a;
    for (;;) { a = NextAnswer(answerScript, ms.apos); sprintf(s, "ANS=%ld", a); Wv(s); if (a >= 0) break; MF(); if (MT()) return -2; }
    if (MT()) return -2;
    return a;
}
static long MGold() { char s[32]; sprintf(s, "GOLD=%ld", ms.gold); Wv(s); return ms.gold; }
static void MPay(long price)
{ char s[48]; Wv("MMREMOVE:Me"); Wv("CLEARINFO:Me"); sprintf(s, "GIVEGOLD:%ld", -price); Wv(s); ms.gold -= price; sprintf(s, "ENTITYGOLD:Me/%ld", price); Wv(s); }
static void Model()
{
    char s[96];
    mRescued = world.rescued0;
    Wv("P"); MF(); if (MT()) return;
    MRelease();
    while (!MStart(4)) { MF(); if (MT()) return; }
    if (MT()) return;
    if (!mRescued) { if (MT()) return; Wv("FACTION:Me/FACTION_HERO"); Wv("MINIMAP:Me/HUD_ORB_QUEST_OPTIONAL"); Wv("INFO:Me/000"); }
    if (!mRescued) {
        if (MT()) return;
        Wv("MOVIE:[]"); Wv("PAUSE1"); mInMovie = true;
        const bool att = world.attacked;
        if (MT()) return;
        sprintf(s, "CUT:CS_RANSOM_CAVE_INTERIOR_%s/NULL", att ? "1B" : "1A"); Wv(s);
        long a = MAsk(att ? "TEXT_QST_B06_QUESTION_RANSOM_HIGHER" : "TEXT_QST_B06_QUESTION_RANSOM_NORMAL"); if (a == -2) return;
        long price = att ? 10000 : 7500;
        if (a == 1) {
            if (MT()) return;
            if (MGold() >= price) {
                if (MT()) return;
                sprintf(s, "CUT:CS_RANSOM_CAVE_INTERIOR_2/TEXT_CS_B06_RANSOM_PAID_%s_10", att ? "ATTACK" : "NORMAL"); Wv(s);
                mRescued = true; MPay(price);
            } else {
                if (MT()) return;
                if (!MSpeak("TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", 2)) return;
            }
        } else {
            if (MT()) return;
            sprintf(s, "CUT:CS_RANSOM_CAVE_INTERIOR_2/TEXT_CS_B06_RANSOM_REFUSED_%s_10", att ? "ATTACK" : "NORMAL"); Wv(s);
        }
        Wv("PAUSE0"); Wv("~MOVIE"); mInMovie = false;
    }
    for (;;) {
        bool t = NextBit(termScript, ms.tpos, true); Wv(t ? "T" : "t"); if (t) return;
        bool talk = world.talk[F(ms.frame)] != 0; sprintf(s, "TALK:SCRIPT_NAME_HERO=%d", talk); Wv(s);
        if (talk) {
            if (MT()) return;
            Wv("MOVIE:[]"); Wv("PAUSE1"); mInMovie = true;
            if (!mRescued) {
                if (MT()) return;
                if (!MSpeak("TEXT_QST_B06_RANSOM_ASK", 2)) return;
                const bool att = world.attacked; long price = att ? 10000 : 7500;
                if (MT()) return;
                long a = MAsk(att ? "TEXT_QST_B06_QUESTION_RANSOM_HIGHER" : "TEXT_QST_B06_QUESTION_RANSOM_NORMAL"); if (a == -2) return;
                if (a == 1) {
                    if (MT()) return;
                    if (MGold() >= price) {
                        if (MT()) return;
                        mRescued = true;
                        if (!MSpeak("TEXT_QST_B06_RANSOM_THANKS_10", 0)) return;
                        MPay(price);
                    } else {
                        if (MT()) return;
                        if (!MSpeak("TEXT_QST_B06_RANSOM_NOT_ENOUGH_GOLD", 2)) return;
                    }
                } else {
                    if (MT()) return;
                    if (!MSpeak("TEXT_QST_B06_RANSOM_ASK_REMIND", 2)) return;
                }
            } else {
                if (MT()) return;
                if (!MSpeak("TEXT_QST_B06_RANSOM_DONE", 2)) return;
            }
            Wv("PAUSE0"); Wv("~MOVIE"); mInMovie = false;
        }
        int f = F(ms.frame);
        bool hit = world.hit[f] != 0; sprintf(s, "HIT:SCRIPT_NAME_HERO=%d", hit); Wv(s);
        if (!hit) {
            bool any = world.ability[f] != 0; sprintf(s, "ANYABIL:SCRIPT_NAME_HERO=%d", any); Wv(s);
            if (any) { bool heal = world.heal[f] != 0; sprintf(s, "ABIL14:SCRIPT_NAME_HERO=%d", heal); Wv(s); hit = !heal; }
        }
        if (hit) {
            if (MT()) return;
            Wv("FACTION:Me/FACTION_BANDITS"); Wv("CONV:Me/00");
            long i = ++convIndex; sprintf(s, "PERSON:%ld/Hero", i); Wv(s); sprintf(s, "LINE:%ld/TEXT_QST_B06_RANSOM_ON_HIT_10/0/Me/Hero", i); Wv(s);
            MRelease();
            for (;;) { MF(); if (MT()) return; }
        }
        MF();
    }
}

static int cov[16];
static void Run(const World& w, const char* term, const char* start, const char* answers, const float* health, int k)
{
    world = w; termScript = term; startScript = start; answerScript = answers; healthScript = health; healthPos = mHealthPos = 0;
    memset(&gs, 0, sizeof gs); memset(&ms, 0, sizeof ms); gs.gold = ms.gold = w.gold; ngot = nwant = 0; got[0] = want[0] = 0; mInMovie = false;
    memset(parent.b, 0x5A, sizeof parent.b); parent.b[0x4b] = w.rescued0; parent.b[0x50] = w.attacked;
    Handle* h = Seh(); h->vtbl = sehVtbl; h->ip = 0; h->data = (k & 2) ? &stbImpl : 0; h->info = (k & 2) ? &hInfo : 0;
    hInfo.RefCount = REF0 + ((k & 2) ? 1 : 0); ms.valid = (k & 2) != 0;
    memset(ent.b, 0x11, sizeof ent.b); void* g = gameObj; memcpy(ent.b + 4, &g, 4); void* pp = parent.b; memcpy(ent.b + 0x14, &pp, 4);
    Thing* me = Me(); me->vtbl = thingVtbl; me->data = &meImpl; me->info = &meInfo;
    meInfo.RefCount = REF0;
    liveThings = baseLive = liveStrings = movieLive = 0; badGame = false;
    convIndex = 0; Model(); convIndex = 0;
    CallMain(ent.b);
    if (strcmp(got, want)) {
        ++failures;
        if (failures < 6) {
            int i = 0; while (got[i] && got[i] == want[i]) ++i;
            int a = i > 160 ? i - 160 : 0;
            printf("FAIL case %d events differ at %d\n  got  ...%.240s\n  want ...%.240s\n", k, i, got + a, want + a);
        }
        return;
    }
    check(!badGame, "Game receiver", k);
    check((parent.b[0x4b] != 0) == mRescued, "RescuedVictim", k);
    check(parent.b[0x50] == (unsigned char)w.attacked, "RansomHelpersAttacked untouched", k);
    check(liveThings == 0, "every CScriptThing (predicate copy, script-thing temporaries) destroyed", k);
    check(baseLive == 0 && movieLive == 0, "every CBaseObject part and movie destroyed", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(meInfo.RefCount == REF0, "Me's reference count balanced", k);
    check(hInfo.RefCount == REF0 + (Seh()->data ? 1u : 0u), "handle references balanced", k);
    const char* pats[] = { "PAID_NORMAL", "PAID_ATTACK", "REFUSED_NORMAL", "REFUSED_ATTACK", "NOT_ENOUGH_GOLD", "RANSOM_ASK/", "THANKS_10",
        "ASK_REMIND", "RANSOM_DONE", "FACTION_BANDITS", "ANS=-1", "BUSY1 N T PAUSE0", "ABIL14:SCRIPT_NAME_HERO=1", "s4" };
    for (int i = 0; i < 14; ++i) if (strstr(got, pats[i])) ++cov[i];
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    for (int i = 0; i < 80; ++i) thingVtbl[i] = (void*)VirtualTrap;
    thingVtbl[21] = (void*)T_HitBy; thingVtbl[27] = (void*)T_TalkedTo; thingVtbl[41] = (void*)T_AbilityFrom; thingVtbl[42] = (void*)T_AnyAbilityFrom;
    for (int i = 0; i < 40; ++i) sehVtbl[i] = (void*)VirtualTrap;
    sehVtbl[12] = (void*)S_GetScriptThing; sehVtbl[13] = (void*)S_Speak; sehVtbl[26] = (void*)S_Busy;
    meImpl.name = "me"; heroImpl.name = "hero";
    heroThing.vtbl = thingVtbl; heroThing.data = &heroImpl; heroThing.info = 0;
    for (int i = 0; i < 900; ++i) gameVtbl[i] = (void*)VirtualTrap;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[39] = (void*)G_Answer; gameVtbl[70] = (void*)G_GetHero;
    gameVtbl[114] = (void*)G_Question; gameVtbl[126] = (void*)G_GiveGold; gameVtbl[127] = (void*)G_GetGold; gameVtbl[264] = (void*)G_GetHealth;
    gameVtbl[350] = (void*)G_MiniMap; gameVtbl[352] = (void*)G_MiniMapRemove; gameVtbl[360] = (void*)G_HasInfo; gameVtbl[361] = (void*)G_ClearInfo;
    gameVtbl[364] = (void*)G_NewConv; gameVtbl[365] = (void*)G_AddPerson; gameVtbl[366] = (void*)G_AddLine; gameVtbl[370] = (void*)G_StartMovie;
    gameVtbl[379] = (void*)G_Pause; gameVtbl[520] = (void*)G_EntityGold; gameVtbl[601] = (void*)G_Faction;
    static float health[96]; int k = 0;
    static const char zeros[] = "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
    // targeted: talk at 3, 6, 9; hit at 12; answers scripted per variant; a terminate at each of the first 100 tests
    static const char* answers[] = { "-1", "0", "1", "-0-1", "01", "1-1", "001" };
    for (int variant = 0; variant < 14; ++variant) {
        World w; memset(&w, 0, sizeof w);
        w.talk[3] = 1; w.talk[6] = 1; w.talk[9] = 1; w.hit[12] = 1; w.ability[10] = 1; w.heal[10] = 1;
        w.speakFrames = variant == 3 ? 0 : 2; w.attacked = (variant & 1) != 0; w.rescued0 = variant == 12; w.gold = (variant / 2) % 3 == 0 ? 20000 : (variant / 2) % 3 == 1 ? 8000 : 100;
        for (int i = 0; i < 96; ++i) health[i] = (variant == 13 && i == 0) ? 0.0f : 10.0f;
        char ts[128];
        for (int t = 0; t < 100; ++t) { memcpy(ts, zeros, 110); ts[t] = '1'; ts[110] = 0; Run(w, ts, (t & 1) ? "1" : "0010", answers[variant % 7], health, ++k); }
    }
    unsigned long seed = 31;
    for (int r = 0; r < 6000; ++r) {
#define RND(n) (seed = seed * 1103515245ul + 12345ul, (int)((seed >> 16) % (n)))
        World w; memset(&w, 0, sizeof w);
        for (int f = 0; f < FRAMES; ++f) { w.talk[f] = RND(5) == 0; w.hit[f] = RND(14) == 0; w.ability[f] = RND(9) == 0; w.heal[f] = RND(2); }
        w.speakFrames = RND(4); w.attacked = RND(2) != 0; w.rescued0 = RND(8) == 0;
        static const long golds[] = { 0, 7499, 7500, 9999, 10000, 50000 }; w.gold = golds[RND(6)];
        for (int i = 0; i < 96; ++i) health[i] = RND(4) ? 5.0f : (RND(2) ? 0.0f : -1.0f);
        static char ts[400], ss[16], as[64];
        for (int i = 0; i < 399; ++i) ts[i] = RND(60) == 0 ? '1' : '0';
        ts[399] = 0;
        for (int i = 0; i < 15; ++i) ss[i] = RND(3) == 0 ? '0' : '1';
        ss[15] = 0;
        for (int i = 0; i < 63; ++i) { int c = RND(3); as[i] = c == 0 ? '-' : c == 1 ? '1' : '0'; }
        as[63] = 0;
        Run(w, ts, ss, as, health, 1000 + r);
    }
    for (int i = 0; i < 14; ++i) printf("cov%d %d\n", i, cov[i]);
    if (failures) { printf("00de8370_TEST FAIL %d\n", failures); return 1; }
    printf("00de8370_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}