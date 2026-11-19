// Behaviour test for 0x00D6C830 NScript::CQ_HangingTreeEvilScript::CheckBanditFollowStatus (links the REAL row object, built with
// its /d2inlT62 flag: the row body, the expanded and the called ~CScriptThing copies and TRetVoid all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the script object's +0x40 Game pointer (0x00D6C848),
// SwitchingRegions +0x51 (0x00D6C8D9), ShouldFollowHero +0x58 (0x00D6C880 / 0x00D6C8DC); Game vtable slots 7 NewScriptFrame (+0x1C),
// 70 GetHero (+0x118, returns CScriptThing&), 72 GetThingWithScriptName (+0x120, hidden return pointer). The fake interface methods
// are __fastcall functions with an unused edx parameter (thiscall-compatible). Each exit point returned by slot 72 is raw storage
// {vptr, PImp.Data, PImp.Info} over its own CCPPointerInfo {RefCount, DeleteFunc (__fastcall), Data}, so every release is observable;
// CScriptThing::operator= (0x008AB980, out of line) is defined here as the counted-pointer share (add the new reference, release the
// old one). CScriptThing's other virtuals are referenced only through the vtable the inline destructor stores; they alias one trap.
// The world changes only inside NewScriptFrame: a per-frame script sets / clears SwitchingRegions and ShouldFollowHero.
// Reference model: the generated-script form written independently below; every scripted run is compared event by event (frames,
// terminate tests, distance queries with their arguments, exit-point fetches and assignments, releases, CCharString balance).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected CScriptThing virtual call\n"); exit(3); }
#pragma comment(linker, "/alternatename:??_GCScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DecrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetActivationTriggerStatus@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetAngleXY@CScriptThing@@UBEMXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentScriptPriority@CScriptThing@@UBE?AW4EScriptAIPriority@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetCurrentStateGroupType@CScriptThing@@UBE?AW4EAIStateGroupType@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetHomeMapName@CScriptThing@@UBE?AVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetName@CScriptThing@@UBEABVCCharString@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPThing@CScriptThing@@UBEPAVCThing@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPThingUniqueID@CScriptThing@@UBE_KXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetPos@CScriptThing@@UBEABVC3DVector@@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetScriptCounter@CScriptThing@@UAEJXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IncrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsAwareOfHero@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsOpenDoor@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSneaking@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsSummonedCreature@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUnconscious@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?IsUsable@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgAttackedWithBareHandsWithoutHittingAnything@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgAttackedWithMeleeWeaponWithoutHittingAnything@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgExpressionPerformedTo@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgGetThingsKilled@CScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithBareHands@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithMeleeWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithRangedWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithBareHands@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithMeleeWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHitFriendWithRangedWeapon@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgHowLongWasExpressionPerformed@CScriptThing@@UBEJXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyNonAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithDecapitate@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithFlourish@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithProjectileWeapon@CScriptThing@@UBE_NABVCCharString@@AAM@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsHitByWithWeapon@CScriptThing@@UBE_NABVCCharString@@0@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKicked@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKilledBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsKnockedOutBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsTriggeredBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgIsUsedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOnCutSceneAnimEvent@CScriptThing@@UBE_NAAVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOnMoralityChanged@CScriptThing@@UBE_NAAJ@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgOpenedChest@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnyAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnyNonAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedAnySpecialAbility@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedFlourish@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedSpecialAbility@CScriptThing@@UBE_NW4EHeroAbility@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgPerformedSuccessfulFlourish@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgReceivedInventoryItem@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgReceivedMoney@CScriptThing@@UBE_NAAJ@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgTalkedToAnyone@CScriptThing@@UBE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoHitMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoKilledMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoKnockedMeOut@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?MsgWhoTalkedToMe@CScriptThing@@UBE?AV1@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetActivationTriggerStatus@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetAsUsable@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetFriendsWithEverythingFlag@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?UpdateThingAttachment@CScriptThing@@UAEXXZ=_VirtualTrap")

class CCharString { public: CCharString(const char* text, long length); ~CCharString(); const char* Text; };
class CBaseClass { public: virtual ~CBaseClass(); };
class CScriptBase { public: bool IsActiveThreadTerminating(); };
class CScriptThing { public: CScriptThing& operator=(const CScriptThing& other); };
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);
namespace NScript { class CQ_HangingTreeEvilScript { public: void CheckBanditFollowStatus(); }; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

static int liveStrings;
CCharString::CCharString(const char* text, long length) { Text = text; ++liveStrings; if (length != -1) { printf("FAIL CCharString length %ld\n", length); ++failures; } }
CCharString::~CCharString() { --liveStrings; Text = 0; }

// ---- event log and scripted world ----------------------------------------------------------------------------------------------
enum { MAXEV = 6000, MAXEXIT = 64 };
static char got[MAXEV + 1]; static int ngot;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static const char* termScript; static int termPos;    // '1' = terminating; past the end: terminating (bounds every run)
static const char* distScript; static int distPos;    // '1' = hero within the distance; past the end: '0'
static const char* worldScript; static int worldPos;  // per frame: 'S' / 's' set / clear SwitchingRegions, 'F' / 'f' set / clear
                                                      // ShouldFollowHero, anything else no change; past the end: no change
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

static union { unsigned char b[0x100]; double align; } scriptObj;
enum { SWITCHING = 0x51, FOLLOW = 0x58 };
bool CScriptBase::IsActiveThreadTerminating()
{
    if ((void*)this != scriptObj.b) { printf("FAIL terminate receiver\n"); ++failures; }
    bool t = NextBit(termScript, termPos, true); Ev(t ? 'T' : 't'); return t;
}

// exit points: thing i = {vptr, Data = &data[i], Info = &info[i]}
struct InfoBlock { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
static InfoBlock info[MAXEXIT]; static unsigned char data[MAXEXIT][8]; static int nexits;
static int deleteFuncCalls[MAXEXIT], operatorDeletes, baseDtors;
static void __fastcall DeleteExitData(void* d) { int i = (int)(((unsigned char*)d - data[0]) / 8); if (i >= 0 && i < MAXEXIT) ++deleteFuncCalls[i]; }
void __cdecl operator delete(void* p) { ++operatorDeletes; }
static void* localAddr;
static void* tempAddr;
CBaseClass::~CBaseClass() { ++baseDtors; if ((void*)this == localAddr) Ev('X'); else if ((void*)this == tempAddr) Ev('x'); else { printf("FAIL ~CBaseClass receiver\n"); ++failures; } }
static int ExitOf(const void* thing) { void* d; memcpy(&d, (const char*)thing + 4, 4); return (int)(((unsigned char*)d - data[0]) / 8); }
static void Release(InfoBlock* ib) { if (ib && --ib->RefCount == 0) { ib->DeleteFunc(ib->Data); operator delete(ib); } }
CScriptThing& CScriptThing::operator=(const CScriptThing& other)
{
    if ((void*)this != localAddr) { printf("FAIL operator= receiver\n"); ++failures; }
    InfoBlock* mine; InfoBlock* theirs; memcpy(&mine, (char*)this + 8, 4); memcpy(&theirs, (const char*)&other + 8, 4);
    if (theirs) ++theirs->RefCount;
    Release(mine);
    memcpy((char*)this + 4, (const char*)&other + 4, 8);
    Ev('A'); Ev((char)('a' + ExitOf(this)));
    return *this;
}

static void* gameVtbl[700]; static void* gameObj[1]; static bool badGame;
static unsigned char heroThing[12];
static void CheckGame(void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*)
{
    CheckGame(self); Ev('N');
    char c = worldScript[worldPos]; if (c) ++worldPos;
    if (c == 'S') scriptObj.b[SWITCHING] = 1; else if (c == 's') scriptObj.b[SWITCHING] = 0;
    else if (c == 'F') scriptObj.b[FOLLOW] = 1; else if (c == 'f') scriptObj.b[FOLLOW] = 0;
}
static void* __fastcall G_GetHero(void* self, void*) { CheckGame(self); Ev('H'); return heroThing; }
static void* __fastcall G_GetThingWithScriptName(void* self, void*, void* ret, const CCharString* name)
{
    CheckGame(self); if (!name->Text || strcmp(name->Text, "HTE_BanditExitPoint")) { printf("FAIL exit point name\n"); ++failures; }
    static void* fakeVtbl[1];
    int i = nexits < MAXEXIT - 1 ? nexits++ : MAXEXIT - 1;
    info[i].RefCount = 1; info[i].DeleteFunc = DeleteExitData; info[i].Data = data[i];
    void* v = fakeVtbl; memcpy(ret, &v, 4); void* d = data[i]; memcpy((char*)ret + 4, &d, 4); InfoBlock* ib = &info[i]; memcpy((char*)ret + 8, &ib, 4);
    if (!localAddr) localAddr = ret; else tempAddr = ret;
    Ev('G'); Ev((char)('a' + i)); return ret;
}
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{
    if ((const void*)&a != heroThing || (const void*)&b != localAddr || distance != 20.0f) { printf("FAIL distance arguments\n"); ++failures; }
    bool u = NextBit(distScript, distPos, false); Ev(u ? 'D' : 'd'); Ev((char)('a' + ExitOf(&b))); return u;
}

// ---- reference model: the generated-script form ---------------------------------------------------------------------------------
static char want[MAXEV + 1]; static int nwant; static int mt, md, mw, mexit, mexits;
static bool mSwitch, mFollow;
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static bool MT() { bool t = NextBit(termScript, mt, true); W(t ? 'T' : 't'); return t; }
static void MN() { W('N'); char c = worldScript[mw]; if (c) ++mw; if (c == 'S') mSwitch = true; else if (c == 's') mSwitch = false; else if (c == 'F') mFollow = true; else if (c == 'f') mFollow = false; }
static bool MD() { W('H'); bool u = NextBit(distScript, md, false); W(u ? 'D' : 'd'); W((char)('a' + mexit)); return u; }
static void MG() { int i = mexits < MAXEXIT - 1 ? mexits++ : MAXEXIT - 1; W('G'); W((char)('a' + i)); mexit = i; }
static void Model()
{
    MG();
    for (;;) {
        if (MT()) break;
        bool out = false;
        while (!mFollow) {
            MN(); if (MT()) { out = true; break; }
            if (MD()) {
                if (MT()) { out = true; break; }
                mFollow = true;
                while (!mSwitch) { MN(); if (MT()) { out = true; break; } } if (out) break;
                if (MT()) { out = true; break; }
                mFollow = false;
                while (mSwitch) { MN(); if (MT()) { out = true; break; } } if (out) break;
                if (MT()) { out = true; break; }
                MN(); if (MT()) { out = true; break; }
                MG(); W('A'); W((char)('a' + mexit)); W('x');
            }
        }
        if (out) break;
        if (MT()) break;
        MN();
    }
    W('X');
}

static void Run(const char* term, const char* dist, const char* world, bool follow0, bool switch0, int k)
{
    termScript = term; distScript = dist; worldScript = world; termPos = distPos = worldPos = 0; ngot = 0;
    mt = md = mw = 0; nwant = 0; mexits = 0; mSwitch = switch0; mFollow = follow0; Model();
    memset(info, 0, sizeof info); memset(deleteFuncCalls, 0, sizeof deleteFuncCalls);
    nexits = 0; operatorDeletes = baseDtors = 0; badGame = false; liveStrings = 0; localAddr = 0; tempAddr = 0;
    scriptObj.b[FOLLOW] = follow0; scriptObj.b[SWITCHING] = switch0;
    ((NScript::CQ_HangingTreeEvilScript*)(void*)scriptObj.b)->CheckBanditFollowStatus();
    got[ngot] = 0; want[nwant] = 0;
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); }
    check(scriptObj.b[FOLLOW] == (mFollow ? 1 : 0), "ShouldFollowHero left as the model leaves it", k);
    check(scriptObj.b[SWITCHING] == (mSwitch ? 1 : 0), "SwitchingRegions untouched by the row", k);
    check(!badGame, "Game receiver", k);
    check(baseDtors == mexits, "the local and every re-fetched temporary destroyed exactly once", k);
    check(nexits == mexits, "exit point fetched as often as the model", k);
    for (int i = 0; i < nexits; ++i) {
        check(info[i].RefCount == 0, "every fetched exit point released", k);
        check(deleteFuncCalls[i] == 1, "every exit point deleted once", k);
    }
    check(operatorDeletes == nexits, "one count block freed per exit point", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
}

int main()
{
    memset(scriptObj.b, 0, sizeof scriptObj.b);
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThingWithScriptName;
    void* g = gameObj; memcpy(scriptObj.b + 0x40, &g, 4);
    int k = 0;
    // every terminate return, in order (sites 1..10), plus the two entry states of ShouldFollowHero
    Run("1", "", "", false, false, ++k);                         // terminating at once
    Run("01", "", "", false, false, ++k);                        // after the first frame of the follow wait
    Run("001", "1", "", false, false, ++k);                      // hero near, terminating before following
    Run("0001", "1", "", false, false, ++k);                     // inside the switching-regions wait
    Run("0001", "1", ".S", false, false, ++k);                   // region switch starts after one frame, terminating after the wait
    Run("00001", "1", ".S", false, false, ++k);                  // ... after the wait
    Run("00001", "1", ".S", false, true, ++k);                   // switching already: no wait frame
    Run("000001", "1", ".S..", false, false, ++k);               // inside the end-of-switch wait
    Run("0000001", "1", ".Ss", false, false, ++k);               // switch ends, terminating after that wait
    Run("00000001", "1", ".Ss.", false, false, ++k);             // the extra frame, terminating
    Run("000000001", "1", ".Ss..", false, false, ++k);           // exit point fetched again, then terminating at the loop-bottom test
    Run("00001", "", "", true, false, ++k);                      // already following: the after-wait test
    Run("0001", "", "", true, false, ++k);                       // already following: loop bottom frame then terminating
    Run("0000000001", "0001", "", false, false, ++k);            // three far frames, then near
    Run("000000000000001", "1001", ".Ss.....F", false, false, ++k);   // a full follow cycle, then the flag set from outside
    unsigned long seed = 11;
    for (int r = 0; r < 3000; ++r) {
        static char ts[80], ds[80], ws[80];
        for (int i = 0; i < 79; ++i) {
            seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 12 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; ds[i] = ((seed >> 16) % 3 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; { unsigned v = (seed >> 16) % 10; ws[i] = v == 0 ? 'S' : v == 1 ? 's' : v == 2 ? 'F' : v == 3 ? 'f' : '.'; }
        }
        ts[79] = ds[79] = ws[79] = 0;
        Run(ts, ds, ws, (r % 5) == 0, (r % 7) == 0, 1000 + r);
    }
    if (failures) { printf("00d6c830_TEST FAIL %d\n", failures); return 1; }
    printf("00d6c830_TEST PASS (%d targeted, 3000 random)\n", k);
    return 0;
}