// Behaviour test for 0x00E63730 NScript::CV_ChickenKickingScript::CSpectator::Init(): the spectator's number is 1 when it stands near
// the "Spectator1" marker (retail 0x012E7450), else 2 near "Spectator2" (0x012E7418), else 3 -- unless the script thread is terminating.
// NF420 (2026-11-25): LINKED test -- it links the row object (the row function is declared here, not defined; it is called through a
// __fastcall shim aliased to the row symbol). The game interface is a vtable fixture that logs the names it is given and the distance
// test is defined under the row's spelling, so the test asserts both literals; a mutated literal fails.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
extern "C" void __cdecl NF420TrapHit(const char* sym) { printf("FAIL unexpected call %s\n", sym); exit(3); }
extern "C" char NF420TrapData[4096] = { 0 };
// ---- script fixtures: CCharString / CBaseClass under the row's spellings; the game interface and every CScriptThing are raw objects
// whose vtables trap except the slots a test fills (thiscall-compatible __fastcall functions, unused edx); a CScriptThing is
// {vptr, PImp.Data (an id), PImp.Info (a shared count block that never reaches zero)}.
class CCharString { public: void* PStringData; CCharString(const char* text, long length); ~CCharString(); };
class CBaseClass { public: virtual ~CBaseClass(); };
static char g_log[512]; static int g_bad = 0, g_strings = 0;
static void Log(const char* tag, const void* s) { strcat(g_log, tag); strcat(g_log, "["); strcat(g_log, s ? (const char*)s : "(null)"); strcat(g_log, "]"); }
CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; if (length != -1) g_bad = 1; ++g_strings; }
CCharString::~CCharString() { --g_strings; }
CBaseClass::~CBaseClass() { }
struct NF420Info { long RefCount; void* DeleteFunc; void* Data; };
static NF420Info g_info = { 1000, 0, 0 };
static void* g_gameVtbl[918]; static void* g_thingVtbl[79]; static void* g_game[1] = { g_gameVtbl };
extern "C" void __cdecl NF420_VirtualTrap() { NF420TrapHit("game-interface / CScriptThing virtual"); }
static void NF420InitVtbls() { for (int i = 0; i < 918; ++i) g_gameVtbl[i] = (void*)NF420_VirtualTrap; for (int j = 0; j < 79; ++j) g_thingVtbl[j] = (void*)NF420_VirtualTrap; }
static void NF420MakeThing(void* p, long id) { void** t = (void**)p; t[0] = g_thingVtbl; t[1] = (void*)id; t[2] = &g_info; }
static long ThingId(const void* p) { return (long)((void* const*)p)[1]; }
// slot 72 GetThingWithScriptName(const CCharString&) const: a CScriptThing by hidden pointer, id 77, holding one reference
extern "C" void* __fastcall NF420_GetThingWithScriptName(void*, int, void* ret, const CCharString& name) { Log("thing", name.PStringData); NF420MakeThing(ret, 77); ++g_info.RefCount; return ret; }   // the returned thing owns one reference
// ---- end of script fixtures
static unsigned char g_obj[0x40];
class CScriptThing;
class CEntityScriptBase { public: bool IsActiveThreadTerminating() const; };
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);
static int g_near = 0; static bool g_terminating = false;
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance)
{ if ((void*)&a != g_obj + 8 || ThingId(&b) != 77 || distance != 1.0f) g_bad = 1; static int calls = 0; return ++calls == g_near; }
bool CEntityScriptBase::IsActiveThreadTerminating() const { if ((void*)this != g_obj) g_bad = 1; return g_terminating; }
extern "C" void __fastcall NF420_Row(void* self, int edx);
#pragma comment(linker, "/alternatename:@NF420_Row@8=?Init@CSpectator@CV_ChickenKickingScript@NScript@@EAEXXZ")   // the row function, called through the shim
// Row-object externals this test does not model (generated): a call reaches a trap that prints FAIL and exits; a data
// symbol reads a zeroed buffer.
extern "C" void __cdecl NF420Trap0() { NF420TrapHit("?DecrementScriptCounter@CScriptThing@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?DecrementScriptCounter@CScriptThing@@UAEXXZ=_NF420Trap0")
extern "C" void __cdecl NF420Trap1() { NF420TrapHit("?GetActivationTriggerStatus@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?GetActivationTriggerStatus@CScriptThing@@UBE_NXZ=_NF420Trap1")
extern "C" void __cdecl NF420Trap2() { NF420TrapHit("?GetAngleXY@CScriptThing@@UBEMXZ"); }
#pragma comment(linker, "/alternatename:?GetAngleXY@CScriptThing@@UBEMXZ=_NF420Trap2")
extern "C" void __cdecl NF420Trap3() { NF420TrapHit("?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ"); }
#pragma comment(linker, "/alternatename:?GetCurrentMapName@CScriptThing@@UBE?AVCCharString@@XZ=_NF420Trap3")
extern "C" void __cdecl NF420Trap4() { NF420TrapHit("?GetCurrentScriptPriority@CScriptThing@@UBE?AW4EScriptAIPriority@@XZ"); }
#pragma comment(linker, "/alternatename:?GetCurrentScriptPriority@CScriptThing@@UBE?AW4EScriptAIPriority@@XZ=_NF420Trap4")
extern "C" void __cdecl NF420Trap5() { NF420TrapHit("?GetCurrentStateGroupType@CScriptThing@@UBE?AW4EAIStateGroupType@@XZ"); }
#pragma comment(linker, "/alternatename:?GetCurrentStateGroupType@CScriptThing@@UBE?AW4EAIStateGroupType@@XZ=_NF420Trap5")
extern "C" void __cdecl NF420Trap6() { NF420TrapHit("?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ"); }
#pragma comment(linker, "/alternatename:?GetDataString@CScriptThing@@UBE?AVCCharString@@XZ=_NF420Trap6")
extern "C" void __cdecl NF420Trap7() { NF420TrapHit("?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ"); }
#pragma comment(linker, "/alternatename:?GetDefName@CScriptThing@@UBE?AVCCharString@@XZ=_NF420Trap7")
extern "C" void __cdecl NF420Trap8() { NF420TrapHit("?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ"); }
#pragma comment(linker, "/alternatename:?GetFocalPos@CScriptThing@@UBE?AVC3DVector@@XZ=_NF420Trap8")
extern "C" void __cdecl NF420Trap9() { NF420TrapHit("?GetHomeMapName@CScriptThing@@UBE?AVCCharString@@XZ"); }
#pragma comment(linker, "/alternatename:?GetHomeMapName@CScriptThing@@UBE?AVCCharString@@XZ=_NF420Trap9")
extern "C" void __cdecl NF420Trap10() { NF420TrapHit("?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ"); }
#pragma comment(linker, "/alternatename:?GetHomePos@CScriptThing@@UBE?AVC3DVector@@XZ=_NF420Trap10")
extern "C" void __cdecl NF420Trap11() { NF420TrapHit("?GetName@CScriptThing@@UBEABVCCharString@@XZ"); }
#pragma comment(linker, "/alternatename:?GetName@CScriptThing@@UBEABVCCharString@@XZ=_NF420Trap11")
extern "C" void __cdecl NF420Trap12() { NF420TrapHit("?GetPThing@CScriptThing@@UBEPAVCThing@@XZ"); }
#pragma comment(linker, "/alternatename:?GetPThing@CScriptThing@@UBEPAVCThing@@XZ=_NF420Trap12")
extern "C" void __cdecl NF420Trap13() { NF420TrapHit("?GetPThingUniqueID@CScriptThing@@UBE_KXZ"); }
#pragma comment(linker, "/alternatename:?GetPThingUniqueID@CScriptThing@@UBE_KXZ=_NF420Trap13")
extern "C" void __cdecl NF420Trap14() { NF420TrapHit("?GetPos@CScriptThing@@UBEABVC3DVector@@XZ"); }
#pragma comment(linker, "/alternatename:?GetPos@CScriptThing@@UBEABVC3DVector@@XZ=_NF420Trap14")
extern "C" void __cdecl NF420Trap15() { NF420TrapHit("?GetScriptCounter@CScriptThing@@UAEJXZ"); }
#pragma comment(linker, "/alternatename:?GetScriptCounter@CScriptThing@@UAEJXZ=_NF420Trap15")
extern "C" void __cdecl NF420Trap16() { NF420TrapHit("?IncrementScriptCounter@CScriptThing@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?IncrementScriptCounter@CScriptThing@@UAEXXZ=_NF420Trap16")
extern "C" void __cdecl NF420Trap17() { NF420TrapHit("?IsAlive@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsAlive@CScriptThing@@UBE_NXZ=_NF420Trap17")
extern "C" void __cdecl NF420Trap18() { NF420TrapHit("?IsAwareOfHero@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsAwareOfHero@CScriptThing@@UBE_NXZ=_NF420Trap18")
extern "C" void __cdecl NF420Trap19() { NF420TrapHit("?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?IsBeingCarriedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap19")
extern "C" void __cdecl NF420Trap20() { NF420TrapHit("?IsDead@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsDead@CScriptThing@@UBE_NXZ=_NF420Trap20")
extern "C" void __cdecl NF420Trap21() { NF420TrapHit("?IsEqualTo@CScriptThing@@MBE_NABV1@@Z"); }
#pragma comment(linker, "/alternatename:?IsEqualTo@CScriptThing@@MBE_NABV1@@Z=_NF420Trap21")
extern "C" void __cdecl NF420Trap22() { NF420TrapHit("?IsNull@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsNull@CScriptThing@@UBE_NXZ=_NF420Trap22")
extern "C" void __cdecl NF420Trap23() { NF420TrapHit("?IsOpenDoor@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsOpenDoor@CScriptThing@@UBE_NXZ=_NF420Trap23")
extern "C" void __cdecl NF420Trap24() { NF420TrapHit("?IsSneaking@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsSneaking@CScriptThing@@UBE_NXZ=_NF420Trap24")
extern "C" void __cdecl NF420Trap25() { NF420TrapHit("?IsSummonedCreature@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsSummonedCreature@CScriptThing@@UBE_NXZ=_NF420Trap25")
extern "C" void __cdecl NF420Trap26() { NF420TrapHit("?IsUnconscious@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsUnconscious@CScriptThing@@UBE_NXZ=_NF420Trap26")
extern "C" void __cdecl NF420Trap27() { NF420TrapHit("?IsUsable@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?IsUsable@CScriptThing@@UBE_NXZ=_NF420Trap27")
extern "C" void __cdecl NF420Trap28() { NF420TrapHit("?MsgAttackedWithBareHandsWithoutHittingAnything@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgAttackedWithBareHandsWithoutHittingAnything@CScriptThing@@UBE_NXZ=_NF420Trap28")
extern "C" void __cdecl NF420Trap29() { NF420TrapHit("?MsgAttackedWithMeleeWeaponWithoutHittingAnything@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgAttackedWithMeleeWeaponWithoutHittingAnything@CScriptThing@@UBE_NXZ=_NF420Trap29")
extern "C" void __cdecl NF420Trap30() { NF420TrapHit("?MsgExpressionPerformedTo@CScriptThing@@UBE_NAAVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgExpressionPerformedTo@CScriptThing@@UBE_NAAVCCharString@@@Z=_NF420Trap30")
extern "C" void __cdecl NF420Trap31() { NF420TrapHit("?MsgGetThingsKilled@CScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgGetThingsKilled@CScriptThing@@UBE_NAAV?$vector@KV?$allocator@K@std@@@std@@@Z=_NF420Trap31")
extern "C" void __cdecl NF420Trap32() { NF420TrapHit("?MsgHitEnemyWithBareHands@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithBareHands@CScriptThing@@UBE_NXZ=_NF420Trap32")
extern "C" void __cdecl NF420Trap33() { NF420TrapHit("?MsgHitEnemyWithMeleeWeapon@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithMeleeWeapon@CScriptThing@@UBE_NXZ=_NF420Trap33")
extern "C" void __cdecl NF420Trap34() { NF420TrapHit("?MsgHitEnemyWithRangedWeapon@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgHitEnemyWithRangedWeapon@CScriptThing@@UBE_NXZ=_NF420Trap34")
extern "C" void __cdecl NF420Trap35() { NF420TrapHit("?MsgHitFriendWithBareHands@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgHitFriendWithBareHands@CScriptThing@@UBE_NXZ=_NF420Trap35")
extern "C" void __cdecl NF420Trap36() { NF420TrapHit("?MsgHitFriendWithMeleeWeapon@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgHitFriendWithMeleeWeapon@CScriptThing@@UBE_NXZ=_NF420Trap36")
extern "C" void __cdecl NF420Trap37() { NF420TrapHit("?MsgHitFriendWithRangedWeapon@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgHitFriendWithRangedWeapon@CScriptThing@@UBE_NXZ=_NF420Trap37")
extern "C" void __cdecl NF420Trap38() { NF420TrapHit("?MsgHowLongWasExpressionPerformed@CScriptThing@@UBEJXZ"); }
#pragma comment(linker, "/alternatename:?MsgHowLongWasExpressionPerformed@CScriptThing@@UBEJXZ=_NF420Trap38")
extern "C" void __cdecl NF420Trap39() { NF420TrapHit("?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap39")
extern "C" void __cdecl NF420Trap40() { NF420TrapHit("?MsgIsHitByAnyAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap40")
extern "C" void __cdecl NF420Trap41() { NF420TrapHit("?MsgIsHitByAnyNonAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByAnyNonAggressiveSpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap41")
extern "C" void __cdecl NF420Trap42() { NF420TrapHit("?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByAnySpecialAbilityFrom@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap42")
extern "C" void __cdecl NF420Trap43() { NF420TrapHit("?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitBySpecialAbilityFrom@CScriptThing@@UBE_NW4EHeroAbility@@ABVCCharString@@@Z=_NF420Trap43")
extern "C" void __cdecl NF420Trap44() { NF420TrapHit("?MsgIsHitByWithDecapitate@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByWithDecapitate@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap44")
extern "C" void __cdecl NF420Trap45() { NF420TrapHit("?MsgIsHitByWithFlourish@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByWithFlourish@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap45")
extern "C" void __cdecl NF420Trap46() { NF420TrapHit("?MsgIsHitByWithProjectileWeapon@CScriptThing@@UBE_NABVCCharString@@AAM@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByWithProjectileWeapon@CScriptThing@@UBE_NABVCCharString@@AAM@Z=_NF420Trap46")
extern "C" void __cdecl NF420Trap47() { NF420TrapHit("?MsgIsHitByWithWeapon@CScriptThing@@UBE_NABVCCharString@@0@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsHitByWithWeapon@CScriptThing@@UBE_NABVCCharString@@0@Z=_NF420Trap47")
extern "C" void __cdecl NF420Trap48() { NF420TrapHit("?MsgIsKicked@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgIsKicked@CScriptThing@@UBE_NXZ=_NF420Trap48")
extern "C" void __cdecl NF420Trap49() { NF420TrapHit("?MsgIsKilledBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsKilledBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap49")
extern "C" void __cdecl NF420Trap50() { NF420TrapHit("?MsgIsKnockedOutBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsKnockedOutBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap50")
extern "C" void __cdecl NF420Trap51() { NF420TrapHit("?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsPresentedWithItem@CScriptThing@@UBE_NAAVCCharString@@@Z=_NF420Trap51")
extern "C" void __cdecl NF420Trap52() { NF420TrapHit("?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsTalkedToBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap52")
extern "C" void __cdecl NF420Trap53() { NF420TrapHit("?MsgIsTriggeredBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsTriggeredBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap53")
extern "C" void __cdecl NF420Trap54() { NF420TrapHit("?MsgIsUsedBy@CScriptThing@@UBE_NABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgIsUsedBy@CScriptThing@@UBE_NABVCCharString@@@Z=_NF420Trap54")
extern "C" void __cdecl NF420Trap55() { NF420TrapHit("?MsgOnCutSceneAnimEvent@CScriptThing@@UBE_NAAVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgOnCutSceneAnimEvent@CScriptThing@@UBE_NAAVCCharString@@@Z=_NF420Trap55")
extern "C" void __cdecl NF420Trap56() { NF420TrapHit("?MsgOnMoralityChanged@CScriptThing@@UBE_NAAJ@Z"); }
#pragma comment(linker, "/alternatename:?MsgOnMoralityChanged@CScriptThing@@UBE_NAAJ@Z=_NF420Trap56")
extern "C" void __cdecl NF420Trap57() { NF420TrapHit("?MsgOpenedChest@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgOpenedChest@CScriptThing@@UBE_NXZ=_NF420Trap57")
extern "C" void __cdecl NF420Trap58() { NF420TrapHit("?MsgPerformedAnyAggressiveSpecialAbility@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgPerformedAnyAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_NF420Trap58")
extern "C" void __cdecl NF420Trap59() { NF420TrapHit("?MsgPerformedAnyNonAggressiveSpecialAbility@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgPerformedAnyNonAggressiveSpecialAbility@CScriptThing@@UBE_NXZ=_NF420Trap59")
extern "C" void __cdecl NF420Trap60() { NF420TrapHit("?MsgPerformedAnySpecialAbility@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgPerformedAnySpecialAbility@CScriptThing@@UBE_NXZ=_NF420Trap60")
extern "C" void __cdecl NF420Trap61() { NF420TrapHit("?MsgPerformedFlourish@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgPerformedFlourish@CScriptThing@@UBE_NXZ=_NF420Trap61")
extern "C" void __cdecl NF420Trap62() { NF420TrapHit("?MsgPerformedSpecialAbility@CScriptThing@@UBE_NW4EHeroAbility@@@Z"); }
#pragma comment(linker, "/alternatename:?MsgPerformedSpecialAbility@CScriptThing@@UBE_NW4EHeroAbility@@@Z=_NF420Trap62")
extern "C" void __cdecl NF420Trap63() { NF420TrapHit("?MsgPerformedSuccessfulFlourish@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgPerformedSuccessfulFlourish@CScriptThing@@UBE_NXZ=_NF420Trap63")
extern "C" void __cdecl NF420Trap64() { NF420TrapHit("?MsgReceivedInventoryItem@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgReceivedInventoryItem@CScriptThing@@UBE_NXZ=_NF420Trap64")
extern "C" void __cdecl NF420Trap65() { NF420TrapHit("?MsgReceivedMoney@CScriptThing@@UBE_NAAJ@Z"); }
#pragma comment(linker, "/alternatename:?MsgReceivedMoney@CScriptThing@@UBE_NAAJ@Z=_NF420Trap65")
extern "C" void __cdecl NF420Trap66() { NF420TrapHit("?MsgTalkedToAnyone@CScriptThing@@UBE_NXZ"); }
#pragma comment(linker, "/alternatename:?MsgTalkedToAnyone@CScriptThing@@UBE_NXZ=_NF420Trap66")
extern "C" void __cdecl NF420Trap67() { NF420TrapHit("?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ"); }
#pragma comment(linker, "/alternatename:?MsgWhoExpressedToMe@CScriptThing@@UBE?AV1@XZ=_NF420Trap67")
extern "C" void __cdecl NF420Trap68() { NF420TrapHit("?MsgWhoHitMe@CScriptThing@@UBE?AV1@XZ"); }
#pragma comment(linker, "/alternatename:?MsgWhoHitMe@CScriptThing@@UBE?AV1@XZ=_NF420Trap68")
extern "C" void __cdecl NF420Trap69() { NF420TrapHit("?MsgWhoKilledMe@CScriptThing@@UBE?AV1@XZ"); }
#pragma comment(linker, "/alternatename:?MsgWhoKilledMe@CScriptThing@@UBE?AV1@XZ=_NF420Trap69")
extern "C" void __cdecl NF420Trap70() { NF420TrapHit("?MsgWhoKnockedMeOut@CScriptThing@@UBE?AV1@XZ"); }
#pragma comment(linker, "/alternatename:?MsgWhoKnockedMeOut@CScriptThing@@UBE?AV1@XZ=_NF420Trap70")
extern "C" void __cdecl NF420Trap71() { NF420TrapHit("?MsgWhoTalkedToMe@CScriptThing@@UBE?AV1@XZ"); }
#pragma comment(linker, "/alternatename:?MsgWhoTalkedToMe@CScriptThing@@UBE?AV1@XZ=_NF420Trap71")
extern "C" void __cdecl NF420Trap72() { NF420TrapHit("?SetActivationTriggerStatus@CScriptThing@@UAEX_N@Z"); }
#pragma comment(linker, "/alternatename:?SetActivationTriggerStatus@CScriptThing@@UAEX_N@Z=_NF420Trap72")
extern "C" void __cdecl NF420Trap73() { NF420TrapHit("?SetAsUsable@CScriptThing@@UAEX_N@Z"); }
#pragma comment(linker, "/alternatename:?SetAsUsable@CScriptThing@@UAEX_N@Z=_NF420Trap73")
extern "C" void __cdecl NF420Trap74() { NF420TrapHit("?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z"); }
#pragma comment(linker, "/alternatename:?SetDataString@CScriptThing@@UAEXABVCCharString@@@Z=_NF420Trap74")
extern "C" void __cdecl NF420Trap75() { NF420TrapHit("?SetFriendsWithEverythingFlag@CScriptThing@@UAEX_N@Z"); }
#pragma comment(linker, "/alternatename:?SetFriendsWithEverythingFlag@CScriptThing@@UAEX_N@Z=_NF420Trap75")
extern "C" void __cdecl NF420Trap76() { NF420TrapHit("?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z"); }
#pragma comment(linker, "/alternatename:?SetToKillOnLevelUnload@CScriptThing@@UAEX_N@Z=_NF420Trap76")
extern "C" void __cdecl NF420Trap77() { NF420TrapHit("?UpdateThingAttachment@CScriptThing@@UAEXXZ"); }
#pragma comment(linker, "/alternatename:?UpdateThingAttachment@CScriptThing@@UAEXXZ=_NF420Trap77")
int main()
{
    NF420InitVtbls(); g_gameVtbl[72] = (void*)NF420_GetThingWithScriptName;
    *(void**)(g_obj + 4) = g_game; NF420MakeThing(g_obj + 8, 1);
    g_near = 2;                                                       // not near Spectator1, near Spectator2
    NF420_Row(g_obj, 0);
    bool ok = !g_bad && strcmp(g_log, "thing[Spectator1]thing[Spectator2]") == 0 && *(long*)(g_obj + 0x1c) == 2 && g_strings == 0 && g_info.RefCount == 1000;
    if (ok) printf("00e63730_TEST PASS\n");
    else printf("FAIL bad=%d log=%s number=%ld strings=%d refs=%ld\n", g_bad, g_log, *(long*)(g_obj + 0x1c), g_strings, g_info.RefCount);
    return ok ? 0 : 1;
}
