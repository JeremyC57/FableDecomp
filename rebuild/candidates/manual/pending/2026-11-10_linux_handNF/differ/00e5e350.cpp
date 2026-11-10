// 00e5e350 NScript::CV_BodyGuardScript::HenchmanCommentary() -- maintain henchman chatter and hero inactivity state.
// Grade: DIFFER(584v585) -- missing first timer initialization store; conditional branch encodings and alignment differ
// Inline-budget: native 00e5e580/00e5e58d call both timer destructors at this exit only; plain and per-row budget trials failed.
// Leaf: ?HenchmanCommentary@CV_BodyGuardScript@NScript@@QAEXXZ
// Retail: 0x00e5e350, 585 bytes, member call with no arguments.
// Flags:
#define FABLE_RETAIL_CTIMER_ORIGINAL_ACCESS
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTER
#define FABLE_RETAIL_C3DVECTOR_NOT_EQUAL
#define FABLE_RETAIL_CV_BODYGUARDSCRIPT_COMMENTARY
#include "engine/C3DVector.h"
#include "engine/TRetVoid.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CV_BodyGuardScript.h"
extern CGameScriptInterfaceBase* pDSTGame;
inline CTimer::CTimer() : TimerIndex(pDSTGame->RegisterTimer()) { }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex,time); return *this; }
inline long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
void NScript::CV_BodyGuardScript::HenchmanCommentary()
{
    CTimer IdleConversationTimer;
    CTimer CheckExpensiveThingsTimer;
    TRetVoid _Fail_Val;
    C3DVector HeroLastPosition;
    while (!IsActiveThreadTerminating()) {
        while (HenchmanList.size() == 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        if (MasterScriptData->HenchmanCommentaryState && CommentaryTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            MasterScriptData->HenchmanCommentaryState = 0;
        }
        if (CheckExpensiveThingsTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            CheckExpensiveThingsTimer=1;
            HeroWieldingWeapon = Game->IsPlayerWieldingWeapon();
        }
        if (HeroLastPosition != Game->GetHero().GetPos()) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            HeroStoodStillCounter = 0.0f;
        }
        HeroLastPosition = Game->GetHero().GetPos();
        if (IdleConversationTimer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            if (MakeHenchmanPerformIdleComment()) {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                IdleConversationTimer=5;
            }
        }
        Game->NewScriptFrame();
    }
}
