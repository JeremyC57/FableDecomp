// NScript::CV_FisticuffsClubScript::~CV_FisticuffsClubScript() -- implicit quest-script destructor: destroy RegionNames[5], Village, SFighterTwo, SFighterOne and deregister Fighter2Timer / Fighter1Timer in reverse order, then ~CScriptBase
// Leaf: ??1CV_FisticuffsClubScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8B750, 248 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00E8B733 is the scalar deleting destructor 0x00E8B730, slot 0 of the RTTI-less vtable
//   0x012EAF14; that vtable is stored only by the ctor 0x00E81F60, whose only caller is the Alloc 0x00E8B700 (`push 0C8h ;
//   call new ; ... call 0x00E81F60`), stored at 0x00CD8964 by the script-registration routine right after the level name
//   "V_FisticuffsClub" (0x012C5BC4) and the save tag "S_VFCC" (0x012F770C): NScript::CV_FisticuffsClubScript (Ego_r.pdb /
//   FableWin.pdb ??1CV_FisticuffsClubScript@NScript@@UAE@XZ). The body stores no vtable of its own: the destructor is
//   compiler-generated. Catalog label right.
// Layout: Ego_r.pdb CV_FisticuffsClubScript members, 0x14 lower in retail: RelevantRegion +0x48 (an unnamed enum in the PDB;
//   its enumerators are not in the type dump), thirteen bools +0x4C..+0x58, Fighter1Timer +0x5C, Fighter2Timer +0x60,
//   SFighterOne +0x64, SFighterTwo +0x70, HitID..FightCount +0x7C..+0x88, three bools +0x8C, AllowedHits +0x90, five
//   Completed*PunchClub bools +0x94, Village +0x9C, PlayerLost +0xA8, CheerNumber/BooNumber +0xAC/+0xB0, RegionNames[5]
//   +0xB4; retail size 0xC8 (Alloc).
// Callees: ~CCharString 0x0099EAE0, ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release:
//   DeleteFunc, operator delete 0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0, CTimer::~CTimer inline (engine/CTimer.h:
//   g_pGameScriptInterface 0x0143E8F8 slot 88 DeregisterTimer), ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor ??0CV_FisticuffsClubScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined with an empty body
//   (retail's TU defines it), which emits the vtable, the scalar deleting dtor and this implicit dtor; only the dtor is graded.
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
inline CScriptThing::~CScriptThing() { }
#include "engine/CScriptBase.h"

class CCharStringData;
class CCharString { public:
    CCharString();
    ~CCharString();                                         // 0x0099EAE0
    CCharStringData* PStringData;
};

class CGameScriptInterfaceBase;
template <class T> class CLevelScriptBase : public CScriptBase { public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData((T*)data) {}
    T* GetMasterData();
    CGameScriptInterfaceBase* Game;                         // +0x40
    T*                        MasterScriptData;             // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_FisticuffsClubScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    CV_FisticuffsClubScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    enum { } RelevantRegion;                                // +0x48 (Ego_r <unnamed-type-RelevantRegion>; enumerators not in the PDB dump)
    bool         FighterOneInRing;                          // +0x4C
    bool         FighterTwoInRing;                          // +0x4D
    bool         ChallengeAccepted;                         // +0x4E
    bool         FightFinished;                             // +0x4F
    bool         HeroInsideRing;                            // +0x50
    bool         HeroOfferedFisticuffsClubEntry;            // +0x51
    bool         HeroPaidEntryFee;                          // +0x52
    bool         HeroInFisticuffsClub;                      // +0x53
    bool         TylerInFight;                              // +0x54
    bool         HeroBeatTyler;                             // +0x55
    bool         TimeOut;                                   // +0x56
    bool         RingCleared;                               // +0x57
    bool         RoundStarted;                              // +0x58
    CTimer       Fighter1Timer;                             // +0x5C
    CTimer       Fighter2Timer;                             // +0x60
    CScriptThing SFighterOne;                               // +0x64
    CScriptThing SFighterTwo;                               // +0x70
    long         HitID;                                     // +0x7C
    long         HitCount;                                  // +0x80
    long         AttackWarnings;                            // +0x84
    long         FightCount;                                // +0x88
    bool         EverybodyAttack;                           // +0x8C
    bool         LostWhileOutside;                          // +0x8D
    bool         FisticuffsActive;                          // +0x8E
    long         AllowedHits;                               // +0x90
    bool         CompletedThisPunchClub;                    // +0x94
    bool         CompletedKnotholePunchClub;                // +0x95
    bool         CompletedBowerstonePunchClub;              // +0x96
    bool         CompletedBanditPunchClub;                  // +0x97
    bool         CompletedOakvalePunchClub;                 // +0x98
    CScriptThing Village;                                   // +0x9C
    bool         PlayerLost;                                // +0xA8
    float        CheerNumber;                               // +0xAC
    float        BooNumber;                                 // +0xB0
    CCharString  RegionNames[5];                            // +0xB4
};
CV_FisticuffsClubScript::CV_FisticuffsClubScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00E81F60
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_FisticuffsClubScript) == 0xC8);