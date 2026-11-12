// NScript::CQ_FireHeartScript::RunVillagerComments() -- spawned script thread (from Main 0x00D35090): until the mission
//   succeeds, every 25 seconds collect the villagers within 5 units of the hero (squared distance <= 25.0f) into a
//   candidate list, wait (frame by frame) until at least one is near, then flag one picked at random to speak.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38D20, length 526 bytes (0x21E).
// Leaf: ?RunVillagerComments@CQ_FireHeartScript@NScript@@QAEXXZ
// Callees: vector<long>::vector(size_type) 0x00D38F30 (out of line, FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE), free 0x00BFEA14
//   (inline ~vector copies), rand 0x00BFEB16, GetSquaredDistanceBetweenThings 0x00CBE583 (__fastcall, float),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through Game (+0x40): NewScriptFrame (slot 7), GetHero (slot 70);
//   the timer service pDSTGame 0x0143E8F8: RegisterTimer (87), DeregisterTimer (88), SetTimer (89), GetTimer (90),
//   all through the inline CTimer ctor / dtor / operator= / GetTime.
// Shape: the local TRetVoid _Fail_Val (the script macros' `return _Fail_Val.Return();`) is what fixes the exit-tail
//   layout: without it VC7.1 places the normal-exit cleanup (inline ~vector + ~CTimer) after the return copies.
//   ShouldVillagerSpeak is the retail plain byte vector<bool>; Villagers[i] is evaluated before the GetHero vcall
//   (fastcall arguments right to left).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CEntityScriptBase.h"
#include "stlport/fable_map.h"
#include "engine/CQ_FireHeartScript.h"
#include "engine/CTimer.h"
#include "engine/TRetVoid.h"
#include <stdlib.h>
extern CGameScriptInterfaceBase* pDSTGame;                 // 0x0143E8F8
inline CTimer::CTimer() { TimerIndex = pDSTGame->RegisterTimer(); }
inline CTimer::~CTimer() { pDSTGame->DeregisterTimer(TimerIndex); }
FABLE_FORCEINLINE CTimer& CTimer::operator=(long time) { pDSTGame->SetTimer(TimerIndex, time); return *this; }
FABLE_FORCEINLINE long CTimer::GetTime() const { return pDSTGame->GetTimer(TimerIndex); }
float FABLE_FASTCALL GetSquaredDistanceBetweenThings(const CScriptThing&, const CScriptThing&);   // 0x00CBE583
void NScript::CQ_FireHeartScript::RunVillagerComments()
{
    TRetVoid _Fail_Val;
    CTimer timer;
    std::vector<long> candidates(NumberOfVillagers);
    while (!MissionSucceeded) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        timer = 25;
        while (timer.GetTime() != 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        }
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        long count;
        do {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
            count = 0;
            for (long i = 0; i < NumberOfVillagers; ++i) {
                if (IsActiveThreadTerminating())
                    return _Fail_Val.Return();
                if (GetSquaredDistanceBetweenThings(Game->GetHero(), Villagers[i]) <= 25.0f) {
                    if (IsActiveThreadTerminating())
                        return _Fail_Val.Return();
                    candidates[count] = i;
                    count++;
                }
            }
            if (IsActiveThreadTerminating())
                return _Fail_Val.Return();
        } while (count <= 0);
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
        ShouldVillagerSpeak[candidates[rand() % count]] = true;
        if (IsActiveThreadTerminating())
            return _Fail_Val.Return();
    }
    if (IsActiveThreadTerminating())
        return _Fail_Val.Return();
}