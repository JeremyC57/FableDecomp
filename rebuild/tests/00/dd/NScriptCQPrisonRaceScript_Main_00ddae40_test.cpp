// Behaviour test for 0x00DDAE40 NScript::CQ_PrisonRaceScript::Main (links the REAL row object: the row body, the expanded
// CEntityScriptBinding / CSpawnedFunc constructors, the expanded and the called ~CScriptThing, the binding's slot-0
// GetEntityScriptName and the spawned function's slot-4 SuspendableProcess all run as compiled).
// Fixtures (native layout, every offset read by an instruction of the row): the script object (0x60 bytes) with Game +0x40 and
// MasterScriptData +0x44 (PrisonRaceWonByHero +0xEC); Game's vtable slots 7 NewScriptFrame (+0x1C), 12 IsRegionLoaded (+0x30),
// 64 PostAddScriptedEntities (+0x100), 70 GetHero (+0x118), 72 GetThingWithScriptName (+0x120, hidden return pointer), 473
// EntityTeleportToPosition (+0x764). Fake interface methods are __fastcall with an unused edx (thiscall-compatible). The marker
// CScriptThing returned by slot 72 is raw storage {vptr, PImp.Data, PImp.Info} over a CCPPointerInfo {RefCount, DeleteFunc
// (__fastcall), Data}; PImp.Data is a fake thing whose slots 6 GetPos (+0x18) and 10 GetAngleXY (+0x28) the row expands. Every
// other virtual reached only through the vtables the row stores is aliased to one trap. operator new can be scripted to fail.
// Reference model: Model() below, written from the quest script's statements; every run is compared event by event.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" __declspec(dllimport) void* __stdcall SetUnhandledExceptionFilter(void*);
extern "C" __declspec(dllimport) void __stdcall ExitProcess(unsigned int);
static long __stdcall Crash(void*) { printf("FAIL crash\n"); ExitProcess(3); return 0; }
extern "C" void __cdecl VirtualTrap() { printf("FAIL unexpected virtual / trap call\n"); exit(3); }
#pragma comment(linker, "/alternatename:??_GCScriptThing@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?DecrementScriptCounter@CScriptThing@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?GetActivationTriggerStatus@CScriptThing@@UBE_NXZ=_VirtualTrap")
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
#pragma comment(linker, "/alternatename:?CreateActiveEntityScript@?$CEntityScriptBinding@VCQ_PrisonRaceScript@NScript@@@@MAE?AV?$CCountedPointer@VCActiveEntityScriptBase@@@@ABVCScriptThing@@_K@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:??1CSuspendableProcessMicrothread@@MAE@XZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?OnInterrupted@CSpawnedFuncBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?OnPredicateFail@CSpawnedFuncBase@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Restart@CSuspendableProcessMicrothread@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?SuspendSuspendableProcess@CSuspendableProcessMicrothread@@UAE_NXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:?RunSuspendableProcess@CSuspendableProcessMicrothread@@UAEXXZ=_VirtualTrap")
#pragma comment(linker, "/alternatename:??_G?$CSpawnedFunc@VCQ_PrisonRaceScript@NScript@@@@UAEPAXI@Z=_VirtualTrap")
#pragma comment(linker, "/alternatename:?Alloc@CPrisonRaceGuard@CQ_PrisonRaceScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=@AllocStub0@16")
#pragma comment(linker, "/alternatename:?Alloc@CPrisonRaceWinningPostGuard@CQ_PrisonRaceScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=@AllocStub1@16")
#pragma comment(linker, "/alternatename:?Alloc@CPrisonRaceWarden@CQ_PrisonRaceScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=@AllocStub2@16")
#pragma comment(linker, "/alternatename:?Alloc@CPrisonRacer@CQ_PrisonRaceScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=@AllocStub3@16")
#pragma comment(linker, "/alternatename:?Alloc@CPrisonRaceStartingGuard@CQ_PrisonRaceScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=@AllocStub4@16")
#pragma comment(linker, "/alternatename:?Alloc@CWaypointSpeedCheck@CQ_PrisonRaceScript@NScript@@SI?AV?$CCountedPointer@VCEntityScriptBase@@@@AAV23@PAVCScriptDataBase@@ABVCScriptThing@@@Z=@AllocStub5@16")
#pragma comment(linker, "/alternatename:?NullCoord@CScriptThing@@1VC3DVector@@A=_g_NullCoord")

// ---- the row's external C++ callees (declared with the signatures the row object names) ----------------------------------------
class CCharString { public: CCharString(const char* text, long length); CCharString(const CCharString& o); ~CCharString(); const char* Text; };
class CBaseClass { public: virtual ~CBaseClass(); };
class CEntityScriptBindingBase; class CSpawnedFuncBase;
class CScriptBase { public:
    void AddEntityScriptBinding(CEntityScriptBindingBase*); void PostAddScriptedEntities(); bool IsActiveThreadTerminating();
    void AddSpawnedFunction(CSpawnedFuncBase*, const CCharString&); };
class CSpawnedFuncBase { public: CSpawnedFuncBase(const CCharString& name, unsigned long id); };
namespace NScript { class CQ_PrisonRaceScript { public: void WatchForHeroWinOrLose(); }; }
extern "C" void __fastcall CallMain(void* self);   // the row (virtual, thiscall: this in ecx)
#pragma comment(linker, "/alternatename:@CallMain@4=?Main@CQ_PrisonRaceScript@NScript@@UAEXXZ")

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }
extern "C" float g_NullCoord[3] = { 0.0f, 0.0f, 0.0f };

// ---- event log -----------------------------------------------------------------------------------------------------------------
enum { MAXEV = 4000 };
static char got[MAXEV + 1]; static int ngot;
static void Ev(char c) { if (ngot < MAXEV) got[ngot++] = c; }
static const char* termScript; static int termPos;   // '1' = terminating; past the end: terminating (bounds every run)
static const char* loadScript; static int loadPos;   // '1' = region loaded; past the end: loaded
static const char* newScript;  static int newPos;    // '0' = operator new fails; past the end: succeeds
static bool NextBit(const char* s, int& pos, bool dflt) { char c = s[pos]; if (!c) return dflt; ++pos; return c == '1'; }

static union { unsigned char b[0x60]; double align; } scriptObj;
static union { unsigned char b[0x100]; double align; } master;

// strings: the text pointer is the whole object (retail CCharString is one pointer)
static int liveStrings;
CCharString::CCharString(const char* text, long length) { Text = text; ++liveStrings; if (length != -1) { ++failures; printf("FAIL string length\n"); } }
CCharString::CCharString(const CCharString& o) { Text = o.Text; ++liveStrings; Ev('c'); }
CCharString::~CCharString() { --liveStrings; Text = 0; }

// operator new / delete: a pool of objects
static union { unsigned char b[16][0x40]; double align; } pool; static int poolUsed; static unsigned newSizes[16];
void* __cdecl operator new(unsigned int n)
{
    bool ok = NextBit(newScript, newPos, true);
    if (poolUsed < 16) newSizes[poolUsed] = n;
    Ev(ok ? 'n' : '0');
    if (!ok || poolUsed >= 16 || n > 0x40) return 0;
    memset(pool.b[poolUsed], 0xCD, 0x40); return pool.b[poolUsed++];
}
static int operatorDeletes; static void* deletedPtr;
void __cdecl operator delete(void* p) { ++operatorDeletes; deletedPtr = p; }

// allocation functions (only stored by the row)
extern "C" {
void* __fastcall AllocStub0(void*, void*, void*, void*) { VirtualTrap(); return 0; }
void* __fastcall AllocStub1(void*, void*, void*, void*) { VirtualTrap(); return 0; }
void* __fastcall AllocStub2(void*, void*, void*, void*) { VirtualTrap(); return 0; }
void* __fastcall AllocStub3(void*, void*, void*, void*) { VirtualTrap(); return 0; }
void* __fastcall AllocStub4(void*, void*, void*, void*) { VirtualTrap(); return 0; }
void* __fastcall AllocStub5(void*, void*, void*, void*) { VirtualTrap(); return 0; }
}
static void* allocs[6] = { (void*)AllocStub0, (void*)AllocStub1, (void*)AllocStub2, (void*)AllocStub3, (void*)AllocStub4, (void*)AllocStub5 };
static const char* bindingNames[6] = { "PrisonRaceGuard", "PrisonRaceWinningPostGuard", "PrisonRaceWarden", "PrisonRacer", "PrisonRaceStartingGuard", "WaypointSpeedCheck" };

// script base members
static int bindingIndex; static unsigned char* bindings[6];
void CScriptBase::AddEntityScriptBinding(CEntityScriptBindingBase* b)
{
    if ((void*)this != scriptObj.b) { ++failures; printf("FAIL binding receiver\n"); }
    int i = bindingIndex++;
    if (i < 6) bindings[i] = (unsigned char*)b;
    Ev(b ? 'B' : 'b');
}
void CScriptBase::PostAddScriptedEntities() { if ((void*)this != scriptObj.b) { ++failures; printf("FAIL post receiver\n"); } Ev('P'); }
bool CScriptBase::IsActiveThreadTerminating()
{
    if ((void*)this != scriptObj.b) { ++failures; printf("FAIL terminate receiver\n"); }
    bool t = NextBit(termScript, termPos, true); Ev(t ? 'T' : 't'); return t;
}
static unsigned char* spawnedObj; static const char* spawnedGroup; static int spawnedLiveAtAdd;
void CScriptBase::AddSpawnedFunction(CSpawnedFuncBase* f, const CCharString& group)
{
    if ((void*)this != scriptObj.b) { ++failures; printf("FAIL spawn receiver\n"); }
    spawnedObj = (unsigned char*)f; spawnedGroup = group.Text; spawnedLiveAtAdd = liveStrings; Ev(f ? 'S' : 's');
}
static const char* spawnName; static unsigned long spawnId; static void* spawnThis;
CSpawnedFuncBase::CSpawnedFuncBase(const CCharString& name, unsigned long id) { spawnName = name.Text; spawnId = id; spawnThis = this; Ev('F'); }
static int watchCalls; static void* watchThis;
void NScript::CQ_PrisonRaceScript::WatchForHeroWinOrLose() { ++watchCalls; watchThis = this; }

// the marker thing and its counted pointer
struct InfoBlock { unsigned long RefCount; void (__fastcall* DeleteFunc)(void*); void* Data; };
static InfoBlock info; static int deleteFuncCalls, baseDtors; static void* deletedData; static void* markerAddr;
static void __fastcall DeleteMarkerData(void* data) { ++deleteFuncCalls; deletedData = data; }
CBaseClass::~CBaseClass() { ++baseDtors; if ((void*)this != markerAddr) { ++failures; printf("FAIL ~CBaseClass receiver\n"); } Ev('X'); }
static float markerPos[3] = { 11.5f, -3.25f, 7.0f }; static float markerAngle = 1.375f;
static void* thingVtbl[16]; static void* thingObj[1]; static bool nullMarker;
static const float* __fastcall T_GetPos(void* self, void*) { if (self != (void*)thingObj) { ++failures; printf("FAIL GetPos receiver\n"); } Ev('p'); return markerPos; }
static float __fastcall T_GetAngleXY(void* self, void*) { if (self != (void*)thingObj) { ++failures; printf("FAIL GetAngleXY receiver\n"); } Ev('a'); return markerAngle; }

// game interface
static void* gameVtbl[700]; static void* gameObj[1]; static bool badGame;
static void CheckGame(void* self) { if (self != (void*)gameObj) badGame = true; }
static void __fastcall G_NewScriptFrame(void* self, void*) { CheckGame(self); Ev('N'); }
static bool __fastcall G_IsRegionLoaded(void* self, void*, const CCharString* name)
{
    CheckGame(self); if (!name->Text || strcmp(name->Text, "PrisonCourtyard")) { ++failures; printf("FAIL region name\n"); }
    bool l = NextBit(loadScript, loadPos, true); Ev(l ? 'L' : 'l'); return l;
}
static void __fastcall G_PostAdd(void* self, void*) { CheckGame(self); Ev('Q'); }
static unsigned char hero[12];
static void* __fastcall G_GetHero(void* self, void*) { CheckGame(self); Ev('H'); return hero; }
static void* __fastcall G_GetThingWithScriptName(void* self, void*, void* ret, const CCharString* name)
{
    CheckGame(self); if (!name->Text || strcmp(name->Text, "PrisonRaceStartHSP")) { ++failures; printf("FAIL marker name\n"); }
    static void* fakeVtbl[1];
    void* v = fakeVtbl; memcpy(ret, &v, 4);
    void* d = nullMarker ? 0 : (void*)thingObj; memcpy((char*)ret + 4, &d, 4); InfoBlock* ib = &info; memcpy((char*)ret + 8, &ib, 4);
    markerAddr = ret; Ev('G'); return ret;
}
static const void* telThing; static float telPos[3]; static float telAngle; static bool telA, telB; static int telStrings;
static void __fastcall G_Teleport(void* self, void*, const void* thing, const float* pos, float angle, bool a, bool b)
{
    CheckGame(self); telThing = thing; memcpy(telPos, pos, 12); telAngle = angle; telA = a; telB = b; telStrings = liveStrings; Ev('E');
}

// ---- reference model -----------------------------------------------------------------------------------------------------------
static char want[MAXEV + 1]; static int nwant; static int mt, ml, mn;
static void W(char c) { if (nwant < MAXEV) want[nwant++] = c; }
static bool MT() { bool t = NextBit(termScript, mt, true); W(t ? 'T' : 't'); return t; }
static bool ML() { bool l = NextBit(loadScript, ml, true); W(l ? 'L' : 'l'); return l; }
static bool MN() { bool o = NextBit(newScript, mn, true); W(o ? 'n' : '0'); return o; }
static void Model()
{
    for (int i = 0; i < 6; ++i) { if (MN()) { W('c'); W('B'); } else W('b'); }
    W('P'); W('Q'); W('G');
    if (!nullMarker) { W('a'); W('p'); }
    W('H'); W('E');
    bool term = false;
    while (!ML()) { W('N'); if (MT()) { term = true; break; } }
    if (!term && !MT()) {
        if (MN()) { W('F'); W('S'); } else W('s');
    }
    W('X');
}

static void Run(const char* term, const char* load, const char* nw, bool nullM, unsigned long refs, int k)
{
    termScript = term; loadScript = load; newScript = nw; termPos = loadPos = newPos = 0; ngot = 0; nullMarker = nullM;
    mt = ml = mn = 0; nwant = 0; Model();
    info.RefCount = refs; info.DeleteFunc = DeleteMarkerData; info.Data = thingObj;
    deleteFuncCalls = operatorDeletes = baseDtors = 0; deletedPtr = deletedData = 0; badGame = false; liveStrings = 0;
    poolUsed = 0; bindingIndex = 0; memset(bindings, 0, sizeof bindings); spawnedObj = 0; spawnedGroup = 0; spawnName = 0; spawnId = 99;
    spawnThis = 0; watchCalls = 0; watchThis = 0; telThing = 0; telAngle = -99.0f; memset(telPos, 0, 12); telA = telB = true;
    memset(scriptObj.b, 0x5A, sizeof scriptObj.b);
    void* g = gameObj; memcpy(scriptObj.b + 0x40, &g, 4); void* m = master.b; memcpy(scriptObj.b + 0x44, &m, 4);
    memset(master.b, 0x77, sizeof master.b); master.b[0xEC] = 1;
    CallMain(scriptObj.b);
    got[ngot] = 0; want[nwant] = 0;
    if (strcmp(got, want)) { ++failures; if (failures < 40) printf("FAIL case %d events\n  got  %s\n  want %s\n", k, got, want); return; }
    check(!badGame, "Game receiver", k);
    check(master.b[0xEC] == 0 && master.b[0xEB] == 0x77 && master.b[0xED] == 0x77, "PrisonRaceWonByHero cleared, neighbours kept", k);
    for (int i = 0; i < 0x60; ++i) if (i < 0x40 || i >= 0x48) check(scriptObj.b[i] == 0x5A, "script object untouched", k);
    // teleport: hero, marker position / angle (NullCoord / 0.0f for an empty marker), false, false; no string alive across it
    check(telThing == hero && !telA && !telB, "teleport thing and flags", k);
    if (nullMarker) check(telPos[0] == 0.0f && telPos[1] == 0.0f && telPos[2] == 0.0f && telAngle == 0.0f, "empty marker: NullCoord and 0", k);
    else check(!memcmp(telPos, markerPos, 12) && telAngle == markerAngle, "marker position and angle", k);
    int nb = 0; for (int i = 0; i < 6; ++i) nb += bindings[i] != 0;
    check(telStrings == nb, "temporaries destroyed before the teleport (the bindings keep their name copies)", k);
    // bindings: vtable, copied name, parent, factory, valid flag, flags; slot 0 returns the name member
    int pi = 0; void* vt0 = 0;
    for (int i = 0; i < 6; ++i) {
        unsigned char* b = bindings[i]; if (!b) continue;
        check(b == pool.b[pi] && newSizes[pi] == 0x1C, "binding storage is the new(0x1C) block", k); ++pi;
        void* vt; memcpy(&vt, b, 4); if (!vt0) vt0 = vt; check(vt == vt0 && vt != 0, "binding vtable", k);
        const char* nm; memcpy(&nm, b + 4, 4); check(nm && !strcmp(nm, bindingNames[i]), "binding name", k);
        void* par; memcpy(&par, b + 8, 4); check(par == scriptObj.b, "binding parent", k);
        void* fn; memcpy(&fn, b + 0x10, 4); check(fn == allocs[i], "binding factory", k);
        check(b[0x14] == 1, "binding alloc valid", k);
        unsigned long fl; memcpy(&fl, b + 0x18, 4); check(fl == 0, "binding flags", k);
        typedef void* (__fastcall* NameFn)(void*, void*); NameFn f = (NameFn)((void**)vt)[0];
        check(f(b, 0) == b + 4, "slot 0 GetEntityScriptName", k);
    }
    // spawned function: CSpawnedFuncBase(name, 0) on the new(0x3C) block, Func / PClass, group "", slot 4 runs WatchForHeroWinOrLose
    if (spawnedObj) {
        check(spawnedObj == pool.b[pi] && newSizes[pi] == 0x3C && spawnThis == spawnedObj, "spawn storage is the new(0x3C) block", k);
        check(spawnName && !strcmp(spawnName, "WatchForHeroWinOrLose") && spawnId == 0, "spawn name and id", k);
        check(spawnedGroup && !strcmp(spawnedGroup, "") && spawnedLiveAtAdd == nb + 2, "spawn group, name still alive", k);
        void* pc; memcpy(&pc, spawnedObj + 0x38, 4); check(pc == scriptObj.b, "spawn PClass", k);
        void* vt; memcpy(&vt, spawnedObj, 4);
        spawnedObj[5] = 0;          // CASuspendableProcess::StopRunning +5, set by slot 4 after the call
        typedef void (__fastcall* ProcFn)(void*, void*); ProcFn f = (ProcFn)((void**)vt)[4];
        f(spawnedObj, 0);
        check(watchCalls == 1 && watchThis == scriptObj.b, "slot 4 calls WatchForHeroWinOrLose on the script", k);
        check(spawnedObj[5] == 1, "slot 4 sets StopRunning", k);
        void* fp; memcpy(&fp, spawnedObj + 0x34, 4); check(fp != 0, "spawn Func stored", k);
    }
    check(baseDtors == 1, "marker destroyed exactly once", k);
    check(info.RefCount == refs - 1, "marker reference released once", k);
    check(deleteFuncCalls == (refs == 1) && operatorDeletes == (refs == 1), "marker deleted only when the count reaches zero", k);
    if (refs == 1) check(deletedData == thingObj && deletedPtr == &info, "delete function gets Data, operator delete the count block", k);
    check(liveStrings == nb, "every temporary CCharString destroyed", k);
}

int main()
{
    SetUnhandledExceptionFilter((void*)Crash);
    thingObj[0] = thingVtbl; thingVtbl[6] = (void*)T_GetPos; thingVtbl[10] = (void*)T_GetAngleXY;
    gameObj[0] = gameVtbl;
    gameVtbl[7] = (void*)G_NewScriptFrame; gameVtbl[12] = (void*)G_IsRegionLoaded; gameVtbl[64] = (void*)G_PostAdd;
    gameVtbl[70] = (void*)G_GetHero; gameVtbl[72] = (void*)G_GetThingWithScriptName; gameVtbl[473] = (void*)G_Teleport;
    int k = 0;
    for (unsigned long refs = 1; refs <= 2; ++refs)
        for (int nm = 0; nm < 2; ++nm) {
            Run("0", "1", "", nm != 0, refs, ++k);                         // region loaded at once, thread spawned
            Run("1", "1", "", nm != 0, refs, ++k);                         // terminating after the wait
            Run("01", "0", "", nm != 0, refs, ++k);                        // one frame, terminating inside the wait
            Run("1", "0", "", nm != 0, refs, ++k);                         // terminating at the first frame
            Run("000", "0001", "", nm != 0, refs, ++k);                    // three frames, then spawned
            Run("0", "1", "0000000", nm != 0, refs, ++k);                  // every new fails
            Run("0", "1", "1010101", nm != 0, refs, ++k);                  // alternate failures
            Run("0", "1", "0101010", nm != 0, refs, ++k);
            Run("0", "1", "1111110", nm != 0, refs, ++k);                  // only the spawn fails
        }
    unsigned long seed = 11;
    for (int r = 0; r < 4000; ++r) {
        static char ts[64], ls[64], ns[16];
        for (int i = 0; i < 63; ++i) {
            seed = seed * 1103515245ul + 12345ul; ts[i] = ((seed >> 16) % 7 == 0) ? '1' : '0';
            seed = seed * 1103515245ul + 12345ul; ls[i] = ((seed >> 16) % 3 == 0) ? '1' : '0';
        }
        for (int i = 0; i < 7; ++i) { seed = seed * 1103515245ul + 12345ul; ns[i] = ((seed >> 16) % 4 == 0) ? '0' : '1'; }
        ts[63] = ls[63] = ns[7] = 0;
        Run(ts, ls, ns, (r % 5) == 0, 1 + (r & 1), 1000 + r);
    }
    if (failures) { printf("00ddae40_TEST FAIL %d\n", failures); return 1; }
    printf("00ddae40_TEST PASS (%d targeted, 4000 random)\n", k);
    return 0;
}