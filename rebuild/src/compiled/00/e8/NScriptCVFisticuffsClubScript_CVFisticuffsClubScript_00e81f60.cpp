// CV_FisticuffsClubScript::CV_FisticuffsClubScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012EAF14, the two fighter timers, the SFighterOne/SFighterTwo/Village script things and the five RegionNames strings
// Leaf: ??0CV_FisticuffsClubScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E81F60, 167 bytes (padded to 176), `ret 8`.
// Identity: factory 0x00E8B700 (`push 0C8h`, this ctor) registered at 0x00CD8968; size 0xc8 = Ego_r.pdb 220 - 0x14
//   (the retail CScriptBase is 0x14 bytes smaller), the members line up at PDB - 0x14.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CGameScriptInterface slot 87 RegisterTimer (+0x15c, through the
//   global interface 0x0143E8F8), CBaseClass::CBaseClass 0x0099A2D0 (CScriptThing base, CScriptThing vtable 0x01238C8C),
//   CCharString::CCharString() 0x0099E4B0 (the RegionNames array, an inline counted `dec ebx ; jne` loop).
// Shape: the two argument words stored before the derived vtable are the inlined `__declspec(novtable)` intermediate base
//   (landed CV_MurderTwistFinalScript / CV_BordelloScript model); timers are member objects whose inline ctor calls the
//   interface; script things are CBaseClass + zeroed counted pointer; plain scalars are left uninitialised.
// Layout (retail = PDB - 0x14): RelevantRegion +0x48, flags +0x4c..+0x58, Fighter1Timer +0x5c, Fighter2Timer +0x60,
//   SFighterOne +0x64, SFighterTwo +0x70, HitID..FightCount +0x7c..+0x88, flags +0x8c, AllowedHits +0x90,
//   flags +0x94..+0x98, Village +0x9c, PlayerLost +0xa8, CheerNumber +0xac, BooNumber +0xb0, RegionNames +0xb4.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
template<class T> class CCountedPointer { public: T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::CScriptThing() : CBaseClass(), PImp() { }
class CCharString { public:                               // 4 bytes (data block pointer)
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
namespace NScript {
class CV_FisticuffsClubScript : public CQuestScriptObject {
public:
    long RelevantRegion;                           // +0x48
    bool FighterOneInRing, FighterTwoInRing, ChallengeAccepted, FightFinished, HeroInsideRing,
         HeroOfferedFisticuffsClubEntry, HeroPaidEntryFee, HeroInFisticuffsClub, TylerInFight, HeroBeatTyler,
         TimeOut, RingCleared, RoundStarted;       // +0x4c..+0x58
    CTimer Fighter1Timer;                          // +0x5c
    CTimer Fighter2Timer;                          // +0x60
    CScriptThing SFighterOne;                      // +0x64
    CScriptThing SFighterTwo;                      // +0x70
    long HitID;                                    // +0x7c
    long HitCount;                                 // +0x80
    long AttackWarnings;                           // +0x84
    long FightCount;                               // +0x88
    bool EverybodyAttack, LostWhileOutside, FisticuffsActive;   // +0x8c
    long AllowedHits;                              // +0x90
    bool CompletedThisPunchClub, CompletedKnotholePunchClub, CompletedBowerstonePunchClub,
         CompletedBanditPunchClub, CompletedOakvalePunchClub;   // +0x94..+0x98
    CScriptThing Village;                          // +0x9c
    bool PlayerLost;                               // +0xa8
    float CheerNumber;                             // +0xac
    float BooNumber;                               // +0xb0
    CCharString RegionNames[5];                    // +0xb4
    virtual ~CV_FisticuffsClubScript();
    CV_FisticuffsClubScript(void* data, CGameScriptInterface* game);
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;
NScript::CV_FisticuffsClubScript::CV_FisticuffsClubScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}