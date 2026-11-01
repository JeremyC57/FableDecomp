// NScript::CV_TourGuideScript::~CV_TourGuideScript() -- implicit quest-script destructor: destroy NextTourWaypoint, the three CCharString[5] response arrays and WaypointInfo[18] in reverse order, then ~CScriptBase
// Leaf: ??1CV_TourGuideScript@NScript@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EE6C50, 215 bytes, thiscall, tail jump to ~CScriptBase 0x00CBD510.
// Identity (proven): the sole caller 0x00EE6C33 is the scalar deleting destructor 0x00EE6C30, slot 0 of the RTTI-less vtable
//   0x012F46B8; that vtable is stored only by the ctor 0x00EE41F0, whose only caller is the Alloc 0x00EE6C00 (`push 178h ;
//   call new ; ... call 0x00EE41F0`), stored at 0x00CD97BC by the script-registration routine right after the level name
//   "V_TourGuide" (0x012C57FC) and the save tag "S_VTG" (0x012F762C): NScript::CV_TourGuideScript (Ego_r.pdb / FableWin.pdb
//   ??1CV_TourGuideScript@NScript@@UAE@XZ; Ego_r.pdb ??_GCV_TourGuideScript@NScript@@UAEPAXI@Z). The body stores no vtable of its own:
//   the destructor is compiler-generated. The catalog label "CActionTalkedTo::~CActionTalkedTo" is propagated and wrong
//   (CActionTalkedTo is NTCScriptedControl::CActionTalkedTo, an RTTI class; this vtable has no COL).
// Layout: Ego_r.pdb CV_TourGuideScript members, 0x14 lower in retail (CScriptBase 0x40 vs Ego_r's Dinkumware 0x54):
//   WaypointInfo[18] +0x48 (WaypointData {CCharString locMarker, locTextOverheard, locTextRequested}, 12 bytes), seven bools
//   +0x120..+0x126, RandomGuideResponse[5] +0x128, RandomFollowerResponseM[5] +0x13C, RandomFollowerResponseF[5] +0x150,
//   WaypointCounter +0x164, NextTourWaypoint +0x168 (CScriptThing), two bools +0x174/+0x175; retail size 0x178 (Alloc).
// Callees: ~CScriptThing inline (vtable 0x01238C8C store, engine/CCountedPointer.h PImp release: DeleteFunc, operator delete
//   0x00BFE9BC), CBaseClass::~CBaseClass 0x0099A2E0, ~CCharString 0x0099EAE0, ~CScriptBase 0x00CBD510.
// Layout: engine/CLevelScriptBase.h declares `virtual ~CLevelScriptBase();` out of line; retail's is implicit (chunks 664/665),
//   so CLevelScriptBase<T> is a local copy of the header without the dtor declaration (NOTES.md "Header debt"). No
//   engine/CCharString.h exists: CCharString is the one-pointer local model the landed quest rows use.
// TU: the PDB ctor ??0CV_TourGuideScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z is defined with an empty body
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