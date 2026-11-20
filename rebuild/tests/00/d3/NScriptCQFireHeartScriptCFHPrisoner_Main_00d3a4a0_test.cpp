// Behaviour test for 0x00D3A4A0 NScript::CQ_FireHeartScript::CFH_Prisoner::Main (links the REAL row object: the row body, the
// expanded CIsThingAlive constructor / destructor, the visible map<CCharString,CCharString>::operator[] lookup, the expanded
// CSpawnedFunc constructor and its slot-4 SuspendableProcess, and the row TU's IsValid COMDAT all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the prisoner (0x24 bytes: Game +4, Me +8, ParentClass +0x14,
// Chosen +0x1c, MyCageNumber +0x20); the parent CQ_FireHeartScript (0x1A4 bytes: csargs +0x58 = a two-node STLport tree holding
// "$CAMERA" and "$LINE", FreedProphets +0x64, KilledProphets +0x68, StartNumberOfSuns +0x6c, PuzzleRound +0x88, ProphetsKilled +0xc8 /
// ProphetsReleased +0xd4 / ShouldProphetSpeak +0xe0 (byte vectors), Guest +0x140, ProphetsScripted +0x144, seh_Prophet +0x148 (16-byte
// handles, PImp.Data +8), MoralityChangeOnKilled +0x198, MoralityChangeOnFreed +0x19c, KilledAllPrisoners +0x1a2, FreedAllPrisoners
// +0x1a3); Game's vtable slots 7, 8, 108, 156, 255. A scripted world changes the round / released / killed / speak flags at every
// NewScriptFrame; termination, StartScriptingEntity results, the handle's validity and operator new are scripted too.
// Reference model: Model() below, written from the quest's statements; every run is compared event by event, then the parent's
// counters, flags and csargs strings, the spawned thread object, string balance and the predicate's thing copy are checked.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void* x) { unsigned long* rec = *(unsigned long**)x; printf("FAIL crash code %lx at %lx\n", rec[0], rec[3]); fflush(stdout); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); exit(3); }
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CSuspendableProcessMicrothread@@MAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?OnInterrupted@CSpawnedFuncBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?OnPredicateFail@CSpawnedFuncBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Restart@CSuspendableProcessMicrothread@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?RunSuspendableProcess@CSuspendableProcessMicrothread@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_G?$CSpawnedFunc@VCQ_FireHeartScript@NScript@@@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@V1@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@V1@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@V1@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@V1@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@V1@@2@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@StubThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@StubThingDtor@8")
#pragma comment(linker, "/alternatename:?Release@CScriptGameResourceObjectScriptedThingBase@@QAEXXZ=@StubRelease@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@StubSetPredicate@12")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@StubTerminating@8")
#pragma comment(linker, "/alternatename:??0CSpawnedFuncBase@@QAE@ABVCCharString@@K@Z=@StubSpawnedFuncBase@16")
#pragma comment(linker, "/alternatename:?AddSpawnedFunction@CScriptBase@@QAEXPAVCSpawnedFuncBase@@ABVCCharString@@@Z=@StubAddSpawned@16")
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CFH_Prisoner@CQ_FireHeartScript@NScript@@EAEXXZ")
extern "C" void __fastcall CallMain(void* self);

// ---- strings (one pointer, like retail's CCharString) ---------------------------------------------------------------------------
static int liveStrings;
class CCharString { public:
    CCharString(); CCharString(const char* text, long length); CCharString(const CCharString& o); ~CCharString();
    const CCharString& operator=(const CCharString& o); bool operator<(const CCharString& o) const;
    char* Text; };
static char* Dup(const char* s) { size_t n = strlen(s); char* p = (char*)malloc(n + 1); memcpy(p, s, n + 1); return p; }
CCharString::CCharString() { Text = Dup(""); ++liveStrings; }
CCharString::CCharString(const char* text, long length) { Text = Dup(text); ++liveStrings; if (length != -1) { printf("FAIL string length\n"); exit(4); } }
CCharString::CCharString(const CCharString& o) { Text = Dup(o.Text); ++liveStrings; }
CCharString::~CCharString() { free(Text); Text = 0; --liveStrings; }
const CCharString& CCharString::operator=(const CCharString& o) { char* p = Dup(o.Text); free(Text); Text = p; return *this; }
bool CCharString::operator<(const CCharString& o) const { return strcmp(Text, o.Text) < 0; }
static CCharString Cat(const char* a, const char* b) { size_t n = strlen(a), m = strlen(b); char* p = (char*)malloc(n + m + 1); memcpy(p, a, n); memcpy(p + n, b, m + 1);
    CCharString r(p, -1); free(p); return r; }
CCharString __fastcall operator+(const char* a, const CCharString& b) { return Cat(a, b.Text); }
CCharString __fastcall operator+(const CCharString& a, const CCharString& b) { return Cat(a.Text, b.Text); }
CCharString __fastcall GFIntToCharString(long v) { char b[16]; sprintf(b, "%ld", v); return CCharString(b, -1); }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

// ---- event log -------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 8000 };
static char got[MAXEV + 1]; static int ngot;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static const char* termScript; static int termPos;    // '1' terminating; past the end: terminating
static const char* startScript; static int startPos;  // '1' StartScriptingEntity succeeds; past the end: succeeds
static const char* newScript; static int newPos;      // '0' operator new fails
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

// ---- world -----------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x24]; double align; } prisoner;
static union { unsigned char b[0x1A4]; double align; } parent;
enum { CAGES = 5, FRAMES = 64 };
static unsigned char released[CAGES], killed[CAGES], speak[CAGES];
static unsigned char handles[CAGES][16];
static long frameRound[FRAMES]; static unsigned char frameRel[FRAMES], frameKill[FRAMES], frameSpeak[FRAMES]; static int frame;
static long Cage() { long c; memcpy(&c, prisoner.b + 0x20, 4); return c; }
template <class T> static T& P(int off) { return *(T*)(parent.b + off); }
static void ApplyFrame(int f, long& round, unsigned char& rel, unsigned char& kil, unsigned char& spk)
{ if (f >= FRAMES) f = FRAMES - 1; round = frameRound[f]; rel = frameRel[f]; kil = frameKill[f]; spk = frameSpeak[f]; }

// csargs tree: header, root "$LINE", left child "$CAMERA"
struct Node { int color; Node* parent; Node* left; Node* right; CCharString first; CCharString second; Node(const char* k) : first(k, -1), second("old", -1) { } };
static Node* header; static Node* lineNode; static Node* camNode;

// script base / entity base stubs (thiscall = fastcall with a spare edx)
extern "C" {
static int thingCopies, thingDtors; static void* predicateAddr; static bool predOk;
void __fastcall StubThingCopy(void* self, void*, const void* other) { memcpy(self, other, 12); ++thingCopies; }
void __fastcall StubThingDtor(void* self, void*) { ++thingDtors; if ((char*)self != (char*)predicateAddr + 4) { ++failures; printf("FAIL ~CScriptThing receiver\n"); } }
void __fastcall StubSetPredicate(void* self, void*, const void* pred)
{
    if (self != prisoner.b) { ++failures; printf("FAIL SetPredicate receiver\n"); }
    predicateAddr = (void*)pred; void* vt; memcpy(&vt, pred, 4);
    predOk = vt != 0 && !memcmp((const char*)pred + 4, prisoner.b + 8, 12) && thingCopies == 1 && thingDtors == 0;
    Ev('P');
}
bool __fastcall StubTerminating(void* self, void*)
{
    if (self != prisoner.b) { ++failures; printf("FAIL terminate receiver\n"); }
    bool t = NextBit(termScript, termPos, true); Ev(t ? 'T' : 't'); return t;
}
void __fastcall StubRelease(void* self, void*)
{
    if (self != handles[Cage()]) { ++failures; printf("FAIL Release receiver\n"); }
    memset((char*)self + 8, 0, 4); Ev('R');
}
static const char* spawnName; static unsigned long spawnId; static void* spawnThis;
void __fastcall StubSpawnedFuncBase(void* self, void*, const CCharString* name, unsigned long id) { spawnName = Dup(name->Text); spawnId = id; spawnThis = self; Ev('F'); }
static void* spawnedObj; static char* spawnGroup; static int stringsAtAdd;
void __fastcall StubAddSpawned(void* self, void*, void* f, const CCharString* group)
{
    if (self != parent.b) { ++failures; printf("FAIL AddSpawnedFunction receiver\n"); }
    spawnedObj = f; spawnGroup = Dup(group->Text); stringsAtAdd = liveStrings; Ev(f ? 'A' : 'a');
}
static int randCalls;
int __cdecl rand(void) { ++randCalls; Ev('D'); return 12345; }
}

// parent and prisoner members (real C++ definitions: the names the row object references)
namespace NScript { class CQ_FireHeartScript { public:
    void PlayCutscene(CCharString); void UpdateHistory(bool); void CheckCompletion();
    class CFH_Prisoner { private: void MakeRoundStartComment(); void MakeComment(); void GetEndCutscene(CCharString&) const; void GetEndSpeech(CCharString&) const; };
}; }
using NScript::CQ_FireHeartScript;
static char lastCutscene[64]; static int historyCalls; static bool lastHistory; static int checkCompletionCalls;
void CQ_FireHeartScript::PlayCutscene(CCharString name)
{ if ((void*)this != parent.b) { ++failures; printf("FAIL PlayCutscene receiver\n"); } strncpy(lastCutscene, name.Text, 63); Ev('K'); }
void CQ_FireHeartScript::UpdateHistory(bool b)
{ if ((void*)this != parent.b) { ++failures; printf("FAIL UpdateHistory receiver\n"); } ++historyCalls; lastHistory = b; Ev(b ? 'H' : 'h'); }
void CQ_FireHeartScript::CheckCompletion() { if ((void*)this != parent.b) { ++failures; printf("FAIL CheckCompletion receiver\n"); } ++checkCompletionCalls; }
void CQ_FireHeartScript::CFH_Prisoner::MakeRoundStartComment() { if ((void*)this != prisoner.b) { ++failures; printf("FAIL receiver\n"); } Ev('r'); }
void CQ_FireHeartScript::CFH_Prisoner::MakeComment() { if ((void*)this != prisoner.b) { ++failures; printf("FAIL receiver\n"); } Ev('C'); }
static const CCharString* speechOut;
void CQ_FireHeartScript::CFH_Prisoner::GetEndSpeech(CCharString& s) const
{ speechOut = &s; char b[32]; sprintf(b, "SPEECH_%ld", Cage()); s = CCharString(b, -1); Ev('L'); }
void CQ_FireHeartScript::CFH_Prisoner::GetEndCutscene(CCharString& s) const
{ if (strcmp(s.Text, "")) { ++failures; printf("FAIL cutscene name not empty\n"); } char b[32]; sprintf(b, "CS_FIREHEART_KILL%ld", Cage() + 1); s = CCharString(b, -1); Ev('E'); }

// operator new / delete
static union { unsigned char b[4][0x40]; double align; } pool; static int poolUsed; static unsigned newSize;
void* __cdecl operator new(unsigned int n) { if (!newScript) return malloc(n); bool ok = NextBit(newScript, newPos, true); newSize = n; Ev(ok ? 'n' : '0'); if (!ok || poolUsed >= 4) return 0; return pool.b[poolUsed++]; }
void __cdecl operator delete(void*) { }

// game interface
static void* gameVtbl[300]; static void* gameObj[1]; static bool badGame;
static void CheckGame(void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*)
{
    CheckGame(self); Ev('N'); ++frame; long c = Cage();
    ApplyFrame(frame, P<long>(0x88), released[c], killed[c], speak[c]);
}
static bool __fastcall G_StartScripting(void* self, void*, const void* me, void* handle, int prio)
{
    CheckGame(self);
    if (me != prisoner.b + 8 || handle != handles[Cage()] || prio != 4) { ++failures; printf("FAIL StartScriptingEntity arguments\n"); }
    bool ok = NextBit(startScript, startPos, true); Ev(ok ? 'S' : 's'); return ok;
}
static void __fastcall G_RemoveThing(void* self, void*, const void* thing, bool a, bool b)
{ CheckGame(self); Ev(thing == prisoner.b + 8 && !a && b ? 'X' : '!'); }
static float lastMorality;
static void __fastcall G_GiveHeroMorality(void* self, void*, float m) { CheckGame(self); lastMorality = m; Ev('M'); }
static void __fastcall G_HeroQuit(void* self, void*) { CheckGame(self); Ev('Q'); }

// ---- reference model -----------------------------------------------------------------------------------------------------------
static char want[MAXEV + 1]; static int nwant; static int mt, ms, mn, mframe;
static long mRound; static unsigned char mRel, mKill, mSpeak;
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static bool MT() { bool t = NextBit(termScript, mt, true); W(t ? 'T' : 't'); return t; }
static void MF() { W('N'); ++mframe; ApplyFrame(mframe, mRound, mRel, mKill, mSpeak); }
struct Expect { long freed, killedN, suns, round, guest, scripted; bool chosen, killedAll, freedAll, line, cam, spawned; float morality; int hist; };
static Expect ex;
static bool ModelBody(long cage, bool handleValid)
{
    W('P'); MF(); if (MT()) return false;
    if (handleValid) W('R');
    for (;;) { bool ok = NextBit(startScript, ms, true); W(ok ? 'S' : 's'); if (ok) break; MF(); if (MT()) return false; }
    if (MT()) return false;
    ex.scripted++;
    while (mRound < cage) { MF(); if (MT()) return false; if (mSpeak) { if (MT()) return false; W('C'); } }
    if (MT()) return false;
    ex.guest = cage; W('r');
    while (!ex.chosen) {
        MF(); if (MT()) return false;
        if (mRel) { if (MT()) return false; W('L'); ex.line = true; ex.cam = true; W('K'); ex.freed++; ex.suns--; W('h'); ex.hist = 0; ex.chosen = true; ex.killedAll = false; W('M'); ex.morality = 2.5f; }
        else if (mKill) { if (MT()) return false; W('D'); W('E'); W('K'); ex.killedN++; ex.suns++; W('H'); ex.hist = 1; ex.chosen = true; ex.freedAll = false; W('M'); ex.morality = -4.0f; }
        else if (mSpeak) { if (MT()) return false; W('C'); }
    }
    if (MT()) return false;
    ex.round = mRound + 1;
    bool ok = NextBit(newScript, mn, true); W(ok ? 'n' : '0'); if (ok) { W('F'); W('A'); ex.spawned = true; } else W('a');
    W('X'); W('Q');
    return true;
}

static void Run(long cage, bool handleValid, const char* term, const char* start, const char* nw, int k)
{
    termScript = term; startScript = start; newScript = nw; termPos = startPos = newPos = 0; ngot = 0; nwant = 0; mt = ms = mn = 0;
    // parent
    memset(parent.b, 0x3C, sizeof parent.b);
    memcpy(parent.b + 0x58, &header, 4); long two = 2; memcpy(parent.b + 0x5c, &two, 4);
    P<long>(0x64) = 10; P<long>(0x68) = 20; P<long>(0x6c) = 3; P<long>(0x140) = -1; P<long>(0x144) = 7;
    unsigned char* r = released; unsigned char* kl = killed; unsigned char* sp = speak; unsigned char* h = &handles[0][0];
    memcpy(parent.b + 0xc8, &kl, 4); memcpy(parent.b + 0xd4, &r, 4); memcpy(parent.b + 0xe0, &sp, 4); memcpy(parent.b + 0x148, &h, 4);
    P<float>(0x198) = -4.0f; P<float>(0x19c) = 2.5f; parent.b[0x1a2] = 1; parent.b[0x1a3] = 1;
    memset(released, 0, sizeof released); memset(killed, 0, sizeof killed); memset(speak, 0, sizeof speak);
    memset(handles, 0, sizeof handles); if (handleValid) handles[cage][8] = 1;
    lineNode->second = CCharString("old", -1); camNode->second = CCharString("old", -1);
    frame = 0; ApplyFrame(0, P<long>(0x88), released[cage], killed[cage], speak[cage]);
    // prisoner
    memset(prisoner.b, 0x11, sizeof prisoner.b);
    void* g = gameObj; memcpy(prisoner.b + 4, &g, 4); void* pp = parent.b; memcpy(prisoner.b + 0x14, &pp, 4);
    prisoner.b[0x1c] = 0; memcpy(prisoner.b + 0x20, &cage, 4);
    // model
    memset(&ex, 0, sizeof ex); ex.freed = 10; ex.killedN = 20; ex.suns = 3; ex.round = -2; ex.guest = -1; ex.scripted = 7; ex.killedAll = ex.freedAll = true; ex.hist = -1;
    mframe = 0; ApplyFrame(0, mRound, mRel, mKill, mSpeak);
    bool done = ModelBody(cage, handleValid);
    // run
    thingCopies = thingDtors = 0; predicateAddr = 0; predOk = false; badGame = false; liveStrings = 0; poolUsed = 0; spawnedObj = 0; spawnName = 0; spawnGroup = 0;
    spawnThis = 0; spawnId = 99; historyCalls = 0; lastHistory = false; checkCompletionCalls = 0; lastCutscene[0] = 0; speechOut = 0; randCalls = 0; lastMorality = 0;
    long round0 = P<long>(0x88);
    CallMain(prisoner.b);
    got[ngot] = 0; want[nwant] = 0;
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); return; }
    check(!badGame, "Game receiver", k);
    check(predOk && thingCopies == 1 && thingDtors == 1, "predicate: CIsThingAlive(Me) built from Me, destroyed after SetPredicate", k);
    check(P<long>(0x64) == ex.freed && P<long>(0x68) == ex.killedN && P<long>(0x6c) == ex.suns, "freed / killed / suns counters", k);
    check(P<long>(0x144) == ex.scripted && P<long>(0x140) == ex.guest, "ProphetsScripted / Guest", k);
    check(P<long>(0x88) == (done ? ex.round : (long)P<long>(0x88)), "PuzzleRound advanced", k);
    if (!done) { long rr; unsigned char a, b2, c; ApplyFrame(frame, rr, a, b2, c); check(P<long>(0x88) == (frame ? rr : round0), "PuzzleRound untouched on a terminate exit", k); }
    check((prisoner.b[0x1c] != 0) == ex.chosen, "Chosen", k);
    check((parent.b[0x1a2] != 0) == ex.killedAll && (parent.b[0x1a3] != 0) == ex.freedAll, "KilledAllPrisoners / FreedAllPrisoners", k);
    if (ex.hist >= 0) check(historyCalls == 1 && lastHistory == (ex.hist == 1), "UpdateHistory argument", k);
    if (ex.chosen) check(lastMorality == ex.morality, "GiveHeroMorality amount", k);
    char b[32];
    if (ex.line) { sprintf(b, "SPEECH_%ld", cage); check(speechOut == &lineNode->second && !strcmp(lineNode->second.Text, b), "csargs[$LINE] = end speech", k); }
    else check(!strcmp(lineNode->second.Text, "old"), "csargs[$LINE] untouched", k);
    if (ex.cam) { sprintf(b, "CAM_FH_PRIS%ld", cage); check(!strcmp(camNode->second.Text, b) && !strcmp(lastCutscene, "CS_FIREHEART_RELEASE"), "csargs[$CAMERA] and the release cutscene", k); }
    else check(!strcmp(camNode->second.Text, "old"), "csargs[$CAMERA] untouched", k);
    if (ex.hist == 1) { sprintf(b, "CS_FIREHEART_KILL%ld", cage + 1); check(!strcmp(lastCutscene, b) && randCalls == 1, "killed: end cutscene, one rand call", k); }
    if (ex.spawned) {
        check(spawnedObj == pool.b[0] && newSize == 0x3C && spawnThis == spawnedObj, "spawn storage is the new(0x3C) block", k);
        check(spawnName && !strcmp(spawnName, "ParentClass.CheckCompletion") && spawnId == 0, "spawn name and id", k);
        check(spawnGroup && !strcmp(spawnGroup, "") && stringsAtAdd == 4, "spawn group; the two name parts, their concatenation and the group alive at the call", k);
        void* pc; memcpy(&pc, (char*)spawnedObj + 0x38, 4); check(pc == parent.b, "spawn PClass", k);
        void* vt; memcpy(&vt, spawnedObj, 4); ((unsigned char*)spawnedObj)[5] = 0;
        typedef void (__fastcall* ProcFn)(void*, void*); ((ProcFn)((void**)vt)[4])(spawnedObj, 0);
        check(checkCompletionCalls == 1 && ((unsigned char*)spawnedObj)[5] == 1, "slot 4 runs CheckCompletion on the parent and stops", k);
    }
    check(liveStrings == 0, "every temporary CCharString destroyed", k);
    if (spawnName) free((void*)spawnName); if (spawnGroup) free(spawnGroup);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    header = new Node("hdr"); lineNode = new Node("$LINE"); camNode = new Node("$CAMERA");
    header->parent = lineNode; header->left = camNode; header->right = lineNode;
    lineNode->parent = header; lineNode->left = camNode; lineNode->right = 0;
    camNode->parent = lineNode; camNode->left = 0; camNode->right = 0;
    poolUsed = 0;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[8] = (void*)G_StartScripting; gameVtbl[108] = (void*)G_RemoveThing;
    gameVtbl[156] = (void*)G_GiveHeroMorality; gameVtbl[255] = (void*)G_HeroQuit;
    int k = 0;
    // targeted: released at frame 3, killed at frame 3, speaking, start retries, terminate at each test
    for (long cage = 0; cage < CAGES; ++cage)
        for (int mode = 0; mode < 3; ++mode) {
            for (int f = 0; f < FRAMES; ++f) { frameRound[f] = f / 2; frameRel[f] = mode == 0 && f >= 3 + 2 * cage; frameKill[f] = mode == 1 && f >= 3 + 2 * cage; frameSpeak[f] = (f % 3) == 1; }
            if (mode == 2) frameRel[FRAMES - 1] = 1;
            Run(cage, cage & 1, "0000000000000000000000000000000000000000000000000000000000000000000000000000", "0101", "1", ++k);
            Run(cage, true, "0000000000000000000000000000000000000000000000000000000000000000000000000000", "1", "0", ++k);
            for (int t = 0; t < 20; ++t) { char ts[64]; memset(ts, '0', sizeof ts); ts[t] = '1'; ts[63] = 0; Run(cage, t & 1, ts, "001", "1", ++k); }
        }
    unsigned long seed = 5;
    for (int r = 0; r < 6000; ++r) {
        static char ts[200], ss[8], ns[2];
        for (int i = 0; i < 199; ++i) { seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 25 == 0) ? '1' : '0'; }
        ts[199] = 0;
        for (int i = 0; i < 7; ++i) { seed = seed * 1103515245ul + 12345ul; ss[i] = ((seed >> 16) % 3 == 0) ? '0' : '1'; } ss[7] = 0;
        seed = seed * 1103515245ul + 12345ul; ns[0] = ((seed >> 16) % 4 == 0) ? '0' : '1'; ns[1] = 0;
        seed = seed * 1103515245ul + 12345ul; long cage = (long)((seed >> 16) % CAGES);
        long round = 0; int relAt = -1, killAt = -1;
        seed = seed * 1103515245ul + 12345ul; int at = 1 + (int)((seed >> 16) % 40);
        seed = seed * 1103515245ul + 12345ul; if ((seed >> 16) % 2) relAt = at; else killAt = at;
        for (int f = 0; f < FRAMES; ++f) {
            seed = seed * 1103515245ul + 12345ul; if ((seed >> 16) % 3 == 0) ++round;
            frameRound[f] = round; frameRel[f] = relAt >= 0 && f >= relAt; frameKill[f] = killAt >= 0 && f >= killAt;
            seed = seed * 1103515245ul + 12345ul; frameSpeak[f] = (seed >> 16) % 4 == 0;
        }
        frameRel[FRAMES - 1] = 1;
        Run(cage, (r & 2) != 0, ts, ss, ns, 1000 + r);
    }
    if (failures) { printf("00d3a4a0_TEST FAIL %d\n", failures); return 1; }
    printf("00d3a4a0_TEST PASS (%d targeted, 6000 random)\n", k);
    return 0;
}