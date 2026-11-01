// NScript::CQ_WizardBattleScript::MonitorMazePhases(CScriptThing& maze) -- thread: mark the maze boss on the minimap and as an evil-hero opinion source, then while it lives wait for each boss-phase change (one script frame at a time, the hero kept more than 5 units away), tracking the phase in MazePhase
// Grade: DIFFER(564v568) -- per-exit-site inlining of the unused handle's destructor: VC7.1 inlines ~CScriptGameResourceObjectScriptedThingBase only at the three exits inside the phase do-loop and calls 0x007E74D0 at the other six; retail inlines it at seven exits (the cross-jumper folds the copies into three register variants) and calls it only at the two MazeDefeated-arm exits. 15 shape probes + flags (probe/00e1cae0.log): the inline choice tracks loop nesting (a do{}while(false) wrapper inlines every exit), not caller size. Next: the real loop nesting of the script source (an enclosing construct that raises the outer-loop and final exits without the MazeDefeated arm), else lane gap: per-site dtor inlining (GOTCHAS chunk 583).
// Leaf: MonitorMazePhases
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E1CAE0, 568 bytes, thiscall, `ret 4`.
// Name: Ego_r.pdb ?MonitorMazePhases@CQ_WizardBattleScript@NScript@@QAEXAAVCScriptThing@@@Z (non-const CScriptThing&).
// Callees: CBaseObject() 0x0099A380 / ~CBaseObject 0x0099A430, ~CScriptGameResourceObjectScriptedThingBase 0x007E74D0,
//   operator delete 0x00BFE9BC, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall),
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 70 GetHero, 350 MiniMapAddMarker, 522 EntityGetBossPhase,
//   622 EntitySetAsOpinionSource(const CScriptThing&, const CCharString&); CScriptThing slot 75 IsAlive.
// Data: "HUD_ORB_QUEST_CORE" 0x01258AB0, "OPINION_SOURCE_HERO_EVIL_HIGH" 0x012D25C0, 5.0f.
// Layout: CLevelScriptBase<CQ_SunnyvaleMasterData> (Game +0x40, MasterScriptData +0x44); MazeDefeated +0x4F and MazePhase
//   +0x50 (Ego_r.pdb 99 / 100 - 0x14). The handle is the unused CScriptGameResourceObjectScriptedThingBase local
//   (inline ctor: CBaseObject call, vtable 0x0127094C, zeroed counted pointer from ebx).
class CCharString { public:
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CLevelScriptBase.h"
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_WizardBattleScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool VillagersTurnOnHero;                           // +0x48 (retail = Ego_r.pdb - 0x14)
    bool VillagersChased;                               // +0x49
    bool FightersStart;                                 // +0x4a
    bool MissionSucceeded;                              // +0x4b
    bool MissionFailed;                                 // +0x4c
    bool MissionOver;                                   // +0x4d
    bool QuestStartScreenShown;                         // +0x4e
    bool MazeDefeated;                                  // +0x4f
    long MazePhase;                                     // +0x50
    void MonitorMazePhases(CScriptThing& maze);
};
}
void NScript::CQ_WizardBattleScript::MonitorMazePhases(CScriptThing& maze)
{
    CScriptGameResourceObjectScriptedThingBase unusedHandle;
    Game->MiniMapAddMarker(maze, "HUD_ORB_QUEST_CORE");
    MazePhase = Game->EntityGetBossPhase(maze);
    Game->EntitySetAsOpinionSource(maze, "OPINION_SOURCE_HERO_EVIL_HIGH");
    while (maze.IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        long phase = MazePhase;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
            while (!IsDistanceBetweenThingsOver(Game->GetHero(), maze, 5.0f)) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            MazePhase = Game->EntityGetBossPhase(maze);
        } while (MazePhase == phase);
        if (IsActiveThreadTerminating())
            return;
        if (maze.IsAlive() && MazeDefeated == false) {
            if (IsActiveThreadTerminating())
                return;
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
