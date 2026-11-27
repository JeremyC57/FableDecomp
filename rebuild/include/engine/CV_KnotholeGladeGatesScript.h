#ifndef FABLE_ENGINE_CV_KNOTHOLEGLADEGATESSCRIPT_H
#define FABLE_ENGINE_CV_KNOTHOLEGLADEGATESSCRIPT_H
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CScriptThing.h"
namespace NScript {
class CV_KnotholeGladeGatesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool MissionSucceeded;                                  // +0x48
    bool MissionFailed;                                     // +0x49
    bool MissionOver;                                       // +0x4a
    bool Cutscene1Start;                                    // +0x4b
    bool HeroAlertedToProblem;                              // +0x4c
    bool GatesOpened;                                       // +0x4d
    bool SaidBefore;                                        // +0x4e
    bool HeroKilledAVillager;                               // +0x4f
    bool IntroductionFinished;                              // +0x50
    bool BalvsKilled;                                       // +0x51
    bool CutsceneStarting;                                  // +0x52
    bool Mentioned;                                         // +0x53
    bool ToldSuccess;                                       // +0x54
    int BalvKillCount;                                     // +0x58
    CScriptThing TheGate;                                   // +0x5c
    bool FirstBalvDead;
    void ControlMainBalverines();
};
}
#endif
