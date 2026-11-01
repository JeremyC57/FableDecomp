// NScript::CV_FisticuffsClubScript::~CV_FisticuffsClubScript() -- probe
// Leaf: ??1CV_FisticuffsClubScript@NScript@@UAE@XZ
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
