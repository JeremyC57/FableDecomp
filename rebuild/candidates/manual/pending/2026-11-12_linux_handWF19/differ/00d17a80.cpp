// NScript::CQ_BountyHuntScript::WatchForBoasts() -- per script frame: read the boasts made this frame (Game->MsgOnBoastsMade);
// Grade: DIFFER(705v785) -- body and spawn match byte for byte; only the vector destruction at the 5 early returns differs: retail keeps 5 separate exits with a per-site inline split (for-top: ??_G pair 0x004DA68A(0) loop + ~_Vector_base 0x004D9C5D call; the two boast-arm returns: inline ~CCharString loop + 0x004D9C5D call; expiry and post-loop returns fully inline) that fits an inline budget spent in REVERSE source order; VC7.1 here spends it forward and expands every site (tail-merged).
//   as 5 separate exits with a per-site inline split (for-top: ??_G pair 0x004DA68A(0) loop + ~_Vector_base 0x004D9C5D call; the two
//   boast-arm returns: inline ~CCharString loop + 0x004D9C5D call; expiry and post-loop returns fully inline), ours expands every site
//   fully and tail-merges them. /d2inlT shows VC7.1 spends the inline budget on the return sites in FORWARD source order here; retail's
//   split is exactly the REVERSE order (the earliest site and even the vector ctor 0x0074F680 got the least).
// the time-limit boast (15) arms BoastActiveTimeLimit and spawns WatchForQuestInfoDisplayConditions, the decapitation boast (16) arms
// BoastActiveDecapitation; once the time-limit boast's timer has run out, set MasterScriptData->BountyHuntTimeLimitExceeded.
// Retail: Fable.exe 0x00D17A80, 785 bytes, plain `ret`, no frame pointer, no EH.
// Callees: CCharString() 0x0099E4B0 / operator= 0x0099EFB0 / (const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
//   IsActiveThreadTerminating 0x00CB7940; vector<pair<long,CCharString> >(const allocator&) 0x0074F680 (out of line);
//   ??_G pair<long,CCharString> 0x004DA68A, ~_Vector_base 0x004D9C5D (landed as a descriptive placeholder), free 0x00BFEA14;
//   operator new 0x00BFEA1A, CSpawnedFuncBase(const CCharString&, ulong) 0x00CDD450 (CSpawnedFunc<CQ_BountyHuntScript> vtable
//   0x012C9EDC, Func 0x00D17DA0 WatchForQuestInfoDisplayConditions), AddSpawnedFunction 0x00CB7E50;
//   CGameScriptInterfaceBase slots 7 NewScriptFrame, 34 MsgOnBoastsMade, 90 GetTimer (CTimer::GetTime via g_pGameScriptInterface 0x0143E8F8).
// Header opt-in (overlay): FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS (declares WatchForBoasts / WatchForQuestInfoDisplayConditions).
// Leaf: ?WatchForBoasts@CQ_BountyHuntScript@NScript@@QAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_CQ_BOUNTYHUNTSCRIPT_BOASTS
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "stlport/fable_containers.h"
#include "engine/CQ_BountyHuntScript.h"
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"

extern CGameScriptInterfaceBase* g_pGameScriptInterface;                                         // 0x0143E8F8
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
template <> std::vector<std::pair<long, CCharString>, std::allocator<std::pair<long, CCharString> > >::vector(const std::allocator<std::pair<long, CCharString> >&); // 0x0074F680

void NScript::CQ_BountyHuntScript::WatchForBoasts()
{
    CCharString boastName;
    while (!IsActiveThreadTerminating()) {
        {
            std::vector<std::pair<long, CCharString> > boasts;
            Game->MsgOnBoastsMade(boasts);
            for (unsigned long i = 0; i != boasts.size(); ++i) {
                if (IsActiveThreadTerminating())
                    return;
                long boast = boasts[i].first;
                boastName = boasts[i].second;
                if (boast == 15) {
                    if (!BoastActiveTimeLimit) {
                        if (IsActiveThreadTerminating())
                            return;
                        BoastActiveTimeLimit = true;
                        AddSpawnedFunction(new CSpawnedFunc<CQ_BountyHuntScript>("WatchForQuestInfoDisplayConditions", 0, this, &CQ_BountyHuntScript::WatchForQuestInfoDisplayConditions), CCharString(""));
                    }
                } else if (boast == 16) {
                    if (!BoastActiveDecapitation) {
                        if (IsActiveThreadTerminating())
                            return;
                        BoastActiveDecapitation = true;
                    }
                }
                if (BoastActiveTimeLimit && BoastTime.GetTime() <= 0 && !MasterScriptData->BountyHuntTimeLimitExceeded) {
                    if (IsActiveThreadTerminating())
                        return;
                    MasterScriptData->BountyHuntTimeLimitExceeded = true;
                }
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}
