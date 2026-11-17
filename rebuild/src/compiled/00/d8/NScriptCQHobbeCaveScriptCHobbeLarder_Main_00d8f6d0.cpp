// NScript::CQ_HobbeCaveScript::CHobbeLarder::Main() -- Hobbe cave larder hobbe: takes scripting control, then until the cave leader has grunted orders re-arms a script timer from the script def whenever it notices the hero; then attacks the hero, releases control and, while BanditSacrifice lives, flags HobbesCloseby once it comes within 10 units of it
// Leaf: ?Main@CHobbeLarder@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D8F6D0, 640 bytes (+ int3 padding).
// Callees: CIsThingAlive predicate (vtable 0x012C32F0, CScriptThing copy 0x004ABE90 / dtor 0x004AA840), SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, the scripted-thing handle (CBaseObject ctor 0x0099A380, IsValid 0x00CD23B9,
//   Release 0x00CD2770, dtor 0x007E74D0, vtable 0x0127094C), CTimer (inline ctor RegisterTimer 87; dtor DeregisterTimer 88,
//   out-of-line copy 0x00CD4470) with SetTimer 89 / GetTimer 90 on pDSTGame 0x0143E8F8, the script def
//   0x0143E90C (+0xCC8), IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0; CGameScriptInterfaceBase
//   slots 7 NewScriptFrame, 8 StartScriptingEntity, 70 GetHero, 72 GetThingWithScriptName, 451 GiveThingBestEnemyTarget;
//   CScriptThing slot 16 IsAwareOfHero (+0x40), the impl's slot 75 IsAlive (+0x12c). String "BanditSacrifice" 0x012D5788.
// Layout: entity-script model of the landed CDrinkingHobbe::Main 0x00D9BF60 (its GameCalls interface copy replaced by the shared header, NF78), CScriptThing
//   IsAlive as the landed CBRIAR_ROSE_IMPOSTER::Main 0x00D81820. CHobbeLarder (Ego_r.pdb, 28 bytes): ParentClass +0x14,
//   PMasterData +0x18; CQ_HobbeCaveScript KingGruntedOrders PDB +0x74 / retail +0x60, HobbesCloseby PDB +0x7f / retail +0x6b.
// Shape: generated script code (`if (IsActiveThreadTerminating()) return;` after every statement); CTimer has inline ctor and
//   dtor (VC7.1 expands the dtor at the two early returns inside the first loop and calls the out-of-line copy elsewhere).
#include "fable_compiler.h"
class CCharString;
struct CScriptThingImpl;
class CScriptThing { public:                                   // 12 bytes, vtable 0x01238C8C {vptr, PImp_Data, PImp_Info}
 virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
 virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
 virtual void v13(); virtual void v14(); virtual void v15();
 virtual bool IsAwareOfHero() const;              // slot 16 (+0x40) 0x004AAAC0
 virtual void v17(); virtual void v18();
 virtual void v19(); virtual void v20();
 virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
 CScriptThingImpl* PImp_Data;
 void* PImp_Info;
 CScriptThing(const CScriptThing& other);   // 0x004ABE90
 virtual ~CScriptThing();                           // 0x004AA840
 bool IsAlive() const;                      // slot 75, inlined below
};
struct CScriptThingImpl {
 virtual void s00();
 virtual void s01();
 virtual void s02();
 virtual void s03();
 virtual void s04();
 virtual void s05();
 virtual void s06();
 virtual void s07();
 virtual void s08();
 virtual void s09();
 virtual void s10();
 virtual void s11();
 virtual void s12();
 virtual void s13();
 virtual void s14();
 virtual void s15();
 virtual void s16();
 virtual void s17();
 virtual void s18();
 virtual void s19();
 virtual void s20();
 virtual void s21();
 virtual void s22();
 virtual void s23();
 virtual void s24();
 virtual void s25();
 virtual void s26();
 virtual void s27();
 virtual void s28();
 virtual void s29();
 virtual void s30();
 virtual void s31();
 virtual void s32();
 virtual void s33();
 virtual void s34();
 virtual void s35();
 virtual void s36();
 virtual void s37();
 virtual void s38();
 virtual void s39();
 virtual void s40();
 virtual void s41();
 virtual void s42();
 virtual void s43();
 virtual void s44();
 virtual void s45();
 virtual void s46();
 virtual void s47();
 virtual void s48();
 virtual void s49();
 virtual void s50();
 virtual void s51();
 virtual void s52();
 virtual void s53();
 virtual void s54();
 virtual void s55();
 virtual void s56();
 virtual void s57();
 virtual void s58();
 virtual void s59();
 virtual void s60();
 virtual void s61();
 virtual void s62();
 virtual void s63();
 virtual void s64();
 virtual void s65();
 virtual void s66();
 virtual void s67();
 virtual void s68();
 virtual void s69();
 virtual void s70();
 virtual void s71();
 virtual void s72();
 virtual void s73();
 virtual void s74();
 virtual bool IsAlive() const;                     // slot 75 (+0x12c)
};
inline bool CScriptThing::IsAlive() const { return PImp_Data && PImp_Data->IsAlive(); }
// NF78 (round 7, q10795; NF70 blocked/port): the global 0x0143E8F8 is pDSTGame, a CGameScriptInterfaceBase* (ESTABLISHED_SPELLINGS),
//   reached through the shared engine/CGameScriptInterfaceBase.h (FABLE_RETAIL_NSCRIPT, as the landed 0x00CBE716 / 0x00D9E930); the local
//   GameCalls interface model (845 slots), its forward declarations and the enum typedef stand-ins are gone. EScriptAIPriority gets the
//   enumerators the landed script rows (0x00D647F0, 0x00D62480) define.
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_LOWEST=0, SCRIPT_AI_PRIORITY_LOW=1, SCRIPT_AI_PRIORITY_MEDIUM=2, SCRIPT_AI_PRIORITY_HIGH=3, SCRIPT_AI_PRIORITY_HIGHEST=4 };
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CPredicateBase { public:virtual bool operator()()const=0;virtual CPredicateBase* Clone()const=0;virtual ~CPredicateBase(){}};
class CIsThingAlive:public CPredicateBase { public:
 CScriptThing Thing;
 FABLE_FORCEINLINE CIsThingAlive(const CScriptThing& t):Thing(t){}
 virtual bool operator()()const;virtual CPredicateBase* Clone()const;
 ~CIsThingAlive(){}
};
}
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"                              // CCharString (agentNF60: was a local stand-in)
struct CCountedPointerWords { void* Data; void* Info; CCountedPointerWords() : Data(0), Info(0) { } };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
class CScriptGameResourceObjectScriptedThingBase : public CBaseObject { public:   // vtable 0x0127094C
 CCountedPointerWords PImp;
 CScriptGameResourceObjectScriptedThingBase() : CBaseObject(), PImp() { }
 virtual ~CScriptGameResourceObjectScriptedThingBase();   // 0x007E74D0
 bool IsValid() const;                                // 0x00CD23B9
 void Release();                                    // 0x00CD2770
};
extern CGameScriptInterfaceBase* pDSTGame;   // 0x0143E8F8 ?pDSTGame@@3PAVCGameScriptInterfaceBase@@A (ESTABLISHED_SPELLINGS)
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
// agentNF60: the local CScriptDef model named these members descriptively; engine/CScriptDef.h (Ego_r / FableWin names at the
//   retail offsets) names them: LarderAlarmTime -> HC_HobbeGruntDelay (+0xcc8).
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/fable_script_def_global.h"          // pGlobalScriptDef 0x0143E90C (CDefPointer<const CScriptDef>; agentNF60: was a local CScriptDef model and `CScriptDef* pGlobalScriptDef`)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CQ_HobbeCaveScript {
 unsigned char BaseState[0x60];
 bool KingGruntedOrders;                            // +0x60 (PDB +0x74)
 unsigned char _pad61[0x6b - 0x61];
 bool HobbesCloseby;                                // +0x6B (PDB +0x7f)
};
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};

struct CHobbeLarder : public CEntityScriptBase {
 void* VTable; CGameScriptInterfaceBase* Game; CScriptThing Me;   // Ego_r.pdb CEntityScriptBase::Game (CGameScriptInterfaceBase*)
 CQ_HobbeCaveScript* ParentClass;                   // +0x14
 void* PMasterData;                                 // +0x18
 void Main();
};
void CHobbeLarder::Main()
{
 SetPredicate(NScript::CIsThingAlive(Me));
 Game->NewScriptFrame();
 if (IsActiveThreadTerminating()) return;
 CScriptGameResourceObjectScriptedThingBase handle;
 if (handle.IsValid())
  handle.Release();
 while (!Game->StartScriptingEntity(Me, handle, SCRIPT_AI_PRIORITY_HIGH)) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
 }
 if (IsActiveThreadTerminating()) return;
 CTimer timer;
 while (!ParentClass->KingGruntedOrders) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  if (Me.IsAwareOfHero() && timer.GetTime() == 0) {
   if (IsActiveThreadTerminating()) return;
   timer = pGlobalScriptDef->HC_HobbeGruntDelay;
  }
 }
 if (IsActiveThreadTerminating()) return;
 Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
 handle.Release();
 CScriptThing sacrifice = Game->GetThingWithScriptName("BanditSacrifice");
 while (sacrifice.IsAlive()) {
  Game->NewScriptFrame();
  if (IsActiveThreadTerminating()) return;
  if (!ParentClass->HobbesCloseby && IsDistanceBetweenThingsUnder(Me, sacrifice, 10.0f)) {
   if (IsActiveThreadTerminating()) return;
   ParentClass->HobbesCloseby = true;
  }
 }
 if (IsActiveThreadTerminating()) return;
}