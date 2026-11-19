// Behaviour test for 0x00D8A240 NScript::CQ_HerosOldHouseScript::WatchForBooty (links the REAL row object, built with its
// /d2inlT57 flag: the row body, the expanded and the called ~CScriptThing copies and TRetVoid all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the script object's +0x40 Game pointer (0x00D8A257),
// whose vtable slots are 7 NewScriptFrame (+0x1C), 12 IsRegionLoaded (+0x30), 72 GetThingWithScriptName (+0x120, hidden return
// pointer), 296 SetQuestCardObjective (+0x4A0), 576 IsDiggingSpotEnabled (+0x900), 655 GetActiveQuestName (+0xA3C, hidden return
// pointer). The fake interface methods are __fastcall functions with an unused edx parameter (thiscall-compatible: this in ecx, the
// stack arguments popped by the callee). The booty CScriptThing returned by slot 72 is raw storage {vptr, PImp.Data, PImp.Info}
// over a CCPPointerInfo {RefCount, DeleteFunc (__fastcall), Data} so its release is observable. CScriptThing's other virtuals are
// referenced only through the vtable the inline destructor stores; they are aliased to one trap stub.
// Reference model: the FableWin form written independently below; every scripted run is compared event by event (frame,
// terminate tests, region / digging-spot queries, objective, booty release, CCharString balance).
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

// ---- the row's external C++ callees (declared with the signatures the row object names) ----------------------------------------
class CCharString { public: CCharString(const char* text, long length); ~CCharString(); const char* Text; };
class CBaseClass { public: virtual ~CBaseClass(); };
class CScriptBase { public: bool IsActiveThreadTerminating(); };
namespace NScript { class CQ_HerosOldHouseScript { public: void WatchForBooty(); }; }

inline void* operator new(unsigned int, void* p) { return p; }
static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

static int liveStrings, strCtors;
CCharString::CCharString(const char* text, long length) { Text = text; ++liveStrings; ++strCtors; if (length != -1) { printf("FAIL CCharString length %ld\n", length); ++failures; } }
CCharString::~CCharString() { --liveStrings; Text = 0; }

// ---- event log and scripted world ----------------------------------------------------------------------------------------------
enum { MAXEV = 4000 };
static char got[MAXEV + 1]; static int ngot;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static const char* termScript; static int termPos;   // '1' = terminating; past the end: terminating (bounds every run)
static const char* loadScript; static int loadPos;   // '1' = loaded; past the end: loaded
static const char* digScript;  static int digPos;    // '1' = digging spot still enabled; past the end: enabled
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

static union { unsigned char b[0x100]; double align; } scriptObj;
bool CScriptBase::IsActiveThreadTerminating()
{
    if ((void*)this != scriptObj.b) { printf("FAIL terminate receiver\n"); ++failures; }
    bool t = NextBit(termScript, termPos, true); Ev(t ? 'T' : 't'); return t;
}

struct InfoBlock { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
static InfoBlock info; static int deleteFuncCalls, operatorDeletes, baseDtors; static void* deletedPtr; static void* deletedData;
static unsigned char bootyData[16];
static void __fastcall DeleteBootyData(void* data) { ++deleteFuncCalls; deletedData = data; }
void __cdecl operator delete(void* p) { ++operatorDeletes; deletedPtr = p; }
static void* bootyAddr;
CBaseClass::~CBaseClass() { ++baseDtors; if ((void*)this != bootyAddr) { printf("FAIL ~CBaseClass receiver\n"); ++failures; } Ev('X'); }

// Game interface methods (thiscall-compatible __fastcall: ecx = this, edx unused).
static void* gameVtbl[700]; static void* gameObj[1]; static bool badGame;
static void CheckGame(void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev('N'); }
static bool __fastcall G_IsRegionLoaded(void* self, void*, const CCharString* name)
{
    CheckGame(self); if (!name->Text || strcmp(name->Text, "OakBay")) { printf("FAIL region name\n"); ++failures; }
    bool l = NextBit(loadScript, loadPos, true); Ev(l ? 'L' : 'l'); return l;
}
static void* __fastcall G_GetThingWithScriptName(void* self, void*, void* ret, const CCharString* name)
{
    CheckGame(self); if (!name->Text || strcmp(name->Text, "HiddenBooty")) { printf("FAIL booty name\n"); ++failures; }
    static void* fakeVtbl[1];
    memset(ret, 0, 12); void* v = fakeVtbl; memcpy(ret, &v, 4);
    void* d = bootyData; memcpy((char*)ret + 4, &d, 4); InfoBlock* ib = &info; memcpy((char*)ret + 8, &ib, 4);
    bootyAddr = ret; Ev('G'); return ret;
}
static bool __fastcall G_IsDiggingSpotEnabled(void* self, void*, const void* thing)
{
    CheckGame(self); if (thing != bootyAddr) { printf("FAIL digging spot argument\n"); ++failures; }
    bool e = NextBit(digScript, digPos, true); Ev(e ? 'D' : 'd'); return e;
}
static const char* questName = "Q_HerosOldHouse";
static void* __fastcall G_GetActiveQuestName(void* self, void*, void* ret)
{ CheckGame(self); new (ret) CCharString(questName, -1); Ev('A'); return ret; }
static const CCharString* objArgs[4];
static void __fastcall G_SetQuestCardObjective(void* self, void*, const CCharString* a, const CCharString* b, const CCharString* c, const CCharString* d)
{
    CheckGame(self); objArgs[0] = a; objArgs[1] = b; objArgs[2] = c; objArgs[3] = d;
    bool ok = a->Text && !strcmp(a->Text, questName) && b->Text && !strcmp(b->Text, "TEXT_QUEST_HEROS_OLD_HOUSE_OBJECTIVE_02") &&
              c->Text && !strcmp(c->Text, "") && d->Text && !strcmp(d->Text, "OakBay");
    Ev(ok ? 'S' : '!');
}

// ---- reference model: the FableWin form ----------------------------------------------------------------------------------------
static char want[MAXEV + 1]; static int nwant; static int mt, ml, md;
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static bool MT() { bool t = NextBit(termScript, mt, true); W(t ? 'T' : 't'); return t; }
static bool ML() { bool l = NextBit(loadScript, ml, true); W(l ? 'L' : 'l'); return l; }
static bool MD() { bool e = NextBit(digScript, md, true); W(e ? 'D' : 'd'); return e; }
static void Model()
{
    W('G');
    for (;;) {
        if (MT()) break;
        if (!ML()) {
            if (MT()) break;
            bool out = false;
            while (!ML()) { W('N'); if (MT()) { out = true; break; } }
            if (out) break;
            if (MT()) break;
        }
        if (!MD()) {
            if (MT()) break;
            W('A'); W('S'); break;
        }
        W('N');
    }
    W('X');
}

static void Run(const char* term, const char* load, const char* dig, unsigned long refs, int k)
{
    termScript = term; loadScript = load; digScript = dig; termPos = loadPos = digPos = 0; ngot = 0;
    mt = ml = md = 0; nwant = 0; Model();
    info.RefCount = refs; info.DeleteFunc = DeleteBootyData; info.Data = bootyData;
    deleteFuncCalls = operatorDeletes = baseDtors = 0; deletedPtr = deletedData = 0; badGame = false; liveStrings = 0; strCtors = 0;
    ((NScript::CQ_HerosOldHouseScript*)(void*)scriptObj.b)->WatchForBooty();
    got[ngot] = 0; want[nwant] = 0;
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); }
    check(!badGame, "Game receiver", k);
    check(baseDtors == 1, "booty destroyed exactly once", k);
    check(info.RefCount == refs - 1, "booty reference released once", k);
    check(deleteFuncCalls == (refs == 1) && operatorDeletes == (refs == 1), "booty deleted only when the count reaches zero", k);
    if (refs == 1) check(deletedData == bootyData && deletedPtr == &info, "delete function gets Data, operator delete the count block", k);
    check(liveStrings == 0, "every CCharString destroyed", k);
}

int main()
{
    memset(scriptObj.b, 0, sizeof scriptObj.b);
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[12] = (void*)G_IsRegionLoaded; gameVtbl[72] = (void*)G_GetThingWithScriptName;
    gameVtbl[296] = (void*)G_SetQuestCardObjective; gameVtbl[576] = (void*)G_IsDiggingSpotEnabled; gameVtbl[655] = (void*)G_GetActiveQuestName;
    void* g = gameObj; memcpy(scriptObj.b + 0x40, &g, 4);
    int k = 0;
    // Each return site, with both release paths (last reference / shared).
    for (unsigned long refs = 1; refs <= 2; ++refs) {
        Run("1", "", "", refs, ++k);                       // S1 before the first frame
        Run("01", "0", "", refs, ++k);                     // S2: region not loaded, terminating
        Run("001", "00", "", refs, ++k);                   // S3: inside the wait
        Run("0001", "000", "", refs, ++k);                 // S3 after one waited frame
        Run("0001", "001", "", refs, ++k);                 // S4: region loaded after one frame, then terminating
        Run("001", "01", "", refs, ++k);                   // S4 with no waited frame
        Run("01", "1", "0", refs, ++k);                    // S5: digging spot gone, terminating
        Run("00", "1", "0", refs, ++k);                    // S6: objective set
        Run("0001", "1", "1", refs, ++k);                  // S1 at the loop bottom after a full frame
        Run("000000", "1100", "110", refs, ++k);           // several frames, a wait, then the objective
    }
    // Random scripts.
    unsigned long seed = 7;
    for (int r = 0; r < 3000; ++r) {
        static char ts[64], ls[64], ds[64];
        for (int i = 0; i < 63; ++i) {
            seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 9 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; ls[i] = ((seed >> 16) % 3 == 0) ? '0' : '1';
            seed = seed * 1103515245ul + 12345ul; ds[i] = ((seed >> 16) % 6 == 0) ? '0' : '1';
        }
        ts[63] = ls[63] = ds[63] = 0;
        Run(ts, ls, ds, 1 + (r & 1), 1000 + r);
    }
    if (failures) { printf("00d8a240_TEST FAIL %d\n", failures); return 1; }
    printf("00d8a240_TEST PASS (%d targeted, 3000 random)\n", k);
    return 0;
}