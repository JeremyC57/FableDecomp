// NScript::CQ_OakValeBanditRaidScript::WatchForGateTimer() -- Watch the Oakvale gate, refresh the countdown and open or repopulate the region.
// Leaf: ?WatchForGateTimer@CQ_OakValeBanditRaidScript@NScript@@QAEXXZ
// Flags: default
// Grade: RELOCATION_MATCH
// Inline-budget: Limit inlining at the six late termination returns to recover the original timer and handle cleanup.
// Inferred: The two region-repopulation arms share the BanditsKilled reset.

#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/TRetVoid.h"
#include "engine/CQ_OakValeBanditRaidScript.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CRGBColour.h"
#include "engine/CTimer.h"
#include "engine/fable_script_distance.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CScriptThing::~CScriptThing() {}
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) {}
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }

void NScript::CQ_OakValeBanditRaidScript::WatchForGateTimer()
{
    TRetVoid _Fail_Val;
    CScriptThing Gate = Game->GetThingWithScriptName("OakValeBanditRaidGate");
    CScriptThing SoundMarker = Game->GetThingWithScriptName("DoorSoundMarker");
    for (;; Game->NewScriptFrame()) {
        if (IsActiveThreadTerminating()) return _Fail_Val.Return();
        while (!GateClosed) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        CTimer GateTimer;
        CTimer ShoutTimer;
        Game->DisplayQuestInfo(true);
        bool LeftRegion = false;
        bool Shouted = false;
        GateTimer = 30;
        ShoutTimer = 0;
        long BanditTimer = Game->AddQuestInfoBar(30.0f, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255), "HUD_CLOCK_ICON", "", 1.0f);
        while (GateTimer.GetTime() > 0 && GateClosed && !LeftRegion) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            long Fred = GateTimer.GetTime();
            Game->UpdateQuestInfoBar(BanditTimer, (float)(30 - GateTimer.GetTime()), -1.0f, -1.0f);
            if (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                LeftRegion = true;
            }
            if (BanditsAwareOfPlayerAgain == true) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                BanditsAwareOfPlayerAgain = false;
                GateTimer = 30;
            }
            if (IsDistanceBetweenThingsUnder(Game->GetHero(), SoundMarker, 16.0f)) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                GateTimer = 30;
                if (ShoutTimer.GetTime() <= 0) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    long HLStyleConversationIndex = Game->AddNewConversation(SoundMarker, false, false);
                    Game->AddPersonToConversation(HLStyleConversationIndex, Game->GetHero());
                    Game->AddLineToConversation(HLStyleConversationIndex, "TEXT_QST_047_HERO_AT_GATE", false, SoundMarker, Game->GetHero());
                    ShoutTimer = 8;
                }
            }
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        Game->DisplayQuestInfo(false);
        Game->RemoveQuestInfoElement(BanditTimer);
        NoPersistence = false;
        if (LeftRegion) {
            if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return _Fail_Val.Return();
#pragma inline_depth()
            }
            while (!Game->IsRegionLoaded("BanditCampPathEntrance")) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return _Fail_Val.Return();
#pragma inline_depth()
                }
            }
            if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return _Fail_Val.Return();
#pragma inline_depth()
            }
            NoPersistence = true;
            if (!BanditsKilled) {
                if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return _Fail_Val.Return();
#pragma inline_depth()
                }
                PopulateLevel();
                Game->OpenDoor(Game->GetThingWithScriptName("OakValeBanditRaidGate"));
                GateClosed = false;
            } else {
                if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return _Fail_Val.Return();
#pragma inline_depth()
                }
                OpenGates = true;
            }
            BanditsKilled = false;
        } else {
            if (IsActiveThreadTerminating())
            {
#pragma inline_depth(0)
                return _Fail_Val.Return();
#pragma inline_depth()
            }
            OpenGates = true;
        }
    }
}