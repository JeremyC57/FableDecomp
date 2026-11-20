// Behaviour test for 0x00E37E40 NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::Main (links the REAL row object: the row body,
// its expanded CIsThingAlive constructor / CScriptThing::GetPos / CTimer members and the out-of-line CTimer COMDATs it calls all run as
// compiled). No class models: every external the row names is bound with /alternatename to an extern "C" __fastcall stub (thiscall-
// compatible, unused edx), every virtual it reaches goes through a hand-built vtable (Game slots 7, 12, 15, 70, 72, 91, 284, 421, 585;
// pDSTGame slots 87..90 on a SEPARATE object, so the timer calls are proved to use the global; Me's slot 3; the marker implementation's
// slot 6). Fixtures use the native layout of every offset an instruction of the row reads (entity +4 / +8 / +0x14 / +0x1C / +0x20,
// parent +0x4C / +0x54 / +0x58 + 16 * i + 0xC, script def +0x39C / +0x3A8 / +0x3C0 / +0x3C4).
// Reference model: Model() below, written from the script's statements; every run is compared event by event plus the final state
// (parent counter / region flags, regionName / regionId, live strings, things and timers back to zero).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); ExitProcess(3); }
#pragma comment(linker, "/alternatename:??_GCPredicateBase@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_GCIsThingAlive@NScript@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Clone@CIsThingAlive@NScript@@UBEPAVCPredicateBase@2@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??RCIsThingAlive@NScript@@UBE_NXZ=@FilterCallOp@8")
#pragma comment(linker, "/alternatename:?NullCoord@CScriptThing@@1VC3DVector@@A=_g_NullCoord")
#pragma comment(linker, "/alternatename:?pDSTGame@@3PAVCGameScriptInterfaceBase@@A=_g_pDSTGame")
#pragma comment(linker, "/alternatename:?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A=_g_ScriptDefHandle")
#pragma comment(linker, "/alternatename:??0CScriptThing@@QAE@ABV0@@Z=@ThingCopy@12")
#pragma comment(linker, "/alternatename:??1CScriptThing@@UAE@XZ=@ThingDtor@8")
#pragma comment(linker, "/alternatename:??1CIsThingAlive@NScript@@UAE@XZ=@FilterDtor@8")
#pragma comment(linker, "/alternatename:?SetPredicate@CEntityScriptBase@@QAEXABVCPredicateBase@NScript@@@Z=@SetPredicate@12")
#pragma comment(linker, "/alternatename:?IsActiveThreadTerminating@CEntityScriptBase@@QBE_NXZ=@IsTerm@8")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@PBDJ@Z=@StrCtor@16")
#pragma comment(linker, "/alternatename:??0CCharString@@QAE@ABV0@@Z=@StrCopy@12")
#pragma comment(linker, "/alternatename:??1CCharString@@QAE@XZ=@StrDtor@8")
#pragma comment(linker, "/alternatename:??4CCharString@@QAEABV0@ABV0@@Z=@StrAssign@12")
#pragma comment(linker, "/alternatename:??9CCharString@@QBE_NABV0@@Z=@StrNotEq@12")
#pragma comment(linker, "/alternatename:?IsDistanceBetweenThingsUnder@@YI_NABVCScriptThing@@0M@Z=@DistUnder@12")
#pragma comment(linker, "/alternatename:?GetRegionIdForRegion@CV_AssassinAttacksScript@NScript@@QAE?AW4AA_LocationIdentifier@12@VCCharString@@@Z=@RegionId@12")
extern "C" void __fastcall CallMain(void* self);   // the row (private virtual, thiscall: this in ecx)
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CM_AssassinTrigger@CV_AssassinAttacksScript@NScript@@EAEXXZ")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
void __cdecl operator delete(void*) { VirtualTrap(); }

// ---- scripted inputs ------------------------------------------------------------------------------------------------------------
struct Script { const char* s; int pos; bool dflt; };
static bool Next(Script& sc) { char c = sc.s[sc.pos]; if (!c) return sc.dflt; ++sc.pos; return c == '1'; }
static Script sTerm, sDist, sScreen, sTimer;       // past the end: terminating / in range / off screen / timer run out
static bool inKnothole, ransomActive, nullMarker;
static long regionIdResult, objectCount, objectTotal; static bool activeFlags[5]; static const char* curRegionText; static const char* lastKillText;

// ---- event log ------------------------------------------------------------------------------------------------------------------
enum { MAXEV = 6000 };
static char got[MAXEV + 1]; static int ngot;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }

// ---- fixtures -------------------------------------------------------------------------------------------------------------------
static union { unsigned char b[0x40]; double a; } self;                // CM_AssassinTrigger (0x24)
static union { unsigned char b[0xC0]; double a; } parent;              // CV_AssassinAttacksScript (0xB8)
static union { unsigned char b[0x400]; double a; } def;                // CScriptDef (fields to +0x3C8)
extern "C" void* g_ScriptDefHandle = def.b;                            // CDefPointer<const CScriptDef> {Object}
extern "C" float g_NullCoord[3] = { 0.0f, 0.0f, 0.0f };
static void* gameVtbl[600]; static void* gameObj[1];
static void* dstVtbl[600]; static void* dstObj[1];
extern "C" void* g_pDSTGame = dstObj;
static void* meVtbl[80]; static void* implVtbl[80]; static void* implObj[1]; static void* markerVtbl[1]; static void* creatureVtbl[1];
static int meInfo, markerInfo, creatureInfo;                            // stand-in PImp.Info blocks (identity only)
static unsigned char hero[12];
static float markerPos[3] = { 12.5f, -4.25f, 3.0f };
static const char* objectTexts[8] = { "OBJ_A", "OBJ_B", "OBJ_C", "OBJ_D", "OBJ_E", "OBJ_F", "OBJ_G", "OBJ_H" };
static const char* regionText = "RegionInitial";
static char dataTextBuf[] = "SpawnMarker"; static const char* dataText = dataTextBuf;

// strings: one text pointer
static int liveStrings;
extern "C" void* __fastcall StrCtor(const char** s, void*, const char* text, long len) { *s = text; ++liveStrings; if (len != -1) check(false, "string length", 0); return s; }
extern "C" void* __fastcall StrCopy(const char** s, void*, const char* const* o) { *s = *o; ++liveStrings; return s; }
extern "C" void __fastcall StrDtor(const char** s, void*) { --liveStrings; *s = 0; }
extern "C" void* __fastcall StrAssign(const char** s, void*, const char* const* o) { *s = *o; Ev('='); return s; }
extern "C" bool __fastcall StrNotEq(const char* const* s, void*, const char* const* o)
{
    check(s == (const char* const*)(self.b + 0x1C) && o == (const char* const*)(parent.b + 0x54), "operator!= operands", 0);
    bool r = strcmp(*s, *o) != 0; Ev(r ? '!' : '~'); return r;
}

// script things: {vptr, Data, Info}
static int liveThings; static int filterCopies;
extern "C" void* __fastcall ThingCopy(void** t, void*, void* const* o) { t[0] = o[0]; t[1] = o[1]; t[2] = o[2]; ++liveThings; Ev('C'); return t; }
extern "C" void __fastcall ThingDtor(void** t, void*) { --liveThings; Ev(t[2] == &creatureInfo ? 'd' : t[2] == &markerInfo ? 'm' : 'D'); t[0] = t[1] = t[2] = 0; }
// filter: {vptr, Thing}
static void* filterVptr; static void* filterAddr;
extern "C" bool __fastcall FilterCallOp(void*, void*) { VirtualTrap(); return false; }
extern "C" void __fastcall SetPredicate(void* ent, void*, void** filter)
{
    check(ent == self.b, "SetPredicate receiver", 0);
    void** vt = (void**)filter[0];
    check(vt && vt[0] == (void*)FilterCallOp, "filter vtable slot 0 = CIsThingAlive::operator()", 0);
    check(filter[1] == meVtbl && filter[2] == implObj && filter[3] == &meInfo, "filter Thing is a copy of Me", 0);
    filterAddr = filter; Ev('S');
}
extern "C" void __fastcall FilterDtor(void** filter, void*) { check(filter == filterAddr, "filter destroyed", 0); --liveThings; Ev('K'); }
extern "C" bool __fastcall IsTerm(void* ent, void*) { check(ent == self.b, "terminate receiver", 0); bool t = Next(sTerm); Ev(t ? 'T' : 't'); return t; }
extern "C" bool __fastcall DistUnder(const void* a, const void* b, float d)
{
    check(a == hero && b == self.b + 8, "distance things (hero, Me)", 0);
    long prox; memcpy(&prox, def.b + 0x39C, 4); check(d == (float)prox, "distance = AssassinTriggerProximity", 0);
    bool u = Next(sDist); Ev(u ? 'U' : 'u'); return u;
}
extern "C" long __fastcall RegionId(void* p, void*, const char* nameByValue)
{
    check(p == parent.b, "GetRegionIdForRegion receiver", 0);
    check(nameByValue == curRegionText, "GetRegionIdForRegion gets a copy of regionName", 0);
    --liveStrings;                                                       // callee destroys its by-value CCharString
    Ev('I'); return regionIdResult;
}

// Me (slot 3 GetDataString) and the marker's implementation (slot 6 GetPos)
static void* __fastcall Me_GetDataString(void* t, void*, const char** ret) { check(t == self.b + 8, "GetDataString on Me", 0); *ret = dataText; ++liveStrings; Ev('g'); return ret; }
static const float* __fastcall Impl_GetPos(void* t, void*) { check(t == implObj, "GetPos receiver", 0); Ev('p'); return markerPos; }

// game interface
static void CheckGame(void* t) { if (t != gameObj) check(false, "Game receiver", 0); }
static void __fastcall G_NewScriptFrame(void* t, void*) { CheckGame(t); Ev('N'); }
static bool __fastcall G_IsRegionLoaded(void* t, void*, const char* const* n) { CheckGame(t); check(!strcmp(*n, "KnotholeGlade"), "region name", 0); Ev('L'); return inKnothole; }
static bool __fastcall G_IsQuestActive(void* t, void*, const char* const* n) { CheckGame(t); check(!strcmp(*n, "Q_RansomVictim"), "quest name", 0); Ev('Q'); return ransomActive; }
static const char* const* __fastcall G_GetRegionName(void* t, void*) { CheckGame(t); Ev('r'); return &curRegionText; }
static void* __fastcall G_GetHero(void* t, void*) { CheckGame(t); Ev('H'); return hero; }
static void** markerAddr;
static void* __fastcall G_GetThingWithScriptName(void* t, void*, void** ret, const char* const* n)
{
    CheckGame(t); check(*n == dataText, "marker name = Me's data string (the same CCharString text)", 0);
    ret[0] = markerVtbl; ret[1] = nullMarker ? 0 : (void*)implObj; ret[2] = &markerInfo; ++liveThings; markerAddr = ret; Ev('G'); return ret;
}
static void CheckPos(const float* p) { const float* w = nullMarker ? g_NullCoord : markerPos; check(!memcmp(p, w, 12), "spawn position", 0); }
static bool __fastcall G_IsCameraPosOnScreen(void* t, void*, const float* p) { CheckGame(t); CheckPos(p); bool o = !Next(sScreen); Ev(o ? 'O' : 'o'); return o; }
static void** creatureAddr; static int createStrings;
static void* __fastcall G_CreateCreature(void* t, void*, void** ret, const char* const* def_, const float* pos, const char* const* script, bool b)
{
    CheckGame(t); check(!strcmp(*def_, "CREATURE_ASSASSIN") && !strcmp(*script, "AttackingAssassin") && !b, "CreateCreature arguments", 0);
    CheckPos(pos); createStrings = liveStrings;
    ret[0] = creatureVtbl; ret[1] = 0; ret[2] = &creatureInfo; ++liveThings; creatureAddr = ret; Ev('c'); return ret;
}
static const char* addedItem;
static void __fastcall G_AddItemToContainer(void* t, void*, void* const* thing, const char* const* item)
{
    CheckGame(t); check(thing == (void* const*)creatureAddr && thing[2] == &creatureInfo, "AddItemToContainer thing = the new assassin", 0);
    addedItem = *item; Ev('A');
}
// pDSTGame timers
static long timerIndex; static int liveTimers; static long timerSet;
static long __fastcall D_RegisterTimer(void* t, void*) { check(t == dstObj, "timer via pDSTGame", 0); ++liveTimers; Ev('R'); return timerIndex = 0x50 + liveTimers; }
static void __fastcall D_DeregisterTimer(void* t, void*, long i) { check(t == dstObj && i == timerIndex, "DeregisterTimer", 0); --liveTimers; Ev('X'); }
static void __fastcall D_SetTimer(void* t, void*, long i, long v) { check(t == dstObj && i == timerIndex, "SetTimer", 0); timerSet = v; Ev('Z'); }
static long __fastcall D_GetTimer(void* t, void*, long i) { check(t == dstObj && i == timerIndex, "GetTimer", 0); bool z = Next(sTimer); Ev(z ? 'z' : 'w'); return z ? 0 : timerSet; }

// ---- reference model ------------------------------------------------------------------------------------------------------------
static char want[MAXEV + 1]; static int nwant;
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static Script mTerm, mDist, mScreen, mTimer;
static bool MT() { bool t = Next(mTerm); W(t ? 'T' : 't'); return t; }
static bool MO() { bool o = !Next(mScreen); W(o ? 'O' : 'o'); return o; }
static long mCount; static bool mActive[5]; static bool mRegionSet; static const char* mItem;
static void Model()
{
    mItem = 0; mRegionSet = false; mCount = objectCount; memcpy(mActive, activeFlags, sizeof mActive);
    W('C'); W('S'); W('K'); W('N');
    if (MT()) return;
    W('L'); if (inKnothole) W('Q');
    if (inKnothole && ransomActive) return;
    if (MT()) return;
    W('g'); W('G'); if (!nullMarker) W('p');
    for (;;) { W('H'); bool u = Next(mDist); W(u ? 'U' : 'u'); if (u) break; W('N'); if (MT()) { W('m'); return; } }
    if (MT()) { W('m'); return; }
    W('R');
    for (;;) {
        if (MT()) break;
        bool term = false;
        while (MO()) { W('N'); if (MT()) { term = true; break; } }
        if (term) break;
        if (MT()) break;
        W('Z');
        while (MO()) { bool z = Next(mTimer); W(z ? 'z' : 'w'); if (z) break; W('N'); if (MT()) { term = true; break; } }
        if (term) break;
        if (MT()) break;
        if (!MO()) {
            if (MT()) break;
            if (MT()) break;
            W('r'); W('='); W('I'); mRegionSet = true;
            if (mActive[regionIdResult]) break;
            bool ne = strcmp(curRegionText, lastKillText) != 0; W(ne ? '!' : '~');
            if (ne) {
                if (MT()) break;
                W('c'); mActive[regionIdResult] = true;
                mItem = objectTexts[mCount]; W('A');
                ++mCount; if ((unsigned long)mCount >= (unsigned long)objectTotal) mCount = 0;
                W('d');
            }
            break;
        }
        W('N');
    }
    W('X'); W('m');
}

static void Run(const char* term, const char* dist, const char* screen, const char* timer, bool knot, bool ransom, bool nullM,
                long rid, long count, long total, unsigned activeMask, const char* cur, const char* last, int k)
{
    sTerm.s = mTerm.s = term; sDist.s = mDist.s = dist; sScreen.s = mScreen.s = screen; sTimer.s = mTimer.s = timer;
    sTerm.pos = sDist.pos = sScreen.pos = sTimer.pos = mTerm.pos = mDist.pos = mScreen.pos = mTimer.pos = 0;
    sTerm.dflt = mTerm.dflt = true; sDist.dflt = mDist.dflt = true; sScreen.dflt = mScreen.dflt = true; sTimer.dflt = mTimer.dflt = true;
    inKnothole = knot; ransomActive = ransom; nullMarker = nullM; regionIdResult = rid; objectCount = count; objectTotal = total;
    for (int i = 0; i < 5; ++i) activeFlags[i] = ((activeMask >> i) & 1) != 0;
    curRegionText = cur; lastKillText = last;
    ngot = nwant = 0; Model();
    liveStrings = liveThings = liveTimers = 0; addedItem = 0; creatureAddr = 0; markerAddr = 0; filterAddr = 0; timerSet = -1;
    memset(self.b, 0x5A, sizeof self.b); memset(parent.b, 0x6B, sizeof parent.b); memset(def.b, 0x7C, sizeof def.b);
    void* p;
    p = meVtbl; memcpy(self.b + 0x08, &p, 4); p = implObj; memcpy(self.b + 0x0C, &p, 4); p = &meInfo; memcpy(self.b + 0x10, &p, 4);
    p = gameObj; memcpy(self.b + 0x04, &p, 4); p = parent.b; memcpy(self.b + 0x14, &p, 4);
    memcpy(self.b + 0x1C, &regionText, 4); long oldId = 0x1234; memcpy(self.b + 0x20, &oldId, 4);
    memcpy(parent.b + 0x4C, &count, 4); memcpy(parent.b + 0x54, &lastKillText, 4);
    for (int i = 0; i < 5; ++i) parent.b[0x58 + 16 * i + 0xC] = activeFlags[i];
    long prox = 7 + k % 13, offscreen = 30 + k % 7; memcpy(def.b + 0x39C, &prox, 4); memcpy(def.b + 0x3A8, &offscreen, 4);
    const char** first = objectTexts; const char** last_ = objectTexts + total; memcpy(def.b + 0x3C0, &first, 4); memcpy(def.b + 0x3C4, &last_, 4);
    CallMain(self.b);
    got[ngot] = 0; want[nwant] = 0;
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); return; }
    long c; memcpy(&c, parent.b + 0x4C, 4); check(c == mCount, "AssassinObjectCount", k);
    for (int i = 0; i < 5; ++i) check(parent.b[0x58 + 16 * i + 0xC] == (unsigned char)mActive[i], "RegionInfo assassinActive", k);
    const char* rn; memcpy(&rn, self.b + 0x1C, 4); long id; memcpy(&id, self.b + 0x20, 4);
    if (mRegionSet) check(rn == curRegionText && id == rid, "regionName / regionId stored", k);
    else check(rn == regionText && id == 0x1234, "regionName / regionId untouched", k);
    check(addedItem == mItem, "item given = AssassinObjects[AssassinObjectCount]", k);
    if (mItem) check(createStrings == 3, "CreateCreature: data and the two literal temporaries alive", k);
    if (strchr(want, 'Z')) { long o; memcpy(&o, def.b + 0x3A8, 4); check(timerSet == o, "timer = AssassinCameraOffScreenTime", k); }
    check(liveStrings == 0, "every CCharString destroyed", k);
    check(liveThings == 0, "every CScriptThing destroyed", k);
    check(liveTimers == 0, "timer deregistered", k);
    for (int i = 0x24; i < 0x40; ++i) check(self.b[i] == 0x5A, "entity storage beyond 0x24 untouched", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    gameObj[0] = gameVtbl; dstObj[0] = dstVtbl; implObj[0] = implVtbl;
    for (int i = 0; i < 600; ++i) gameVtbl[i] = dstVtbl[i] = (void*)VirtualTrap;
    for (int i = 0; i < 80; ++i) meVtbl[i] = implVtbl[i] = (void*)VirtualTrap;
    markerVtbl[0] = creatureVtbl[0] = (void*)VirtualTrap;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[12] = (void*)G_IsRegionLoaded; gameVtbl[15] = (void*)G_GetRegionName;
    gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThingWithScriptName; gameVtbl[91] = (void*)G_CreateCreature;
    gameVtbl[284] = (void*)G_IsQuestActive; gameVtbl[421] = (void*)G_IsCameraPosOnScreen; gameVtbl[585] = (void*)G_AddItemToContainer;
    dstVtbl[87] = (void*)D_RegisterTimer; dstVtbl[88] = (void*)D_DeregisterTimer; dstVtbl[89] = (void*)D_SetTimer; dstVtbl[90] = (void*)D_GetTimer;
    meVtbl[3] = (void*)Me_GetDataString; implVtbl[6] = (void*)Impl_GetPos;
    int k = 0;
    const char* A = "RegionA"; const char* B = "RegionB";
    for (int nm = 0; nm < 2; ++nm) {
        Run("1", "", "", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);                      // terminating at the first frame
        Run("00", "", "", "", true, true, nm, 0, 0, 3, 0, A, B, ++k);                       // Knothole + ransom quest: nothing
        Run("01", "", "", "", true, false, nm, 0, 0, 3, 0, A, B, ++k);                      // Knothole, quest inactive
        Run("001", "", "", "", false, true, nm, 0, 0, 3, 0, A, B, ++k);
        Run("0001", "0", "", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);                  // terminating in the distance wait
        Run("0000000000", "1", "1", "", false, false, nm, 2, 1, 3, 0, A, B, ++k);          // straight to the spawn
        Run("0000000000", "1", "1", "", false, false, nm, 2, 2, 3, 0, A, B, ++k);          // counter wraps
        Run("0000000000", "1", "1", "", false, false, nm, 4, 0, 1, 0, A, B, ++k);
        Run("0000000000", "1", "1", "", false, false, nm, 2, 1, 3, 4, A, B, ++k);          // region already has an assassin
        Run("0000000000", "1", "1", "", false, false, nm, 2, 1, 3, 0, A, A, ++k);          // most recent kill region
        Run("0000000000", "001", "01", "", false, false, nm, 1, 0, 2, 0, A, B, ++k);
        Run("000000000000", "1", "001", "1", false, false, nm, 3, 1, 4, 0, B, A, ++k);     // waits on screen, timer runs out
        Run("0000000000000000", "1", "0001", "001", false, false, nm, 3, 1, 4, 0, B, A, ++k);
        Run("000000001", "1", "000", "000", false, false, nm, 0, 0, 3, 0, A, B, ++k);      // terminating in the timed wait
        Run("00001", "1", "0", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);               // terminating at the loop head
        Run("000001", "1", "000", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);            // terminating in the first wait
        Run("00000001", "1", "1", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);
        Run("0000000001", "1", "1", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);
        Run("00000000001", "1", "1", "", false, false, nm, 0, 0, 3, 0, A, B, ++k);
    }
    unsigned long seed = 23;
    for (int r = 0; r < 20000; ++r) {
        static char ts[96], ds[16], ss[64], tm[64];
        for (int i = 0; i < 95; ++i) { seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 23 == 0) ? '1' : '0'; }
        for (int i = 0; i < 15; ++i) { seed = seed * 1103515245ul + 12345ul; ds[i] = ((seed >> 16) % 3 == 0) ? '1' : '0'; }
        for (int i = 0; i < 63; ++i) { seed = seed * 1103515245ul + 12345ul; ss[i] = ((seed >> 16) % 3 == 0) ? '1' : '0'; }
        for (int i = 0; i < 63; ++i) { seed = seed * 1103515245ul + 12345ul; tm[i] = ((seed >> 16) % 4 == 0) ? '1' : '0'; }
        ts[95] = ds[15] = ss[63] = tm[63] = 0;
        seed = seed * 1103515245ul + 12345ul; unsigned v = (unsigned)(seed >> 8);
        long total = 1 + v % 8, count = (v >> 3) % total, rid = (v >> 6) % 5; unsigned mask = (v >> 9) & 31;
        bool knot = ((v >> 14) & 3) == 0, ransom = ((v >> 16) & 1) != 0, nm = ((v >> 17) % 5) == 0;
        Run(ts, ds, ss, tm, knot, ransom, nm, rid, count, total, mask, ((v >> 20) & 1) ? A : B, ((v >> 21) & 1) ? A : B, 1000 + r);
    }
    if (failures) { printf("00e37e40_TEST FAIL %d\n", failures); return 1; }
    printf("00e37e40_TEST PASS (%d targeted, 20000 random)\n", k);
    return 0;
}