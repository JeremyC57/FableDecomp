// NScript::CQ_FireHeartScript::StartRound() -- start a Fireheart minigame round: enter the minigame, lay out the puzzle blocks, use the overhead camera, fade in and play the start sound; the first time (with the tutorial system on) show the three info pages with the HUD off; clear the round's comment flags, start the round timer (per-round limit, shown as a HUD clock) and a heartbeat timer, then until the round ends or the clock runs out play a fast/medium/slow heartbeat by the time left, give the time warning once, and when the clock hits zero zap the hero (quit the minigame, play the zapped cutscene, take health) and spawn the next StartRound; finally remove the clock
// Grade: RELOCATION_MATCH
// Leaf: ?StartRound@CQ_FireHeartScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D39960, 1774 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb / FableWin.pdb ?StartRound@CQ_FireHeartScript@NScript@@QAEXXZ (engine/CQ_FireHeartScript.h 0x00D39960).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, ~CScriptThing (inline: ~CBaseClass 0x0099A2E0 +
//   the CCountedPointer release), operator new 0x00BFEA1A, CSpawnedFuncBase::CSpawnedFuncBase 0x00CDD450 (CSpawnedFunc vtable
//   0x012CCBA4), CScriptBase::IsActiveThreadTerminating 0x00CB7940 / AddSpawnedFunction 0x00CB7E50, CQ_FireHeartScript::
//   SetupPuzzleBlocks 0x00D36140 / TriggerProphetComment 0x00D38400 / PlayCutscene(CCharString) 0x00D39330 (all landed);
//   pDSTGame 0x0143E8F8 slots 87-90 (the inline CTimer); Game (+0x40) slots 7, 40, 72, 115, 119, 254, 255, 263, 321, 326,
//   338, 357, 374, 411, 692; pGlobalScriptDef 0x0143E90C (FHTimeLimit +0xc38, FHWarnTime +0xc44, FHHealthChangeWhenZapped +0xc4c).
// Layout: engine/CQ_FireHeartScript.h (PuzzleRound +0x88, RoundEnded +0x94, RoundBeginning +0x95, PreventRoundEnd +0x96,
//   InfoDisplayed +0x99, CommentGivenThisRound +0xf8); overlay opt-in FABLE_RETAIL_CQ_FIREHEARTSCRIPT_TRIGGERPROPHETCOMMENT.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CQ_FIREHEARTSCRIPT_TRIGGERPROPHETCOMMENT
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"
#include "engine/CTimer.h"
#include "engine/fable_script_def_global.h"
#include "engine/TRetVoid.h"
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CScriptThing::~CScriptThing() { }

void NScript::CQ_FireHeartScript::StartRound()
{
    TRetVoid _Fail_Val;
    RoundBeginning = true;
    Game->HeroPlayFireheartMinigame();
    RoundEnded = false;
    SetupPuzzleBlocks();
    RoundBeginning = false;
    Game->CameraUseCameraPoint(Game->GetThingWithScriptName("CAM_FH_OVERHEAD"), 0, 0, -1.0f, 1, 1);
    Game->FadeScreenIn();
    Game->Play2DSound("SND_MM_MINIGAME_START");
    if (!InfoDisplayed) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (Game->IsTutorialSystemEnabled()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->SetHUDEnabled(false);
            Game->DisplayGameInfo("TEXT_QST_B01_MINIGAME_INFO_10");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->DisplayGameInfo("TEXT_QST_B01_MINIGAME_INFO_20");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->DisplayGameInfo("TEXT_QST_B01_MINIGAME_INFO_30");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            Game->SetHUDEnabled(true);
        }
        InfoDisplayed = true;
    }
    for (long i = 0; i < 5; ++i) {
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CommentGivenThisRound[i] = false;
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    long warnTime = pGlobalScriptDef->FHWarnTime;
    CTimer roundTimer;
    roundTimer = pGlobalScriptDef->FHTimeLimit[PuzzleRound];
    Game->DisplayQuestInfo(true);
    long clock = Game->AddQuestInfoTimer(roundTimer, "HUD_CLOCK_ICON", 1.5f);
    CTimer beatTimer;
    beatTimer = 3;
    while (roundTimer.GetTime() >= 0 && !RoundEnded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (beatTimer.GetTime() <= 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (roundTimer.GetTime() <= 10) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_FAST");
                beatTimer = 1;
            } else if (roundTimer.GetTime() <= 20) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_MEDIUM");
                beatTimer = 2;
            } else {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                Game->Play2DSound("SND_MM_FIREHEART_BEAT_SLOW");
                beatTimer = 3;
            }
        }
        if (roundTimer.GetTime() <= warnTime && !CommentGivenThisRound[4]) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            TriggerProphetComment("TIMEWARNING");
            CommentGivenThisRound[4] = true;
        } else if (roundTimer.GetTime() == 0 && !RoundEnded && !PreventRoundEnd) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            RoundEnded = true;
            Game->HeroQuitFireheartMinigame();
            PlayCutscene("CS_FIREHEART_HEROZAPPED");
            Game->ChangeHeroHealthBy(pGlobalScriptDef->FHHealthChangeWhenZapped, true, false);
            AddSpawnedFunction(new CSpawnedFunc<CQ_FireHeartScript>("StartRound", 0, this, &CQ_FireHeartScript::StartRound),
                               CCharString(""));
        }
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
    Game->RemoveQuestInfoElement(clock);
}
