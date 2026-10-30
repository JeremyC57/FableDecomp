// NScript::CV_IntroductionToTrophiesScript::WatchForCrowdVolume() -- each frame, when Briar Rose's opinion has caught up with the required stage, advance it: stages 0 and 1 run the inside-tavern timer (restarted on entering the tavern, held while outside) and move the requirement to 1 / 2 when it expires; stage 2 waits for a region load and resets the requirement to 0
// Leaf: ?WatchForCrowdVolume@CV_IntroductionToTrophiesScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E9C8D0, 582 bytes, thiscall, plain `ret`.
// Identity (proven): spawned as "WatchForCrowdVolume" (0x012ED588) by the landed CV_IntroductionToTrophiesScript::Main 0x00E9C4B0:
//   the CSpawnedFunc Func store at 0x00E9C711 writes this row right after that literal; the row has no other reference.
//   Ego_r.pdb ?WatchForCrowdVolume@CV_IntroductionToTrophiesScript@NScript@@QAEXXZ. Catalog label sub_00e9c8d0.
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating 0x00CB7940; Game (+0x40)
//   slots 7 NewScriptFrame, 22 MsgOnRegionLoaded; g_pGameScriptInterface 0x0143E8F8 slots 89 SetTimer (inline CTimer::operator=)
//   and 90 GetTimer (inline CTimer::GetTime); pGlobalScriptDef 0x0143E90C.
// Layout: retail NScript headers (CLevelScriptBase 0x48); CV_IntroductionToTrophiesScript members at Ego_r.pdb - 0x14:
//   InsideTavern +0x55, OpinionCurrent +0x58, OpinionRequired +0x5c (BriarRoseOpinion), OpinionTimerReset +0x61,
//   OpinionTimer +0x64. CScriptDef TrophiesInsideTavernTimer at retail +0x6cc (Ego_r +0x728 less the 0x5c delta of chunk 581).
// Shape: one `while (!IsActiveThreadTerminating())` loop (rotated by VC7.1), an if / else-if chain on OpinionCurrent, every
//   script statement guarded by its own terminate check (the sibling WatchForInsideTavern 0x00E9CB20 style); the CTimer
//   operators are `__forceinline` after engine/CTimer.h.
class CCharString {
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterface.h"

extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
__forceinline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
__forceinline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }

class CScriptDef {
public:
    unsigned char _before[0x6cc];
    long TrophiesInsideTavernTimer;                     // +0x6cc
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C

namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_IntroductionToTrophiesScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum BriarRoseOpinion { };
    bool DoneBriarIntroSpeech;                          // +0x48
    bool DoneBriarTrophySpeech;                         // +0x49
    bool AttackedByHero;                                // +0x4a
    bool DoneApproachMessage1;                          // +0x4b
    bool DoneApproachMessage2;                          // +0x4c
    bool AttackSpeechComplete;                          // +0x4d
    long HitByHeroCount;                                // +0x50
    bool HitWarningComplete;                            // +0x54
    bool InsideTavern;                                  // +0x55
    bool QuestComplete;                                 // +0x56
    BriarRoseOpinion OpinionCurrent;                    // +0x58
    BriarRoseOpinion OpinionRequired;                   // +0x5c
    bool CutsceneStarted;                               // +0x60
    bool OpinionTimerReset;                             // +0x61
    bool OutsideTavernTimerReset;                       // +0x62
    CTimer OpinionTimer;                                // +0x64
    CTimer OutsideTavernTimer;                          // +0x68
    void WatchForCrowdVolume();
};

void CV_IntroductionToTrophiesScript::WatchForCrowdVolume()
{
    CCharString region;
    while (!IsActiveThreadTerminating()) {
        if (OpinionCurrent == OpinionRequired) {
            if (IsActiveThreadTerminating())
                return;
            if (OpinionCurrent == 0) {
                if (IsActiveThreadTerminating())
                    return;
                if (InsideTavern) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (!OpinionTimerReset) {
                        if (IsActiveThreadTerminating())
                            return;
                        OpinionTimer = pGlobalScriptDef->TrophiesInsideTavernTimer;
                        OpinionTimerReset = true;
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    OpinionTimer = pGlobalScriptDef->TrophiesInsideTavernTimer;
                }
                if (OpinionTimer.GetTime() == 0 && OpinionTimerReset) {
                    if (IsActiveThreadTerminating())
                        return;
                    OpinionRequired = (BriarRoseOpinion)1;
                    OpinionTimerReset = false;
                }
            } else if (OpinionCurrent == 1) {
                if (IsActiveThreadTerminating())
                    return;
                if (InsideTavern) {
                    if (IsActiveThreadTerminating())
                        return;
                    if (!OpinionTimerReset) {
                        if (IsActiveThreadTerminating())
                            return;
                        OpinionTimer = pGlobalScriptDef->TrophiesInsideTavernTimer;
                        OpinionTimerReset = true;
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    OpinionTimer = pGlobalScriptDef->TrophiesInsideTavernTimer;
                }
                if (OpinionTimer.GetTime() == 0 && OpinionTimerReset) {
                    if (IsActiveThreadTerminating())
                        return;
                    OpinionRequired = (BriarRoseOpinion)2;
                    OpinionTimerReset = false;
                }
            } else if (OpinionCurrent == 2) {
                if (IsActiveThreadTerminating())
                    return;
                while (!Game->MsgOnRegionLoaded(region)) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
                OpinionRequired = (BriarRoseOpinion)0;
            }
        }
        Game->NewScriptFrame();
    }
}
}   // namespace NScript