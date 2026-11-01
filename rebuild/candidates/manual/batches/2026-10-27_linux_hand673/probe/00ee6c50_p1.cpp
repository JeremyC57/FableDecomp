// NScript::CV_TourGuideScript::~CV_TourGuideScript() -- probe
// Leaf: ??1CV_TourGuideScript@NScript@@UAE@XZ
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
class CV_TourGuideScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> { public:
    struct WaypointData {
        WaypointData();
        CCharString locMarker;                              // +0x00
        CCharString locTextOverheard;                       // +0x04
        CCharString locTextRequested;                       // +0x08
    };
    CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    WaypointData WaypointInfo[18];                          // +0x48
    bool         TourGuideKilled;                           // +0x120
    bool         AllFollowersDead;                          // +0x121
    bool         QuestFinished;                             // +0x122
    bool         TourFinished;                              // +0x123
    bool         QuestActivated;                            // +0x124
    bool         Initialise;                                // +0x125
    bool         SpawnedQuestFinishThread;                  // +0x126
    CCharString  RandomGuideResponse[5];                    // +0x128
    CCharString  RandomFollowerResponseM[5];                // +0x13C
    CCharString  RandomFollowerResponseF[5];                // +0x150
    long         WaypointCounter;                           // +0x164
    CScriptThing NextTourWaypoint;                          // +0x168
    bool         GuideSpokenToHeroThisWaypoint;             // +0x174
    bool         OverheardTourGuideThisWaypoint;            // +0x175
};
CV_TourGuideScript::CV_TourGuideScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)   // 0x00EE41F0
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_TourGuideScript) == 0x178);
